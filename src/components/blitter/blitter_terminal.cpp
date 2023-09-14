/*
 * blitter_terminal.cpp
 * E64
 *
 * Copyright © 2022-2023 elmerucr. All rights reserved.
 */

#include "common.hpp"
#include "blitter.hpp"

/*
 * TODO - needs work?
 */
#define RAM_SIZE_CPU_VISIBLE 0x1000000

void E64::blitter_ic::terminal_set_tile(uint8_t number, uint16_t cursor_position, char symbol)
{
	tile_ram[((number << 13) + cursor_position) & TILE_RAM_ELEMENTS_MASK] = symbol;
}

void E64::blitter_ic::terminal_set_tile_fg_color(uint8_t number, uint16_t cursor_position, uint16_t color)
{
	tile_foreground_color_ram[((number << 12) + cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK] = color;
}

void E64::blitter_ic::terminal_set_tile_bg_color(uint8_t number, uint16_t cursor_position, uint16_t color)
{
	tile_background_color_ram[((number << 12) + cursor_position) & TILE_BACKGROUND_COLOR_RAM_ELEMENTS_MASK] = color;
}

uint8_t E64::blitter_ic::terminal_get_tile(uint8_t number, uint16_t cursor_position)
{
	return tile_ram[((number << 13) + cursor_position) & TILE_RAM_ELEMENTS_MASK];
}

uint16_t E64::blitter_ic::terminal_get_tile_fg_color(uint8_t number, uint16_t cursor_position)
{
	return tile_foreground_color_ram[((number << 12) + cursor_position) & TILE_FOREGROUND_COLOR_RAM_ELEMENTS_MASK];
}

uint16_t E64::blitter_ic::terminal_get_tile_bg_color(uint8_t number, uint16_t cursor_position)
{
	return tile_background_color_ram[((number << 12) + cursor_position) & TILE_BACKGROUND_COLOR_RAM_ELEMENTS_MASK];
}

void E64::blitter_ic::set_pixel(uint8_t number, uint32_t pixel_no, uint16_t color)
{
	pixel_ram[((number << 14) + pixel_no) & PIXEL_RAM_ELEMENTS_MASK] = color;
}

uint16_t E64::blitter_ic::get_pixel(uint8_t number, uint32_t pixel_no)
{
	return pixel_ram[((number << 14) + pixel_no) & PIXEL_RAM_ELEMENTS_MASK];
}

void E64::blitter_ic::terminal_init(uint8_t number,
				    uint8_t flags0,
				    uint8_t flags1,
				    uint8_t tw,
				    uint8_t th,
				    uint8_t columns,
				    uint8_t rows,
				    uint16_t foreground_color,
				    uint16_t background_color)
{
	blit[number].background		= flags0 & 0x02 ? true : false;
	blit[number].multicolor	= flags0 & 0x04 ? true : false;
	blit[number].color_per_tile	= flags0 & 0x08 ? true : false;
	blit[number].font_no	= (flags0 & 0xf0) >> 4;

	blit[number].flags_1 = flags1;
	blit[number].process_flags_1();
	
	blit[number].set_tile_width(tw);
	blit[number].set_tile_height(th);
	blit[number].set_columns(columns);
	blit[number].set_rows(rows);

	blit[number].foreground_color = foreground_color;
	blit[number].background_color = background_color;
}

void E64::blitter_ic::terminal_clear(uint8_t number)
{
	for (size_t i=0; i < blit[number].get_tiles(); i++) {
		terminal_set_tile(number, i, ' ');
		terminal_set_tile_fg_color(number, i, blit[number].foreground_color);
		terminal_set_tile_bg_color(number, i, blit[number].background_color);
	}

	blit[number].cursor_position = 0;

	blit[number].cursor_interval = 20; 	// 0.33s (if timer @ 60Hz)
	blit[number].cursor_countdown = 0;
	blit[number].cursor_blinking = false;

	blit[number].cursor_big_move = false;
}

void E64::blitter_ic::terminal_putsymbol_at_cursor(uint8_t number, char symbol)
{
	terminal_set_tile(number, blit[number].cursor_position, symbol);
	terminal_set_tile_fg_color(number, blit[number].cursor_position, blit[number].foreground_color);
	terminal_set_tile_bg_color(number, blit[number].cursor_position, blit[number].background_color);
}

void E64::blitter_ic::terminal_putsymbol(uint8_t number, char symbol)
{
	terminal_putsymbol_at_cursor(number, symbol);
	blit[number].cursor_position++;
	if (blit[number].cursor_position >= blit[number].get_tiles()) {
		terminal_add_bottom_row(number);
		blit[number].cursor_position -= blit[number].get_columns();
	}
}

int E64::blitter_ic::terminal_putchar(uint8_t number, int character)
{
	uint8_t result = (uint8_t)character;
	switch (result) {
		case '\r':
			blit[number].cursor_position -= blit[number].cursor_position % blit[number].get_columns();
			break;
		case '\n':
			blit[number].cursor_position -= blit[number].cursor_position % blit[number].get_columns();
			if ((blit[number].cursor_position / blit[number].get_columns()) == (blit[number].get_rows() - 1)) {
				terminal_add_bottom_row(number);
			} else {
				blit[number].cursor_position += blit[number].get_columns();
			}
			break;
		case '\t':
			while ((blit[number].cursor_position % blit[number].get_columns()) & 0b11) {
				terminal_putsymbol(number, ' ');
			}
			break;
		default:
			terminal_putsymbol(number, result);
			break;
	}
	return result;
}

int E64::blitter_ic::terminal_puts(uint8_t number, const char *text)
{
	int char_count = 0;
	if (text) {
		while (*text) {
			terminal_putchar(number, *text);
			char_count++;
			text++;
		}
	}
	return char_count;
}

int E64::blitter_ic::terminal_printf(uint8_t no, const char *format, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, format);
	int number = vsnprintf(buffer, 1024, format, args);
	va_end(args);
	terminal_puts(no, buffer);
	return number;
}


