/*
 * blitter.hpp
 * E64
 *
 * Copyright © 2020-2023 elmerucr. All rights reserved.
 */

/*
 * Blitter is able to copy data very fast from video memory location to
 * the framebuffer. Copy operations run independently and can
 * be added to a FIFO linked list (blitter internally).
 */

/*
 * I/O addresses blitter_ic
 */
#define BLITTER_SR			0x00	// blitter status register (pending irq's)
#define BLITTER_CR			0x01	// blitter control register (irq activation)
#define BLITTER_OPERATION		0x02	// clear buffer, border draws, blits
#define BLITTER_CONTEXT_PTR_NO		0x03	// to which blit is context ptr pointing to?
#define BLITTER_CONTEXT_PTR_B0		0x04	// most sign byte
#define BLITTER_CONTEXT_PTR_B1		0x05	// byte
#define BLITTER_CONTEXT_PTR_B2		0x06	// byte
#define BLITTER_CONTEXT_PTR_B3		0x07	// least sign byte
#define BLITTER_HOR_BORDER_SIZE		0x08	// horizontal border size
#define BLITTER_VER_BORDER_SIZE		0x0a	// vertical border size
#define BLITTER_HOR_BORDER_COLOR	0x0c	// horizontal border color
#define BLITTER_VER_BORDER_COLOR	0x0e	// vertical border color
#define BLITTER_CLEAR_COLOR		0x10	// clear color (background color)
#define BLITTER_SCREEN_WIDTH		0x12	// 1 byte (high and low nibble)
#define BLITTER_SCREEN_HEIGHT		0x13

/*
 * Individual blit context registers
 */
#define BLIT_SR				0x00	// SR probably not used here
#define BLIT_CR				0x01	// control register
#define BLIT_FLAGS_0			0x02	// flags 0
#define BLIT_FLAGS_1			0x03	// flags 1
#define BLIT_TILE_WIDTH			0x04
#define BLIT_TILE_HEIGHT		0x05
#define BLIT_COLUMNS			0x06
#define BLIT_ROWS			0x07

#define BLIT_FG_COLOR_MSB		0x08	// current foreground color hb
#define BLIT_FG_COLOR_LSB		0x09	// current foreground color lb

#define BLIT_BG_COLOR_MSB		0x0a	// current background color hb
#define BLIT_BG_COLOR_LSB		0x0b	// current background color hb

#define BLIT_XPOS_MSB			0x0c	// xpos hb
#define BLIT_XPOS_LSB			0x0d	// xpos lb

#define BLIT_YPOS_MSB			0x0e	// ypos hb
#define BLIT_YPOS_LSB			0x0f	// ypos lb

#define BLIT_NO_OF_TILES_MSB		0x10
#define BLIT_NO_OF_TILES_LSB		0x11

#define BLIT_CURSOR_POS_MSB		0x12
#define BLIT_CURSOR_POS_LSB		0x13

#define BLIT_CURSOR_COLUMN		0x14
#define BLIT_CURSOR_ROW			0x15

#define BLIT_CURSOR_BLINK_SPEED		0x16
#define BLIT_CURSOR_CHAR		0x17	// tile at cursor, read/write

#define BLIT_CURSOR_FG_COLOR_MSB	0x18
#define BLIT_CURSOR_FG_COLOR_LSB	0x19

#define BLIT_CURSOR_BG_COLOR_MSB	0x1a
#define BLIT_CURSOR_BG_COLOR_LSB	0x1b

#define BLIT_TILE_RAM_PTR_B0		0x20
#define BLIT_TILE_RAM_PTR_B1		0x21
#define BLIT_TILE_RAM_PTR_B2		0x22
#define BLIT_TILE_RAM_PTR_B3		0x23

#define BLIT_FG_COLOR_RAM_PTR_B0	0x24
#define BLIT_FG_COLOR_RAM_PTR_B1	0x25
#define BLIT_FG_COLOR_RAM_PTR_B2	0x26
#define BLIT_FG_COLOR_RAM_PTR_B3	0x27

#define BLIT_BG_COLOR_RAM_PTR_B0	0x28
#define BLIT_BG_COLOR_RAM_PTR_B1	0x29
#define BLIT_BG_COLOR_RAM_PTR_B2	0x2a
#define BLIT_BG_COLOR_RAM_PTR_B3	0x2b

