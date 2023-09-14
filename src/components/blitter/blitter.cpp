/*
 * blitter.cpp
 * E64
 *
 * Copyright © 2020-2023 elmerucr. All rights reserved.
 */

#include "blitter.hpp"
#include "rom.hpp"
#include "common.hpp"

/*
 * The alpha_blend lambda expression takes the current color (destination, which is
 * also the destination) and the color that must be blended (source). It
 * returns the value of the blend which, normally, will be written to the
 * destination.
 * At first, this function seemed to drag down total emulation speed. But, with
 * optimizations (minimum -O2) turned on, it is ok.
 *
 * The idea to use a function (and not a lookup table) comes from this website:
 * https://stackoverflow.com/questions/30849261/alpha-blending-using-table-lookup-is-not-as-fast-as-expected
 * Generally, lookup tables mess around with the cpu cache and don't speed up.
 *
 * In three steps a derivation (source is color to apply, destination
 * is the original color, a is alpha value):
 * (1) ((source * a) + (destination * (COLOR_MAX - a))) / COLOR_MAX
 * (2) ((source * a) - (destination * a) + (destination * COLOR_MAX)) / COLOR_MAX
 * (3) destination + (((source - destination) * a) / COLOR_MAX)
 *
 *
 * Update 2020-06-10, check:
 * https://stackoverflow.com/questions/12011081/alpha-blending-2-rgba-colors-in-c
 * Calculate inv_alpha, then makes use of a bit shift, no divisions anymore.
 * (1) isolate alpha value (0 - max) and add 1
 * (2) calculate inverse alpha by taking (max+1) - alpha
 * (3) calculate the new individual channels:
 *      new = (alpha * source) + (inv_alpha * dest)
 * (4) bitshift the result to the right (normalize)
 * Speeds up a little.
 *
 * Update 2021-03-04, adapted for ARGB444 format
 *
 * Update 2023-02-19, rewritten as lambda expression
 *
 */

auto alpha_blend = [](uint16_t *destination, uint16_t *source)
//static inline void alpha_blend(uint16_t *destination, uint16_t *source)
{
       uint16_t a_dest = (*destination & 0xf000) >> 12;
       uint16_t r_dest = (*destination & 0x0f00) >>  8;
       uint16_t g_dest = (*destination & 0x00f0) >>  4;
       uint16_t b_dest = (*destination & 0x000f);

       uint16_t a_src = ((*source & 0xf000) >> 12) + 1;
       uint16_t r_src =  (*source & 0x0f00) >> 8;
       uint16_t g_src =  (*source & 0x00f0) >> 4;
       uint16_t b_src =  (*source & 0x000f);
   
       uint16_t a_src_inv = 17 - a_src;
       
       //a_dest = (a_dest >= (a_src-1)) ? a_dest : (a_src-1);
       a_dest = (256-((16-(a_src-1))*(16-a_dest))) >> 4;
       r_dest = ((a_src * r_src) + (a_src_inv * r_dest)) >> 4;
       g_dest = ((a_src * g_src) + (a_src_inv * g_dest)) >> 4;
       b_dest = ((a_src * b_src) + (a_src_inv * b_dest)) >> 4;

       *destination = (a_dest << 12) | (r_dest << 8) | (g_dest << 4) | b_dest;
};

