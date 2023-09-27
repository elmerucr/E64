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
public:
	keyboard_t(host_t *h);
	void process();
};

}

#endif