#define BLIT_PIXEL_RAM_PTR_B0		0x2c
#define BLIT_PIXEL_RAM_PTR_B1		0x2d
#define BLIT_PIXEL_RAM_PTR_B2		0x2e
#define BLIT_PIXEL_RAM_PTR_B3		0x2f


#ifndef BLITTER_HPP
#define BLITTER_HPP

#define	GENERAL_RAM_ELEMENTS			0x600000	// 1 byte,   6mb, starts @ $0000000 to $05fffff
#define TILE_RAM_ELEMENTS			0x200000	// 1 byte,   2mb, starts @ $0600000 to $07fffff, steps of $ 2000
#define TILE_FOREGROUND_COLOR_RAM_ELEMENTS	0x200000	// 2 bytes,  4mb, starts @ $0800000 to $0bfffff, steps of $ 4000
#define TILE_BACKGROUND_COLOR_RAM_ELEMENTS	0x200000	// 2 bytes,  4mb, starts @ $0c00000 to $0ffffff, steps of $ 4000
#define PIXEL_RAM_ELEMENTS			0x800000	// 2 bytes, 16mb, starts @ $1000000 to $1ffffff, steps of $10000

#define	GENERAL_RAM_ELEMENTS_MASK		0x7fffff
#define TILE_RAM_ELEMENTS_MASK			(TILE_RAM_ELEMENTS-1)
#define TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK	(TILE_FOREGROUND_COLOR_RAM_ELEMENTS-1)
#define TILE_BACKGROUND_COLOR_RAM_ELEMENTS_MASK	(TILE_BACKGROUND_COLOR_RAM_ELEMENTS-1)
#define PIXEL_RAM_ELEMENTS_MASK			(PIXEL_RAM_ELEMENTS-1)

#include "blit.hpp"
#include <SDL2/SDL.h>

namespace E64
{

enum operation_type {
	CLEAR,
	HOR_BORDER,
	VER_BORDER,
	BLIT
};

struct operation {
	enum operation_type type;
	blit_t blit;
};

class blitter_ic {
private:
	uint16_t pixels_per_scanline, scanlines;
	uint32_t total_pixels;
	
	uint8_t current_blitter_width;
	uint8_t current_blitter_height;

	/*
	 * Video ram, different components
	 */
	uint8_t  *general_ram;			// 2mb
	uint8_t  *tile_ram;			// 2mb
	uint16_t *tile_foreground_color_ram;	// 2mb
	uint16_t *tile_background_color_ram;	// 2mb
	uint16_t *pixel_ram;			// 8mb

	/*
	 * Specific for border
	 */
	uint16_t hor_border_size;
	uint16_t ver_border_size;

	uint16_t hor_border_color;
	uint16_t ver_border_color;

	/*
	 * Specific for clearing framebuffer / background color
	 */
	uint16_t clear_color;

	/*
	 * Blitter contexts visible from 0x20-0xff
	 */
	uint8_t  blitter_context_0;
	uint8_t  blitter_context_1;
	uint8_t  blitter_context_2;
	uint8_t  blitter_context_3;
	uint8_t  blitter_context_4;
	uint8_t  blitter_context_5;
	uint8_t  blitter_context_6;

	/*
	 * Circular buffer containing max 65536 operations. If more
	 * operations would be written (unlikely) and unfinished, buffer
	 * will overwrite itself.
	 */
	struct operation operations[65536];
	uint16_t head;
	uint16_t tail;

	/*
	 * Finite state machine
	 */

	uint16_t tile_number;
	uint8_t  tile_index;
	uint16_t current_background_color;
	uint32_t pixel_in_tile;

	uint16_t source_color;
	
	uint8_t blitter_context_ptr_no;
public:
	blitter_ic(uint16_t _pps, uint16_t _sl);
	~blitter_ic();
	
	uint8_t interrupt_device_no;
	
	SDL_Rect screen_size;
	SDL_Rect scanline_screen_size;

	// framebuffer pointer
	uint16_t *fb;
	
	/*
	 * Pointers to unpacked fonts
	 */
	uint16_t *cbm_font;
	uint16_t *amiga_font;
	uint16_t *ibm_8x8_font;
	uint16_t *ibm_8x16_font;