void E64::blitter_ic::terminal_prompt(uint8_t number)
{
	terminal_putchar(number, '\n');
}

void E64::blitter_ic::terminal_activate_cursor(uint8_t number)
{
	blit[number].cursor_original_char = terminal_get_tile(number, blit[number].cursor_position);
	blit[number].cursor_original_color = terminal_get_tile_fg_color(number, blit[number].cursor_position);
	blit[number].cursor_original_background_color = terminal_get_tile_bg_color(number, blit[number].cursor_position);
	blit[number].cursor_blinking = true;
	blit[number].cursor_countdown = 0;
}

void E64::blitter_ic::terminal_deactivate_cursor(uint8_t no)
{
	blit[no].cursor_blinking = false;
	terminal_set_tile(no, blit[no].cursor_position, blit[no].cursor_original_char);
	terminal_set_tile_fg_color(no, blit[no].cursor_position, blit[no].cursor_original_color);
	terminal_set_tile_bg_color(no, blit[no].cursor_position, blit[no].cursor_original_background_color);
}

void E64::blitter_ic::terminal_process_cursor_state(uint8_t no)
{
	if (blit[no].cursor_blinking == true) {
		if (blit[no].cursor_countdown == 0) {
			terminal_set_tile(no, blit[no].cursor_position, terminal_get_tile(no, blit[no].cursor_position) ^ 0x80);
			if ((terminal_get_tile(no, blit[no].cursor_position) & 0x80) != (blit[no].cursor_original_char & 0x80)) {
				terminal_set_tile_fg_color(no, blit[no].cursor_position, blit[no].foreground_color);
			} else {
				terminal_set_tile_fg_color(no, blit[no].cursor_position, blit[no].cursor_original_color);
			}
			blit[no].cursor_countdown += blit[no].cursor_interval;
		}
		blit[no].cursor_countdown--;
	}
}

void E64::blitter_ic::terminal_cursor_decrease(uint8_t no)
{
	blit[no].cursor_position--;
	if (blit[no].cursor_position > blit[no].get_tiles() - 1) {
		blit[no].cursor_position = blit[no].get_tiles() - 1;
		blit[no].cursor_big_move = true;
	} else {
		blit[no].cursor_big_move = false;
	}
}

void E64::blitter_ic::terminal_cursor_increase(uint8_t no)
{
	blit[no].cursor_position++;
	if (blit[no].cursor_position > blit[no].get_tiles() - 1) {
		blit[no].cursor_position = 0;
		blit[no].cursor_big_move = true;
	} else {
		blit[no].cursor_big_move = false;
	}
}

void E64::blitter_ic::terminal_cursor_left(uint8_t no)
{
	uint16_t min_pos = 0;
	if (blit[no].cursor_position > min_pos) blit[no].cursor_position--;
}

void E64::blitter_ic::terminal_cursor_right(uint8_t no)
{
	blit[no].cursor_position++;
	if (blit[no].cursor_position > blit[no].get_tiles() - 1) {
		terminal_add_bottom_row(no);
		blit[no].cursor_position -= blit[no].get_columns();
	}
}

void E64::blitter_ic::terminal_cursor_up(uint8_t no)
{
	blit[no].cursor_position -= blit[no].get_columns();

	if (blit[no].cursor_position >= blit[no].get_tiles()) {
		uint32_t address;

		switch (terminal_check_output(no, true, &address)) {
			case E64::NOTHING:
				terminal_add_top_row(no);
				break;
			case E64::ASCII:
				terminal_add_top_row(no);
				//hud.memory_dump((address-8) & (RAM_SIZE_CPU_VISIBLE - 1), 1);
				break;
			case E64::MONITOR_WORD:
				terminal_add_top_row(no);
				//hud.memory_word_dump((address - 16) & 0xfffffe, 1);
				break;
		}
	}
}


