/*
 * keyboard.hpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "host.hpp"
#include "common.hpp"

namespace E64 {

class keyboard_t {
private:
	host_t *host;
	bool generate_events{true};
	
	bool key_down{false};
	
	int32_t microseconds_per_frame = 1000000 / FPS;
	int32_t microseconds_remaining{0};
	int32_t time_to_next{0};
	int32_t repeat_delay;
	int32_t repeat_speed;
	
	uint8_t last_char;
	
	// implement a fifo event list, important for key presses, you don't want them in the wrong order
	uint8_t event_list[256];
	void push_event(uint8_t event);	// 'head' always points to the currently available location for an event
	// if (head == tail), no events are available
	uint8_t head;
	uint8_t tail;
public:
	keyboard_t(host_t *h);
	void process();
	
	void reset();
	
	uint8_t pop_event();
	
	inline bool events_waiting()
	{
		return (head == tail) ? false : true;
	}
};

}

#endif
