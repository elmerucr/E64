/*
 * core.hpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#ifndef CORE_HPP
#define CORE_HPP

#include "common.hpp"
#include "lua.hpp"
#include "host.hpp"
#include "sound.hpp"
#include "blitter.hpp"
#include "keyboard.hpp"

namespace E64 {

class core_t {
private:
	lua_State *L{nullptr};
	
	host_t *host;
	keyboard_t *keyboard;
	
	bool timer0_active{false};
public:
	core_t(host_t *h, keyboard_t *k, sound_ic *s);
	~core_t();
	
	blitter_ic *blitter;
	
	void reset();
	void process_keypresses();
	void process_command();
	void timer0_callback();
};

}

#endif