E64::blitter_ic::blitter_ic(uint16_t _pps, uint16_t _sl)
{
	pixels_per_scanline = _pps;
	scanlines = _sl;
	total_pixels = pixels_per_scanline * scanlines;
	
	current_blitter_width = pixels_per_scanline / 8;
	current_blitter_height = scanlines / 8;
	
	screen_size.x = 0;
	screen_size.y = 0;
	screen_size.w = 8 * current_blitter_width;
	screen_size.h = 8 * current_blitter_height;
	
	scanline_screen_size.x = 0;
	scanline_screen_size.y = 0;
	scanline_screen_size.w = screen_size.w;
	scanline_screen_size.h = 4 * screen_size.h;
	
	fb = new uint16_t[total_pixels];

	general_ram =               new uint8_t [GENERAL_RAM_ELEMENTS];			//   2mb
	tile_ram  =                 new uint8_t [TILE_RAM_ELEMENTS];			//   2mb
	tile_foreground_color_ram = new uint16_t[TILE_FOREGROUND_COLOR_RAM_ELEMENTS];	//   2mb
	tile_background_color_ram = new uint16_t[TILE_BACKGROUND_COLOR_RAM_ELEMENTS];	//   2mb
	pixel_ram =                 new uint16_t[PIXEL_RAM_ELEMENTS];			//   8mb +
											// ============
											//  16mb total

	/*
	 * Fill blit memory alternating 64 bytes 0x00 and 64 bytes 0xff
	 */
	for (int i=0; i < (0x100 * 0x10000); i++) {
		video_memory_write_8(i, i & 0b1000000 ? 0xff : 0x00);
	}

	/*
	 * Array of 256 blits
	 */
	blit = new struct blit_t[256];

	for (int i=0; i<256; i++) {
		blit[i].number = i;

		blit[i].background = false;
		blit[i].multicolor = false;
		blit[i].color_per_tile = false;
		blit[i].font_no = 0;

		blit[i].flags_1 = 0;
		blit[i].process_flags_1();

		blit[i].set_tile_width(1);
		blit[i].set_tile_height(1);
		blit[i].set_columns(1);
		blit[i].set_rows(1);

		blit[i].foreground_color = 0;
		blit[i].background_color = 0;
		blit[i].x_pos = 0;
		blit[i].y_pos = 0;
	}

	/*
	 * fonts as bitmaps
	 */
	cbm_font = new uint16_t[256 * 64];
	amiga_font = new uint16_t[256 * 128];

	/*
	 * Convert the character roms to 16bit argb4444 format
	 */
	uint16_t *dest = cbm_font;
	for (int i=0; i<CBM_CP437_FONT_ELEMENTS; i++) {
		uint8_t byte = cbm_cp437_font[i];
		uint8_t count = 8;
		while (count--) {
			*dest = (byte & 0b10000000) ? C64_GREY : 0x0000;
			dest++;
			byte = byte << 1;
		}
	}
	
	dest = amiga_font;
	uint8_t byte;
	for (int i=0; i<AMIGA_CP437_FONT_ELEMENTS; i++) {
		byte = amiga_cp437_font[i];
		uint8_t count = 8;
		while (count--) {
			*dest = (byte & 0b10000000) ? C64_GREY : 0x0000;
			dest++;
			byte = byte << 1;
		}
	}
	
	blitter_context_ptr_no = 0;
}

E64::blitter_ic::~blitter_ic()
{
	delete [] amiga_font;
	delete [] cbm_font;
	delete [] blit;
	delete [] pixel_ram;
	delete [] tile_background_color_ram;
	delete [] tile_foreground_color_ram;
	delete [] tile_ram;
	delete [] general_ram;
	delete [] fb;
}

void E64::blitter_ic::reset()
{
	head = 0;
	tail = 0;

	for (int i=0; i<total_pixels; i++) {
		fb[i] = 0xf222;
	}
}

uint32_t E64::blitter_ic::clear_framebuffer()
{
	uint32_t pixels{0};

	for (uint32_t y = 0; y < (8 * current_blitter_height); y++) {
		for (uint32_t x = 0; x < (8 * current_blitter_width); x++) {
			fb[(y * pixels_per_scanline) + x] = clear_color;
			pixels++;
		}
	}
//	uint32_t pixels = total_pixels;
//	while (pixels--) fb[pixels] = clear_color;
	
	return pixels;
}

uint32_t E64::blitter_ic::draw_horizontal_border()
{
	//uint32_t pixels = pixels_per_scanline * hor_border_size;
	uint32_t pixels{0};
	
	uint32_t constant = ((current_blitter_height * 8) - hor_border_size) * pixels_per_scanline;
	
//	// still doing too much
//	while (pixels--) {
//		alpha_blend(&fb[pixels], &hor_border_color);
//		alpha_blend(&fb[pixels + constant], &hor_border_color);
//		//alpha_blend(&fb[total_pixels - 1 - pixels], &hor_border_color);
//	}
	
	for (uint32_t y=0; y<hor_border_size; y++) {
		for (uint32_t x=0; x<(current_blitter_width*8); x++) {
			alpha_blend(&fb[(y*pixels_per_scanline)+x], &hor_border_color);
			alpha_blend(&fb[(y*pixels_per_scanline)+x+constant], &hor_border_color);
			
			pixels += 2;
		}
	}
	return pixels;
//	return 2 * pixels_per_scanline * hor_border_size;
}

