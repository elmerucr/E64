/*
 * keyboard.hpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#include "keyboard.hpp"
#include "common.hpp"

E64::keyboard_t::keyboard_t(host_t *h)
{
	host = h;
}

void E64::keyboard_t::process()
{
	uint32_t microseconds_per_frame = 1000000 / FPS;
	//double ms = 1000.0 / FPS;
	
	for (int i=0; i<128; i++) {
		if ((host->keyboard_state[i] & 0b11) == 0b01) putchar(i);
	}
}
