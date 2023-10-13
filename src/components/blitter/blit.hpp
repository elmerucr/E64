/*
 * blit.hpp
 * E64
 *
 * Copyright © 2022-2023 elmerucr. All rights reserved.
 */

#ifndef BLIT_HPP
#define BLIT_HPP

#define COMMAND_BUFFER_SIZE 63+(3*64)

#include <cstdint>

namespace E64
{

enum terminal_output_type {
	NOTHING,
	ASCII,
	MONITOR_WORD
};

/*
 * The next class is a surface blit. It is also used for terminal type
 * operations.
 */

class blit_t {
private:
	uint8_t  columns;
	uint16_t rows;
	uint16_t tiles;
	
	uint16_t tile_width;		// Value [1, 128] (each unit is 8 pixels)
	uint16_t tile_height;		// Value [1, 64] (each unit is 8 pixels)
	
	uint16_t tile_width_pixels;
	uint16_t tile_height_pixels;
	
	uint16_t tile_width_pixels_on_screen;
	uint16_t tile_height_pixels_on_screen;
	
	uint16_t width;			// width of the blit source in pixels
	uint16_t height;		// height of the blit source in pixels
	
	uint16_t width_on_screen;	// width of the final bit on screen in pixels (might be double)
	uint16_t height_on_screen;	// height of the final bit on screen in pixels (might be double)
	uint32_t total_pixels;
	
	uint16_t foreground_color;	// for both single color AND current terminal color
	uint16_t background_color;	// for both single color AND current terminal color
	
	int16_t x_pos;
	int16_t y_pos;
	
	/*
	 * Internal flags_0
	 */
	bool	background;	// bit 1
	bool	multicolor;	// bit 2
	bool	color_per_tile;	// bit 3
	uint8_t	font_no;	// bits 4-7
	
	/*
	 * Internal flags_1 things
	 */
	uint8_t double_width;	// bit 0
	uint8_t double_height;	// bit 1
	bool hor_flip;		// bit 4
	bool ver_flip;		// bit 5
	bool xy_flip;		// bit 6
	
//	/*
//	 * Terminal related flag
//	 */
//	bool bottom_row_added{false};
	
	friend class blitter_ic;
public:
	/*
	 * TODO: get rid of number here, only used for terminal type stuff in blitter...
	 */
	uint8_t number;

	/*
	 * Properties related to flags_0 (as encoded inside machine)
	 *
	 * 7 6 5 4 3 2 1 0
	 * | | | | | | |
	 * | | | | | | |
	 * | | | | | | +---- Background (0 = off, 1 = on)
	 * | | | | | +------ Multicolor (0 = off = single color, 1 = on)
	 * | | | | +-------- Color per tile (0 = off, 1 = on)
	 * +-+-+-+---------- Font no (0 = off, 1 = cbm, 2 = ibm, 3 = amiga, ...)
	 *
	 * bits 0 and 4-6: Reserved
	 */
	inline void set_background(bool state) { background = state; }		// bit 1
	inline void set_multicolor(bool state) { multicolor = state; }		// bit 2
	inline void set_color_per_tile(bool state) { color_per_tile = state; }	// bit 3
	inline void set_font(uint8_t no) { font_no = no & 0x0f; }			// bits 4-7
	
	inline bool get_background() { return background; }
	inline bool get_multicolor() { return multicolor; }
	inline bool get_color_per_tile() { return color_per_tile; }
	inline bool get_font() { return font_no; }

	/*
	 * Properties related to flags_1 (as encoded inside machine)
	 * Size, flips and rotations
	 *
	 * 7 6 5 4 3 2 1 0
	 *   | | |     | |
	 *   | | |     | +-- Double width (0 = off, 1 = on)
	 *   | | |     +---- Double height (0 = off, 1 = on)
	 *   | | +---------- Horizontal flip (0 = off, 1 = on)
	 *   | +------------ Vertical flip (0 = off, 1 = on)
	 *   +-------------- XY flip (0 = off, 1 = on)
	 *
	 * bits 2,3 and 7: Reserved
	 */
	uint8_t flags_1;
	