uint32_t E64::blitter_ic::draw_vertical_border()
{
//	uint32_t pixels = scanlines * ver_border_size;
//	while (pixels--) {
//		uint32_t norm = (pixels % ver_border_size) + ((pixels / ver_border_size) * pixels_per_scanline);
//		//fb[norm] = fb[TOTAL_PIXELS - 1 - norm] = ver_border_color;
//		alpha_blend(&fb[norm], &ver_border_color);
//		alpha_blend(&fb[total_pixels - 1 - norm], &ver_border_color);
//	}
//
//	return 2 * scanlines * ver_border_size;
	
	uint32_t pixels{0};
	uint32_t constant = (8 * current_blitter_width) - ver_border_size;
	
	for (uint32_t y=0; y<(8*current_blitter_height); y++) {
		for (uint32_t x=0; x<ver_border_size; x++) {
			alpha_blend(&fb[(y*pixels_per_scanline)+x], &ver_border_color);
			alpha_blend(&fb[(y*pixels_per_scanline)+x+constant], &ver_border_color);
		}
	}
	
	return pixels;
}

uint32_t E64::blitter_ic::draw_blit(blit_t *blit)
{
	uint32_t counter{0};
	
	int16_t startx;
	int16_t endx;
	int16_t starty;
	int16_t endy;
	
	auto min = [](int16_t a, int16_t b) { return a < b ? a : b; };
	auto max = [](int16_t a, int16_t b) { return a > b ? a : b; };
	
	if (!blit->xy_flip) {
		startx = max(0, -blit->x_pos);
		endx = min(blit->width_on_screen, -blit->x_pos + (8*current_blitter_width));
		starty = max(0, -blit->y_pos);
		endy = min(blit->height_on_screen, -blit->y_pos + (8*current_blitter_height));
	} else {
		startx = max(0, -blit->y_pos);
		endx = min(blit->width_on_screen, -blit->y_pos + (8*current_blitter_height));
		starty = max(0, -blit->x_pos);
		endy = min(blit->height_on_screen, -blit->x_pos + (8*current_blitter_width));
	}
	
	if (blit->hor_flip) {
		int16_t temp_value = startx;
		startx = blit->width_on_screen - endx;
		endx   = blit->width_on_screen - temp_value;
	}
	
	if (blit->ver_flip) {
		int16_t temp_value = starty;
		starty = blit->height_on_screen - endy;
		endy   = blit->height_on_screen - temp_value;
	}
	
	uint16_t tile_x;
	uint16_t tile_x_reset = tile_x = startx / blit->tile_width_pixels_on_screen;
	uint16_t tile_y = starty / blit->tile_height_pixels_on_screen;
	
	uint16_t x_in_tile_on_screen;
	uint16_t x_in_tile_on_screen_reset = x_in_tile_on_screen = startx - (tile_x * blit->tile_width_pixels_on_screen);
	uint16_t y_in_tile_on_screen = starty - (tile_y * blit->tile_height_pixels_on_screen);
	
	uint16_t scrn_x;            // final screen x for the current pixel
	uint16_t scrn_y;            // final screen y for the current pixel
	
	for (int16_t y = starty; y < endy; y++) {
		for (int16_t x = startx; x < endx; x++) {
			if (blit->hor_flip) scrn_x = blit->width_on_screen - 1 - x; else scrn_x = x;
			if (blit->ver_flip) scrn_y = blit->height_on_screen - 1 - y; else scrn_y = y;

			if (blit->xy_flip) {
				uint16_t temp = scrn_y;
				scrn_y = scrn_x;
				scrn_x = temp;
			}

			scrn_x += blit->x_pos;
			scrn_y += blit->y_pos;

			tile_number = tile_x + (tile_y * blit->columns);

			tile_index = tile_ram[((blit->number << 13) + tile_number) & TILE_RAM_ELEMENTS_MASK];

			/*
			 * Replace foreground and background colors
			 * if color per tile.
			 */
			if (blit->color_per_tile) {
				blit->foreground_color = tile_foreground_color_ram[((blit->number << 12) + tile_number) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK];
				blit->background_color = tile_background_color_ram[((blit->number << 12) + tile_number) & TILE_BACKGROUND_COLOR_RAM_ELEMENTS_MASK];
			}
			
			pixel_in_tile = (x_in_tile_on_screen >> blit->double_width) + ((y_in_tile_on_screen >> blit->double_height) * blit->tile_width_pixels);

			/*
			 * Pick the right pixel from memory
			 */
			switch (blit->font_no) {
				case 0x01:
					source_color = cbm_font[((tile_index * blit->tile_width_pixels * blit->tile_height_pixels) | pixel_in_tile) & 0x3fff];
					break;
				case 0x02:
					source_color = amiga_font[((tile_index * blit->tile_width_pixels * blit->tile_height_pixels) | pixel_in_tile) & 0x7fff];
					break;
				default:
					source_color = pixel_ram[((blit->number << 14) + ((tile_index * blit->tile_width_pixels * blit->tile_height_pixels) + pixel_in_tile)) & PIXEL_RAM_ELEMENTS_MASK];
					break;
			}

			/*
			 * Check for multicolor or simple color
			 *
			 * If the source color has an alpha value of higher
			 * than 0x0 (pixel present), and not in multicolor mode,
			 * replace with foreground color.
			 *
			 * If there's no alpha value (no pixel), and we have
			 * background 'on', replace the color with background
			 * color.
			 *
			 * At this stage, we have already checked for color per
			 * tile, and if so, the value of foreground and respectively
			 * background color have been replaced accordingly.
			 */
			if (source_color & 0xf000) {
				if (!blit->multicolor) source_color = blit->foreground_color;
			} else {
				if (blit->background) source_color = blit->background_color;
			}

			/*
			 * Finally, call alpha_blend
			 */
			alpha_blend(&fb[scrn_x + (scrn_y * pixels_per_scanline)], &source_color);
			
			counter++;
			
			x_in_tile_on_screen++;
			if (x_in_tile_on_screen == blit->tile_width_pixels_on_screen) {
				x_in_tile_on_screen = 0;
				tile_x++;
				if (tile_x == blit->columns) {
					tile_x = 0;
				}
			}
		}
		
		/*
		 * Starting values tile_x and x_in_tile_on_screen back
		 * after starting new scanline
		 */
		tile_x = tile_x_reset;
		x_in_tile_on_screen = x_in_tile_on_screen_reset;
		
		y_in_tile_on_screen++;
		if (y_in_tile_on_screen == blit->tile_height_pixels_on_screen) {
			y_in_tile_on_screen = 0;
			tile_y++;
		}
	}
	
	return counter;
}

