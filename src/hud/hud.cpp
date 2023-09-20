/*
 * hud.cpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#include "hud.hpp"
#include "common.hpp"

E64::hud_t::hud_t(E64::blitter_ic *b)
{
	blitter = b;
	
	blitter->terminal_init(0, 0x1a, 0x00, 1, 1, 47, 6, AMBER_07, (AMBER_05 & 0x0fff) | 0x8000);
	blitter->terminal_clear(0);
	blitter->blit[0].set_x_pos(4);
	blitter->blit[0].set_y_pos(188);
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
	while (blitter->run_next_operation()) {}
}