void E64::blitter_ic::terminal_cursor_down(uint8_t no)
{
	blit[no].cursor_position += blit[no].get_columns();

	// cursor out of current screen?
	if (blit[no].cursor_position >= blit[no].get_tiles()) {
		uint32_t address;

		switch (terminal_check_output(no, false, &address)) {
			case E64::NOTHING:
				terminal_add_bottom_row(no);
				blit[no].cursor_position -= blit[no].get_columns();
				break;
			case E64::ASCII:
				terminal_add_bottom_row(no);
				blit[no].cursor_position -= blit[no].get_columns();
				//hud.memory_dump((address+8) & (RAM_SIZE_CPU_VISIBLE - 1), 1);
				break;
			case E64::MONITOR_WORD:
				terminal_add_bottom_row(no);
				blit[no].cursor_position -= blit[no].get_columns();
				//hud.memory_word_dump((address + 16) & 0xfffffe, 1);
				break;
		}
	}
}

char *E64::blitter_ic::terminal_enter_command(uint8_t no)
{
	uint16_t start_of_line = blit[no].cursor_position - (blit[no].cursor_position % blit[no].get_columns());
	for (size_t i = 0; i < blit[no].get_columns(); i++) {
		blit[no].command_buffer[i] = terminal_get_tile(no, start_of_line + i);
	}
	size_t i = blit[no].get_columns() - 1;
	while (blit[no].command_buffer[i] == ' ') i--;
	blit[no].command_buffer[i + 1] = 0;

	return blit[no].command_buffer;
}

void E64::blitter_ic::terminal_add_top_row(uint8_t no)
{
	blit[no].cursor_position += blit[no].get_columns();
	for (int i=blit[no].get_tiles()-1; i >= (blit[no].cursor_position - blit[no].terminal_get_current_column()) + blit[no].get_columns(); i--) {
		terminal_set_tile(no, i, terminal_get_tile(no, i - blit[no].get_columns()));
		terminal_set_tile_fg_color(no, i, terminal_get_tile_fg_color(no, i - blit[no].get_columns()));
		terminal_set_tile_bg_color(no, i, terminal_get_tile_bg_color(no, i - blit[no].get_columns()));
	}
	uint16_t start_pos = blit[no].cursor_position - blit[no].terminal_get_current_column();
	for (int i=0; i < blit[no].get_columns(); i++) {
		terminal_set_tile(no, start_pos, ASCII_SPACE);
		terminal_set_tile_fg_color(no, start_pos, blit[no].foreground_color);
		terminal_set_tile_bg_color(no, start_pos, blit[no].background_color);
		start_pos++;
	}
}

void E64::blitter_ic::terminal_add_bottom_row(uint8_t no)
{
	uint16_t no_of_tiles_to_move = blit[no].get_tiles() - blit[no].get_columns();

	for (size_t i=0; i < no_of_tiles_to_move; i++) {
		terminal_set_tile(no, i, terminal_get_tile(no, i + blit[no].get_columns()));
		terminal_set_tile_fg_color(no, i, terminal_get_tile_fg_color(no, i + blit[no].get_columns()));
		terminal_set_tile_bg_color(no, i, terminal_get_tile_bg_color(no, i + blit[no].get_columns()));
	}
	for (size_t i=no_of_tiles_to_move; i < blit[no].get_tiles(); i++) {
		terminal_set_tile(no, i, ' ');
		terminal_set_tile_fg_color(no, i, blit[no].foreground_color);
		terminal_set_tile_bg_color(no, i, blit[no].background_color);
	}
}

enum E64::terminal_output_type E64::blitter_ic::terminal_check_output(uint8_t no, bool top_down, uint32_t *address)
{
	enum terminal_output_type output = NOTHING;

	for (int i = 0; i < blit[no].get_tiles(); i += blit[no].get_columns()) {
		if (terminal_get_tile(no, i) == ':') {
			output = ASCII;
			char potential_address[7];
			for (int j=0; j<6; j++) {
				potential_address[j] = terminal_get_tile(no, i+1+j);
			}
			potential_address[6] = 0;
			//hud.hex_string_to_int(potential_address, address);
			if (top_down) break;
		} else if (terminal_get_tile(no, i) == ';') {
			output = MONITOR_WORD;
			char potential_address[7];
			for (int j=0; j<6; j++) {
				potential_address[j] = terminal_get_tile(no, i+1+j);
			}
			potential_address[6] = 0;
			//hud.hex_string_to_int(potential_address, address);
			if (top_down) break;
		}
	}
	return output;
}


void E64::blitter_ic::terminal_backspace(uint8_t no)
{
	uint16_t pos = blit[no].cursor_position;
	uint16_t min_pos = 0;

	if (pos > min_pos) {
		blit[no].cursor_position--;
		while (pos % blit[no].get_columns()) {
			terminal_set_tile(no, pos - 1, terminal_get_tile(no, pos));
			terminal_set_tile_fg_color(no, pos - 1, terminal_get_tile_fg_color(no, pos));
			terminal_set_tile_bg_color(no, pos - 1, terminal_get_tile_bg_color(no, pos));
			pos++;
		}
		terminal_set_tile(no, pos - 1, ' ');
		terminal_set_tile_fg_color(no, pos - 1, blit[no].foreground_color);
		terminal_set_tile_bg_color(no, pos - 1, blit[no].background_color);
	}
}