	/*
	 * This method is called to notify blitter that screen was
	 * just refreshed, so it has the possibility to raise an
	 * interrupt (if this is enabled).
	 */
	void notify_screen_refreshed();

	/*
	 * io access to blitter_ic (mapped to a specific page in
	 * cpu memory)
	 */
	uint8_t	io_read_8 (uint16_t address);
	void    io_write_8(uint16_t address, uint8_t byte);

	uint8_t io_blit_context_read_8 (uint8_t blit, uint8_t address);
	void    io_blit_context_write_8(uint8_t blit, uint8_t address, uint8_t byte);

	/*
	 * io access to blit contexts (8k)
	 */
	inline uint8_t io_blit_contexts_read_8(uint16_t address)
	{
		return io_blit_context_read_8((address & 0xff00) >> 8, address & 0x00ff);
	}

	inline void io_blit_contexts_write_8(uint16_t address, uint8_t byte)
	{
		io_blit_context_write_8((address & 0xff00) >> 8, address & 0x00ff, byte);
	}

	inline uint8_t video_memory_read_8(uint32_t address)
	{
		// 21 bitshifts to the right: 2mb steps
		switch ((address & 0x01ffffff) >> 21) {
			case 0b000:
			case 0b001:
			case 0b010:
				/*
				 * General ram, 6mb
				 */
				return general_ram[address & GENERAL_RAM_ELEMENTS_MASK];
			case 0b011:
				return tile_ram[address & TILE_RAM_ELEMENTS_MASK];
			case 0b100:
			case 0b101:
				if (address & 0b1) {
					return tile_foreground_color_ram[(address & 0x3fffff) >> 1] & 0x00ff;
				} else {
					return (tile_foreground_color_ram[(address & 0x3fffff) >> 1] & 0xff00) >> 8;
				}
			case 0b110:
			case 0b111:
				if (address & 0b1) {
					return tile_background_color_ram[(address & 0x3fffff) >> 1] & 0x00ff;
				} else {
					return (tile_background_color_ram[(address & 0x3fffff) >> 1] & 0xff00) >> 8;
				}
			default:
				/*
				 * Pixel ram
				 */
				if (address & 0b1) {
					return pixel_ram[(address & 0xffffff) >> 1] & 0x00ff;
				} else {
					return (pixel_ram[(address & 0xffffff) >> 1] & 0xff00) >> 8;
				}
		}
		return 0;
	}

	inline void video_memory_write_8(uint32_t address, uint8_t value)
	{
		switch ((address & 0x01ffffff) >> 21) {
			case 0b000:
			case 0b001:
			case 0b010:
				general_ram[address & GENERAL_RAM_ELEMENTS_MASK] = value;
				break;
			case 0b011:
				tile_ram[address & TILE_RAM_ELEMENTS_MASK] = value;
				break;
			case 0b100:
			case 0b101:
				if (address & 0b1) {
					tile_foreground_color_ram[(address & 0x3fffff) >> 1] = (tile_foreground_color_ram[(address & 0x3fffff) >> 1] & 0xff00) | value;
				} else {
					tile_foreground_color_ram[(address & 0x3fffff) >> 1] = (tile_foreground_color_ram[(address & 0x3fffff) >> 1] & 0x00ff) | (value << 8);
				}
				break;
			case 0b110:
			case 0b111:
				if (address & 0b1) {
					tile_background_color_ram[(address & 0x3fffff) >> 1] = (tile_background_color_ram[(address & 0x3fffff) >> 1] & 0xff00) | value;
				} else {
					tile_background_color_ram[(address & 0x3fffff) >> 1] = (tile_background_color_ram[(address & 0x3fffff) >> 1] & 0x00ff) | (value << 8);
				}
				break;
			default:
				if (address & 0b1) {
					pixel_ram[(address & 0xffffff) >> 1] = (pixel_ram[(address & 0xffffff) >> 1] & 0xff00) | value;
				} else {
					pixel_ram[(address & 0xffffff) >> 1] = (pixel_ram[(address & 0xffffff) >> 1] & 0x00ff) | (value << 8);
				}
				break;

		}
	}

	void reset();

	struct blit_t *blit;

