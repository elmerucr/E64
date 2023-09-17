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
	
	blitter->terminal_init(0, 0x1a, 0x00, 1, 1, 32, 8, AMBER_07, 0x0000);
	blitter->terminal_clear(0);
	blitter->terminal_printf(0, "E64 Lua Based System");
}