void E64::blitter_ic::set_clear_color(uint16_t color)
{
	clear_color = color;
}

void E64::blitter_ic::add_operation_clear_framebuffer()
{
	operations[head].type = CLEAR;
	// leaves blit structure for what it is
	head++;
}

void E64::blitter_ic::add_operation_draw_hor_border()
{
	operations[head].type = HOR_BORDER;
	// leaves blit structure for what it is
	head++;
}

void E64::blitter_ic::add_operation_draw_ver_border()
{
	operations[head].type = VER_BORDER;
	// leaves blit structure for what it is
	head++;
}

void E64::blitter_ic::add_operation_draw_blit(blit_t *blit)
{
	operations[head].type = BLIT;
	operations[head].blit = *blit;
	head++;
}

bool E64::blitter_ic::run_next_operation()
{
	if (head != tail) {
		switch (operations[tail].type) {
			case CLEAR:
				clear_framebuffer();
				break;
			case HOR_BORDER:
				draw_horizontal_border();
				break;
			case VER_BORDER:
				draw_vertical_border();
				break;
			case BLIT:
				draw_blit(&operations[tail].blit);
				break;
		}
		tail++;
		return true;
	} else {
		return false;
	}
}

uint8_t E64::blitter_ic::io_read_8(uint16_t address)
{
	switch (address & 0xff) {
		case BLITTER_SR:
//			if (pending_screenrefresh_irq) {
//				return 0b00000001;
//			} else {
//				return 0b00000000;
//			}
			return 0x00;
		case BLITTER_CR:
//			if (generate_screenrefresh_irq) {
//				return 0b00000001;
//			} else {
//				return 0b00000000;
//			}
			return 0x00;
		case BLITTER_CONTEXT_PTR_NO:
			return blitter_context_ptr_no;
		case BLITTER_CONTEXT_PTR_B0:
			return 0x00;
		case BLITTER_CONTEXT_PTR_B1:
			return 0x01;
		case BLITTER_CONTEXT_PTR_B2:
			return blitter_context_ptr_no;
		case BLITTER_CONTEXT_PTR_B3:
			return 0x00;
		case BLITTER_HOR_BORDER_SIZE:
			return 0x00;
		case BLITTER_HOR_BORDER_SIZE+1:
			return hor_border_size;
		case BLITTER_VER_BORDER_SIZE:
			return (ver_border_size & 0xff00) >> 8;
		case BLITTER_VER_BORDER_SIZE+1:
			return ver_border_size & 0xff;
		case BLITTER_HOR_BORDER_COLOR:
			return (hor_border_color & 0xff00) >> 8;
		case BLITTER_HOR_BORDER_COLOR+1:
			return hor_border_color & 0xff;
		case BLITTER_VER_BORDER_COLOR:
			return (ver_border_color & 0xff00) >> 8;
		case BLITTER_VER_BORDER_COLOR+1:
			return ver_border_color & 0xff;
		case BLITTER_CLEAR_COLOR:
			return (clear_color & 0xff00) >> 8;
		case BLITTER_CLEAR_COLOR+1:
			return clear_color & 0xff;
		case BLITTER_SCREEN_WIDTH:
			return current_blitter_width;
		case BLITTER_SCREEN_HEIGHT:
			return current_blitter_height;
		default:
			return 0;
	}
}