	/*
	 * Max tile size is 16 x 32 (units of 8px)
	 * Totally 16 * 32 * 8 * 8 * 2 = 65536 bytes = 64kb
	 * 64kb is max pix mem per blit (to be implemented 20231003) 32->64
	 */
	inline void set_tile_width(uint8_t tw)
	{
		if (tw == 0) tw = 1;
		if (tw > 16) tw = 16;
		
		tile_width = tw;
		
		calculate_dimensions();
	}
	
	inline void set_tile_height(uint8_t th)
	{
		if (th == 0) th = 1;
		if (th > 32) th = 32;
		
		tile_height = th;
		
		calculate_dimensions();
	}
	
	inline void set_columns(uint8_t c)
	{
		if (c == 0) c = 1;
		if (c > 128) c = 128;
		
		columns = c;
		
		calculate_dimensions();
	}
	
	inline void set_rows(uint8_t r)
	{
		if (r == 0) r = 1;
		if (r > 64) r = 64;
		
		rows = r;
		
		calculate_dimensions();
	}

	inline void calculate_dimensions()
	{
		tile_width_pixels = tile_width << 3;
		tile_height_pixels = tile_height << 3;
		
		tile_width_pixels_on_screen = tile_width_pixels << double_width;
		tile_height_pixels_on_screen = tile_height_pixels << double_height;
		
		// TODO: change to 128 x 64???
		if ((tile_width * columns) > 128) columns = 128 / tile_width;
		if ((tile_height * rows) > 64) rows = 64 / tile_height;
		
		width = tile_width_pixels * columns;
		height = tile_height_pixels * rows;
		
		width_on_screen = width << (double_width ? 1 : 0);
		height_on_screen = height << (double_height ? 1 : 0);
		total_pixels = width_on_screen * height_on_screen;
		
		tiles = columns * rows;
	}
	
	inline void process_flags_1()
	{
		flags_1 &= 0b11110011;
		
		/*
		 * Deal with the stretching flags, flips and rotations
		 */
		if (flags_1 & 0b00000001) {  double_width = 1;    } else {  double_width = 0;     }
		if (flags_1 & 0b00000010) { double_height = 1;    } else { double_height = 0;     }
		if (flags_1 & 0b00010000) {      hor_flip = true; } else {      hor_flip = false; }
		if (flags_1 & 0b00100000) {      ver_flip = true; } else {      ver_flip = false; }
		if (flags_1 & 0b01000000) {       xy_flip = true; } else {       xy_flip = false; }
		
		/*
		 * Need to recalculate dimensions because double size influences
		 * tile sizes on screen.
		 */
		calculate_dimensions();
	}
	
	inline void set_x_pos(int16_t x) { x_pos = x; }
	inline void set_y_pos(int16_t y) { y_pos = y; }
	inline int16_t get_x_pos() { return x_pos; }
	inline int16_t get_y_pos() { return y_pos; }
	
	inline void set_foreground_color(uint16_t fg_color) { foreground_color = fg_color; }
	inline void set_background_color(uint16_t bg_color) { background_color = bg_color; }
	inline uint16_t get_foreground_color() { return foreground_color; }
	inline uint16_t get_background_color() { return background_color; }
	
	inline uint8_t  get_tile_width()  { return tile_width;  }
	inline uint8_t  get_tile_height() { return tile_height; }
	inline uint8_t  get_columns()     { return columns;     }
	inline uint8_t  get_rows()        { return rows;        }
	inline uint16_t get_tiles()       { return tiles;       }
	inline uint16_t get_width()       { return width;       }
	inline uint16_t	get_height()      { return height;      }

	uint16_t cursor_position;
	uint8_t  cursor_interval;
	uint8_t  cursor_countdown;
	char     cursor_original_char;
	uint16_t cursor_original_color;
	uint16_t cursor_original_background_color;
	bool     cursor_blinking;
	bool     cursor_big_move;
	char     command_buffer[COMMAND_BUFFER_SIZE];

	inline int     terminal_lines_remaining()    { return rows - (cursor_position / columns) - 1; }
	inline uint8_t terminal_get_current_column() { return cursor_position % columns; }
	inline uint8_t terminal_get_current_row()    { return cursor_position / columns; }
};

}

#endif