	void set_clear_color(uint16_t color);
	void set_hor_border_color(uint16_t color) { hor_border_color = color; }
	void set_hor_border_size(uint16_t size ) { hor_border_size = size; }
	void set_ver_border_color(uint16_t color) { ver_border_color = color; }
	void set_ver_border_size(uint16_t size ) { ver_border_size = size; }
	

	void add_operation_clear_framebuffer();
	void add_operation_draw_hor_border();
	void add_operation_draw_ver_border();
	void add_operation_draw_blit(blit_t *blit);
	
	bool run_next_operation();
	
	uint32_t clear_framebuffer();
	uint32_t draw_horizontal_border();
	uint32_t draw_vertical_border();
	uint32_t draw_blit(blit_t *blit);

	void set_pixel(uint8_t number, uint32_t pixel_no, uint16_t color);
	uint16_t get_pixel(uint8_t number, uint32_t pixel_no);

	/*
	 * Terminal interface - Init needs work, flags 0 and flags 1 are sort
	 * of artificial ways to set properties.
	 */
	void     terminal_set_tile(uint8_t number, uint16_t cursor_position, char symbol);
	void     terminal_set_tile_fg_color(uint8_t number, uint16_t cursor_position, uint16_t color);
	void     terminal_set_tile_bg_color(uint8_t number, uint16_t cursor_position, uint16_t color);
	uint8_t  terminal_get_tile(uint8_t number, uint16_t cursor_position);
	uint16_t terminal_get_tile_fg_color(uint8_t number, uint16_t cursor_position);
	uint16_t terminal_get_tile_bg_color(uint8_t number, uint16_t cursor_position);

	void terminal_init(uint8_t number,
			   uint8_t flags0,
			   uint8_t flags1,
			   uint8_t tw,
			   uint8_t th,
			   uint8_t columns,
			   uint8_t rows,
			   uint16_t foreground_color,
			   uint16_t background_color);
	void terminal_clear(uint8_t number);
	void terminal_putsymbol_at_cursor(uint8_t number, char symbol);
	void terminal_putsymbol(uint8_t number, char symbol);
	int  terminal_putchar(uint8_t number, int character);
	int  terminal_puts(uint8_t number, const char *text);
	int  terminal_printf(uint8_t no, const char *format, ...);
	void terminal_prompt(uint8_t number);
	void terminal_activate_cursor(uint8_t number);
	void terminal_deactivate_cursor(uint8_t no);
	void terminal_cursor_decrease(uint8_t no);		// moves to left, wraps around
	void terminal_cursor_increase(uint8_t no);		// moves to the right, and wraps around
	void terminal_cursor_left(uint8_t no);
	void terminal_cursor_right(uint8_t no);
	void terminal_cursor_up(uint8_t no);
	void terminal_cursor_down(uint8_t no);
	void terminal_backspace(uint8_t no);
	void terminal_add_top_row(uint8_t no);
	void terminal_add_bottom_row(uint8_t no);

	void terminal_process_cursor_state(uint8_t no);
	char *terminal_enter_command(uint8_t no);
	enum E64::terminal_output_type terminal_check_output(uint8_t no, bool top_down, uint32_t *address);
	
//	inline bool terminal_bottom_row_added(uint8_t no) {
//		bool result = blit[no].bottom_row_added;
//		if (result) blit[no].bottom_row_added = false;
//		return result;
//	}
	
	inline void set_current_blitter_width(uint8_t w) {
		if (w < (pixels_per_scanline / 8)) {
			current_blitter_width = w;
		} else {
			current_blitter_width = pixels_per_scanline / 8;
		}
		if (current_blitter_width == 0) current_blitter_width = 1;
		screen_size.w = 8 * current_blitter_width;
	}

	inline void set_current_blitter_height(uint8_t h) {
		if (h < (scanlines / 8)) {
			current_blitter_height = h;
		} else {
			current_blitter_height = scanlines / 8;
		}
		if (current_blitter_height == 0) current_blitter_height = 1;
		screen_size.h = 8 * current_blitter_height;
		scanline_screen_size.h = 4 * screen_size.h;
	}
	
	inline uint8_t get_current_blitter_width() { return current_blitter_width; }
	inline uint8_t get_current_blitter_height() { return current_blitter_height; }
};

}

#endif
