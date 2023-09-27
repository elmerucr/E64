/*
 * keyboard.hpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#include "keyboard.hpp"
#include "common.hpp"
#include <iostream>

E64::keyboard_t::keyboard_t(host_t *h)
{
	host = h;
	
	repeat_delay = 500000;
	repeat_speed = 50000;
}

void E64::keyboard_t::process()
{
	// TODO: modifier keys?
	
	for (int i=0; i<128; i++) {
		switch (host->keyboard_state[i] & 0b11) {
			case 0b01:
				key_down = true;
				last_char = i;
				time_to_next = repeat_delay;
				std::cout << last_char;
				break;
			case 0b10:
				if (i == last_char) {
					key_down = false;
				}
			default:
				break;
		}
	}

	microseconds_remaining = microseconds_per_frame;
	
	// TODO: check with generate_events?
	
	if (key_down) {
		while (microseconds_remaining) {
			if (time_to_next < microseconds_remaining) {
				microseconds_remaining -= time_to_next;
				time_to_next = repeat_speed;
				std::cout << last_char;
			} else {
				time_to_next -= microseconds_remaining;
				microseconds_remaining = 0;
			}
		}
	}
}
