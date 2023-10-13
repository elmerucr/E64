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
#include <string>
#include <vector>

namespace E64 {

typedef enum {
	CONSOLE,
	RUN
} state;

class core_t {
private:
	lua_State *L{nullptr};
	
	state current_state{CONSOLE};
	
	host_t *host;
	keyboard_t *keyboard;
	
	std::vector<std::string> command_history;
	std::string command;
	uint16_t displayed_command{0};
	//uint16_t command_start_pos;
	uint16_t command_cursor_pos;
	
	bool timer0_active{false};
public:
	core_t(host_t *h, keyboard_t *k, sound_ic *s);
	~core_t();
	
	blitter_ic *blitter;
	
	void reset();
	void do_frame();
	void prompt();
	void process_keypresses();
	void process_command();
	void timer0_callback();
};

}

#endif