void E64::blitter_ic::io_write_8(uint16_t address, uint8_t byte)
{
	
	switch (address & 0xff) {
		case BLITTER_SR:
//			if ((byte & 0b00000001) && pending_screenrefresh_irq) {
//				pending_screenrefresh_irq = false;
//				if (exceptions_connected) {
//					TTL74LS148->release_line(interrupt_device_no);
//				}
//			}
			break;
		case BLITTER_CR:
//			if (byte & 0b00000001) {
//				generate_screenrefresh_irq = true;
//			} else {
//				generate_screenrefresh_irq = false;
//			}
			break;
		case BLITTER_OPERATION:
			if (byte & 0b00000001) add_operation_clear_framebuffer();
			if (byte & 0b00000010) add_operation_draw_hor_border();
			if (byte & 0b00000100) add_operation_draw_ver_border();
			break;
		case BLITTER_CONTEXT_PTR_NO:
			blitter_context_ptr_no = byte;
			break;
		case BLITTER_HOR_BORDER_SIZE+1:
			hor_border_size = byte;
			if (hor_border_size > (scanlines / 2)) {
				hor_border_size = (scanlines / 2);
			}
			break;
		case BLITTER_VER_BORDER_SIZE:
			ver_border_size = (ver_border_size & 0x00ff) | (byte << 8);
			if (ver_border_size > (pixels_per_scanline / 2)) {
				ver_border_size = (pixels_per_scanline / 2);
			}
			break;
		case BLITTER_VER_BORDER_SIZE+1:
			ver_border_size = (ver_border_size & 0xff00) | byte;
			if (ver_border_size > (pixels_per_scanline / 2)) {
				ver_border_size = (pixels_per_scanline / 2);
			}
			break;
		case BLITTER_HOR_BORDER_COLOR:
			hor_border_color = (hor_border_color & 0x00ff) | (byte << 8);
			break;
		case BLITTER_HOR_BORDER_COLOR+1:
			hor_border_color = (hor_border_color & 0xff00) | byte;
			break;
		case BLITTER_VER_BORDER_COLOR:
			ver_border_color = (ver_border_color & 0x00ff) | (byte << 8);
			break;
		case BLITTER_VER_BORDER_COLOR+1:
			ver_border_color = (ver_border_color & 0xff00) | byte;
			break;
		case BLITTER_CLEAR_COLOR:
			clear_color = (clear_color & 0x00ff) | (byte << 8);
			break;
		case BLITTER_CLEAR_COLOR+1:
			clear_color = (clear_color & 0xff00) | byte;
			break;
		case BLITTER_SCREEN_WIDTH:
			set_current_blitter_width(byte);
			break;
		case BLITTER_SCREEN_HEIGHT:
			set_current_blitter_height(byte);
			break;
		default:
			break;
	}
}

