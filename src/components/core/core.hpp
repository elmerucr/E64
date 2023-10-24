/*
 * core.hpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#ifndef CORE_HPP
#define CORE_HPP

#include "common.hpp"
#include "settings.hpp"
#include "lua.hpp"
#include "host.hpp"
#include "sound.hpp"
#include "blitter.hpp"
#include "keyboard.hpp"
#include "timer.hpp"
#include <string>
#include <vector>

namespace E64 {

typedef enum {
	CONSOLE,
	MON,
	RUN
} state;

class core_t {
private:
	lua_State *L{nullptr};
	
	settings_t *settings;
	host_t *host;
	keyboard_t *keyboard;
	
	std::vector<std::string> console_command_history;
	std::string console_current_command;
	uint16_t console_displayed_command{0};
	uint16_t console_command_cursor_pos;
	
	timer_t timer[8];
public:
	core_t(settings_t *_s, host_t *h, keyboard_t *k, sound_ic *s);
	~core_t();
	
	state current_state{CONSOLE};
	
	blitter_ic *blitter;
	blit_t *console;
	blit_t *monitor;
	
	void reset();
	
	void do_sound_and_timers(uint32_t cycles);
	void do_frame();
	
	void console_prompt();
	void console_process_keypresses();
	void console_process_command();
	
	void monitor_prompt();
	void monitor_process_keypresses();
	
	void timer0_callback();
	void timer1_callback();
	void timer2_callback();
	void timer3_callback();
	void timer4_callback();
	void timer5_callback();
	void timer6_callback();
	void timer7_callback();
};

}

#endif
