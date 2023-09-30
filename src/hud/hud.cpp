/*
 * hud.cpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#include "hud.hpp"
#include "common.hpp"

E64::hud_t::hud_t()
{
	blitter = new blitter_ic(VM_MAX_PIXELS_PER_SCANLINE, VM_MAX_SCANLINES);
	
	blitter->reset();
	
	// stats item
	blitter->terminal_init(0, 0x0a, 0x00, 1, 1, 47, 6, AMBER_07, (AMBER_02 & 0x0fff) | 0xc000);
	blitter->terminal_clear(0);
	blitter->blit[0].set_x_pos(4);
	blitter->blit[0].set_y_pos(164);
	
	// notifications item
	blitter->terminal_init(1, 0x0a, 0x00, 1, 1, 47, 6, AMBER_07, (AMBER_02 & 0x0fff) | 0xc000);
	blitter->terminal_clear(1);
	blitter->terminal_printf(1, "\n\n\n\n\n\n");
	blitter->blit[1].set_x_pos(4);
	blitter->blit[1].set_y_pos(4);
	
	notify_frame_counter = 0;
	notify_frames = FPS * 3;	// 3 seconds
}

E64::hud_t::~hud_t()
{
	delete blitter;
}

void E64::hud_t::print_stats(const char *text)
{
	blitter->terminal_clear(0);
	blitter->terminal_printf(0, "%s", text);
}

void E64::hud_t::redraw()
{
	blitter->clear_framebuffer();
	if (stats_visible) blitter->add_operation_draw_blit(&blitter->blit[0]);
	if (notify_frame_counter != 0) {
		blitter->add_operation_draw_blit(&blitter->blit[1]);
		notify_frame_counter--;
	}
	while (blitter->run_next_operation()) {}
}

void E64::hud_t::show_notification(const char *format, ...)
{
	notify_frame_counter = notify_frames;
	
	for (size_t i=0; i < blitter->blit[1].get_tiles(); i++) {
		blitter->terminal_set_tile_fg_color(1, i, AMBER_03);
	}
	
	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	va_end(args);

	if (blitter) {
		blitter->terminal_printf(1, "%s", buffer);
	}
}