uint8_t E64::blitter_ic::io_blit_context_read_8(uint8_t blit_no, uint8_t address)
{
	switch (address) {
		case BLIT_SR:
			// reading status register tells something about cursor
			uint8_t temp_byte;
			temp_byte =
				((blit[blit_no].cursor_position == 0) ? 0b10000000 : 0b00000000) |
				((blit[blit_no].cursor_position % blit[blit_no].columns == 0) ? 0b01000000 : 0b00000000) |
				(blit[blit_no].cursor_big_move ? 0b00100000 : 0b00000000);
			return temp_byte;
		case BLIT_CR:
			// maybe not used
			return 0;
		case BLIT_FLAGS_0:
			return	(blit[blit_no].get_background()     ? 0x02 : 0x00) |
				(blit[blit_no].get_multicolor()     ? 0x04 : 0x00) |
				(blit[blit_no].get_color_per_tile() ? 0x08 : 0x00) |
				(blit[blit_no].get_font()              << 4      ) ;
		case BLIT_FLAGS_1:
			return blit[blit_no].flags_1;
		case BLIT_TILE_WIDTH:
			return blit[blit_no].get_tile_width();
		case BLIT_TILE_HEIGHT:
			return blit[blit_no].get_tile_height();
		case BLIT_COLUMNS:
			return blit[blit_no].get_columns();
		case BLIT_ROWS:
			return blit[blit_no].get_rows();
		case BLIT_FG_COLOR_MSB:
			return ((blit[blit_no].foreground_color) & 0xff00) >> 8;
		case BLIT_FG_COLOR_LSB:
			return (blit[blit_no].foreground_color) & 0x00ff;
		case BLIT_BG_COLOR_MSB:
			return ((blit[blit_no].background_color) & 0xff00) >> 8;
		case BLIT_BG_COLOR_LSB:
			return (blit[blit_no].background_color) & 0x00ff;
		case BLIT_XPOS_MSB:
			return (((uint16_t)blit[blit_no].x_pos) & 0xff00) >> 8;
		case BLIT_XPOS_LSB:
			return (((uint16_t)blit[blit_no].x_pos) & 0x00ff);
		case BLIT_YPOS_MSB:
			return (((uint16_t)blit[blit_no].y_pos) & 0xff00) >> 8;
		case BLIT_YPOS_LSB:
			return (((uint16_t)blit[blit_no].y_pos) & 0x00ff);
		case BLIT_NO_OF_TILES_MSB:
			return (blit[blit_no].tiles & 0xff00) >> 8;
		case BLIT_NO_OF_TILES_LSB:
			return blit[blit_no].tiles & 0xff;
		case BLIT_CURSOR_POS_MSB:
			return (blit[blit_no].cursor_position & 0xff00) >> 8;
		case BLIT_CURSOR_POS_LSB:
			return blit[blit_no].cursor_position & 0xff;
		case BLIT_CURSOR_COLUMN:
			return blit[blit_no].terminal_get_current_column();
		case BLIT_CURSOR_ROW:
			return blit[blit_no].terminal_get_current_row();
		case BLIT_CURSOR_BLINK_SPEED:
			// return cursor blink interval;
			return blit[blit_no].cursor_interval;
		case BLIT_CURSOR_CHAR:
			// character at cursor pos
			return tile_ram[((blit_no << 13) + blit[blit_no].cursor_position) & TILE_RAM_ELEMENTS_MASK];
		case BLIT_CURSOR_FG_COLOR_MSB:
			// foreground color at cursor msb
			return (tile_foreground_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] & 0xff00) >> 8;
		case BLIT_CURSOR_FG_COLOR_LSB:
			// foreground color at cursor lsb
			return tile_foreground_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] & 0x00ff;
		case BLIT_CURSOR_BG_COLOR_MSB:
			// background color at cursor msb
			return (tile_background_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] & 0xff00) >> 8;
		case BLIT_CURSOR_BG_COLOR_LSB:
			// background color at cursor lsb
			return tile_background_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] & 0x00ff;
		case BLIT_TILE_RAM_PTR_B0:
			return 0x00;
		case BLIT_TILE_RAM_PTR_B1:
			return ((0x200000 + ((0x200000 >> 8) * blit_no)) >> 16) & 0xff;
		case BLIT_TILE_RAM_PTR_B2:
			return ((0x200000 + ((0x200000 >> 8) * blit_no)) >>  8) & 0xff;
		case BLIT_TILE_RAM_PTR_B3:
			return ((0x200000 + ((0x200000 >> 8) * blit_no)) >>  0) & 0xff;
		case BLIT_FG_COLOR_RAM_PTR_B0:
			return 0x00;
		case BLIT_FG_COLOR_RAM_PTR_B1:
			return ((0x400000 + ((0x200000 >> 8) * blit_no)) >> 16) & 0xff;
		case BLIT_FG_COLOR_RAM_PTR_B2:
			return ((0x400000 + ((0x200000 >> 8) * blit_no)) >>  8) & 0xff;
		case BLIT_FG_COLOR_RAM_PTR_B3:
			return ((0x400000 + ((0x200000 >> 8) * blit_no)) >>  0) & 0xff;
		case BLIT_BG_COLOR_RAM_PTR_B0:
			return 0x00;
		case BLIT_BG_COLOR_RAM_PTR_B1:
			return ((0x600000 + ((0x200000 >> 8) * blit_no)) >> 16) & 0xff;
		case BLIT_BG_COLOR_RAM_PTR_B2:
			return ((0x600000 + ((0x200000 >> 8) * blit_no)) >>  8) & 0xff;
		case BLIT_BG_COLOR_RAM_PTR_B3:
			return ((0x600000 + ((0x200000 >> 8) * blit_no)) >>  0) & 0xff;
		case BLIT_PIXEL_RAM_PTR_B0:
			return 0x00;
		case BLIT_PIXEL_RAM_PTR_B1:
			return ((0x800000 + ((0x800000 >> 8) * blit_no)) >> 16) & 0xff;
		case BLIT_PIXEL_RAM_PTR_B2:
			return ((0x800000 + ((0x800000 >> 8) * blit_no)) >>  8) & 0xff;
		case BLIT_PIXEL_RAM_PTR_B3:
			return ((0x800000 + ((0x800000 >> 8) * blit_no)) >>  0) & 0xff;
		default:
			return 0;
	}
}

