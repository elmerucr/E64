/*
 * keyboard.hpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "host.hpp"

namespace E64 {

class keyboard_t {
private:
	host_t *host;
	bool generate_events{true};
public:
	keyboard_t(host_t *h);
	void process();
};

}

#endif