void E64::blitter_ic::io_blit_context_write_8(uint8_t blit_no, uint8_t address, uint8_t byte)
{
	uint16_t temp_word;

	switch (address) {
		case BLIT_SR:
			// unused so far...
			break;
		case BLIT_CR:
			switch (byte) {
				case 0b00000001:
					add_operation_draw_blit(&blit[blit_no]);
					break;
				case 0b10000000:
					terminal_cursor_decrease(blit_no);
					break;
				case 0b10000001:
					terminal_cursor_increase(blit_no);
					break;
				case 0b11000000:
					terminal_activate_cursor(blit_no);
					break;
				case 0b11000001:
					terminal_deactivate_cursor(blit_no);
					break;
				case 0b11000010:
					terminal_process_cursor_state(blit_no);
					break;
				default:
					break;
			}
			break;
		case BLIT_FLAGS_0:
			blit[blit_no].set_background(byte & 0x02 ? true : false);
			blit[blit_no].set_multicolor(byte & 0x04 ? true : false);
			blit[blit_no].set_color_per_tile(byte & 0x08 ? true : false);
			blit[blit_no].set_font((byte & 0xf0) >> 4);
			break;
		case BLIT_FLAGS_1:
			blit[blit_no].flags_1 = byte;
			blit[blit_no].process_flags_1();
			blit[blit_no].calculate_dimensions();
			break;
		case BLIT_TILE_WIDTH:
			blit[blit_no].set_tile_width(byte);
			break;
		case BLIT_TILE_HEIGHT:
			blit[blit_no].set_tile_height(byte);
			break;
		case BLIT_COLUMNS:
			blit[blit_no].set_columns(byte);
			break;
		case BLIT_ROWS:
			blit[blit_no].set_rows(byte);
			break;
		case BLIT_FG_COLOR_MSB:
			temp_word = (blit[blit_no].foreground_color) & 0x00ff;
			blit[blit_no].foreground_color = temp_word | (byte << 8);
			break;
		case BLIT_FG_COLOR_LSB:
			temp_word = (blit[blit_no].foreground_color) & 0xff00;
			blit[blit_no].foreground_color = temp_word | byte;
			break;
		case BLIT_BG_COLOR_MSB:
			temp_word = (blit[blit_no].background_color) & 0x00ff;
			blit[blit_no].background_color = temp_word | (byte << 8);
			break;
		case BLIT_BG_COLOR_LSB:
			temp_word = (blit[blit_no].background_color) & 0xff00;
			blit[blit_no].background_color = temp_word | byte;
			break;
		case BLIT_XPOS_MSB:
			temp_word = (blit[blit_no].x_pos) & 0x00ff;
			blit[blit_no].x_pos = temp_word | (byte << 8);
			break;
		case BLIT_XPOS_LSB:
			temp_word = (blit[blit_no].x_pos) & 0xff00;
			blit[blit_no].x_pos = temp_word | byte;
			break;
		case BLIT_YPOS_MSB:
			temp_word = (blit[blit_no].y_pos) & 0x00ff;
			blit[blit_no].y_pos = temp_word | (byte << 8);
			break;
		case BLIT_YPOS_LSB:
			temp_word = (blit[blit_no].y_pos) & 0xff00;
			blit[blit_no].y_pos = temp_word | byte;
			break;
		case BLIT_CURSOR_POS_MSB:
			blit[blit_no].cursor_position =
			(blit[blit_no].cursor_position & 0x00ff) | (byte << 8);
			break;
		case BLIT_CURSOR_POS_LSB:
			blit[blit_no].cursor_position =
			(blit[blit_no].cursor_position & 0xff00) | byte;
			break;
		case BLIT_CURSOR_COLUMN:
			byte %= blit[blit_no].get_columns();
			blit[blit_no].cursor_position =
				(blit[blit_no].terminal_get_current_row() * blit[blit_no].get_columns()) +
				byte;
			break;
		case BLIT_CURSOR_ROW:
			byte %= blit[blit_no].get_rows();
			blit[blit_no].cursor_position =
				blit[blit_no].terminal_get_current_column() +
				(blit[blit_no].get_columns() * byte);
			break;
		case BLIT_CURSOR_BLINK_SPEED:
			blit[blit_no].cursor_interval = byte;
			break;
		case BLIT_CURSOR_CHAR:
			tile_ram[((blit_no << 13) + blit[blit_no].cursor_position) & TILE_RAM_ELEMENTS_MASK] = byte;
			break;
		case BLIT_CURSOR_FG_COLOR_MSB:
			tile_foreground_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] =
			(tile_foreground_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] & 0x00ff) | (byte << 8);
			break;
		case BLIT_CURSOR_FG_COLOR_LSB:
			tile_foreground_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] =
			(tile_foreground_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] & 0xff00) | byte;
			break;
		case BLIT_CURSOR_BG_COLOR_MSB:
			tile_background_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] =
			(tile_background_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] & 0x00ff) | (byte << 8);
			break;
		case BLIT_CURSOR_BG_COLOR_LSB:
			tile_background_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] =
			(tile_background_color_ram[((blit_no << 12) + blit[blit_no].cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] & 0xff00) | byte;
			break;
		default:
			break;
	}
}
