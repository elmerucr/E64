/*
 * core.cpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#include "core.hpp"
#include "sound.hpp"
#include <cstdint>
#include <iostream>

E64::sound_ic *sound;

static int l_poke_sound(lua_State *L)
{
	uint16_t address = lua_tointeger(L, 1);
	uint8_t value = lua_tointeger(L, 2);
	sound->write_byte(address, value);
	return 0;
}

static int l_peek_sound(lua_State *L)
{
	uint16_t address = lua_tointeger(L, 1);
	lua_pushinteger(L, sound->read_byte(address));
	return 1;
}

static int l_sid1_voice1_set_freq(lua_State *L)
{
	uint16_t freq = lua_tointeger(L, 1);
	sound->write_byte(128, freq >> 8);
	sound->write_byte(129, freq & 0xff);
	return 0;
}

static int l_sid1_voice1_set_pw(lua_State *L)
{
	uint16_t freq = lua_tointeger(L, 1);
	sound->write_byte(130, freq >> 8);
	sound->write_byte(131, freq & 0xff);
	return 0;
}

static int l_sid1_voice2_set_freq(lua_State *L)
{
	uint16_t freq = lua_tointeger(L, 1);
	sound->write_byte(136, freq >> 8);
	sound->write_byte(137, freq & 0xff);
	return 0;
}

static int l_sid1_voice2_set_pw(lua_State *L)
{
	uint16_t freq = lua_tointeger(L, 1);
	sound->write_byte(138, freq >> 8);
	sound->write_byte(139, freq & 0xff);
	return 0;
}

static int l_sid1_set_mode_volume(lua_State *L)
{
	uint8_t mod_vol = lua_tointeger(L, 1);
	sound->write_byte(155, mod_vol);
	return 0;
}

static int l_mixer_sid1_left_set_volume(lua_State *L)
{
	uint8_t vol = lua_tointeger(L, 1);
	sound->write_byte(0x200, vol);
	return 0;
}

static int l_mixer_sid1_right_set_volume(lua_State *L)
{
	uint8_t vol = lua_tointeger(L, 1);
	sound->write_byte(0x201, vol);
	return 0;
}

const char lua_code[] = R"Lua(

local teller = 0

-- spy vs spy i track 1
local track1 = {
    0x04e2, 0x09c4, 0x0ea2, 0x1389, 0x04e2, 0x09c4, 0x0000, 0x0000,
    0x03e0, 0x07c1, 0x0b9d, 0x0f81, 0x03e0, 0x07c1, 0x0000, 0x0000,
    0x0342, 0x0685, 0x09c4, 0x0d0a, 0x0342, 0x0685, 0x0000, 0x0000,
    0x045a, 0x08b4, 0x0d0a, 0x1167, 0x03a9, 0x0751, 0x0af7, 0x0ea2,

    0x04e2, 0x09c4, 0x0ea2, 0x1389, 0x04e2, 0x09c4, 0x0000, 0x0000,
    0x03e0, 0x07c1, 0x0b9d, 0x0f81, 0x03e0, 0x07c1, 0x0000, 0x0000,
    0x0342, 0x0685, 0x09c4, 0x0d0a, 0x0342, 0x0685, 0x0000, 0x0000,
    0x045a, 0x08b4, 0x0d0a, 0x1167, 0x03a9, 0x0751, 0x0af7, 0x0ea2,

    0x04e2, 0x09c4, 0x0ea2, 0x1389, 0x04e2, 0x09c4, 0x0000, 0x0000,
    0x03e0, 0x07c1, 0x0b9d, 0x0f81, 0x03e0, 0x07c1, 0x0000, 0x0000,
    0x0342, 0x0685, 0x09c4, 0x0d0a, 0x0342, 0x0685, 0x0000, 0x0000,
    0x045a, 0x08b4, 0x0d0a, 0x1167, 0x03a9, 0x0751, 0x0af7, 0x0ea2
}

-- spy vs spy i track 2
local track2 = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x04e2, 0x09c4, 0x0ea2, 0x1389, 0x04e2, 0x09c4, 0x0000, 0x0000,
    0x04e2, 0x09c4, 0x0ea2, 0x1389, 0x04e2, 0x09c4, 0x0000, 0x0000,
    0x04e2, 0x09c4, 0x0ea2, 0x1389, 0x04e2, 0x09c4, 0x0000, 0x0000,
    0x04e2, 0x09c4, 0x0ea2, 0x1389, 0x04e2, 0x09c4, 0x0000, 0x0000
 }

-- volume
mixer_sid1_left_set_volume(128)
mixer_sid1_right_set_volume(128)
sid1_set_mode_volume(15)

-- pulse width
sid1_voice1_set_pw(0xf0f)
sid1_voice2_set_pw(0xf0f)

-- initial value of ticks
local ticks = 50
local counter = 1

-- callback function
function timer0_callback()
    if ticks == 0 then
        if track1[counter] ~= 0 then
            sid1_voice1_set_freq(track1[counter])
            poke_sound(0x05, 0x04)
            poke_sound(0x06, 0x15)
            poke_sound(0x04, 0x41)
        end
        if track2[counter] ~= 0 then
            sid1_voice2_set_freq(track2[counter])
            poke_sound(13, 0x04)
            poke_sound(14, 0x15)
            poke_sound(12, 0x41)
        end
        ticks = 10
        counter = counter + 1
        if counter == (#track1 + 1) then
            counter = 1
        end
    elseif ticks == 3 then
        poke_sound(0x05, 0x00)
        poke_sound(0x06, 0x00)
        poke_sound(0x04, 0x40)
        poke_sound(13, 0x00)
        poke_sound(14, 0x00)
        poke_sound(12, 0x40)
    end
    ticks = ticks - 1
end

function timer1_callback()
    print("timer1_callbackl()")
end

function timer2_callback()
    print("timer2_callbackl()")
end

function timer3_callback()
    print("timer3_callbackl()")
end

function timer4_callback()
    print("timer4_callbackl()")
end

function timer5_callback()
    print("timer5_callbackl()")
end

function timer6_callback()
    print("timer6_callbackl()")
end

function timer7_callback()
    print("timer7_callbackl()")
end

function frame()
    -- fill up with something
    print(teller)
    teller = teller + 1
end

)Lua";

E64::core_t::core_t(E64::settings_t *_s, E64::host_t *h, E64::keyboard_t *k, E64::sound_ic *s)
{
	/*
	 * Start Lua
	 */
	if (!L) L = luaL_newstate();
	
	if (!L) {
		printf("[core] error, couldn't start Lua\n");
		// TODO: failure... exit?
	} else {
		printf("[core] %s\n", LUA_COPYRIGHT);
	}
	
	luaL_openlibs(L);
	
	settings = _s;
	host = h;
	keyboard = k;
	sound = s;	// assign sound before pushing c functions
	
	lua_pushcfunction(L, l_poke_sound);
	lua_setglobal(L, "poke_sound");
	lua_pushcfunction(L, l_peek_sound);
	lua_setglobal(L, "peek_sound");
	
	lua_pushcfunction(L, l_sid1_voice1_set_freq);
	lua_setglobal(L, "sid1_voice1_set_freq");
	lua_pushcfunction(L, l_sid1_voice1_set_pw);
	lua_setglobal(L, "sid1_voice1_set_pw");
	
	lua_pushcfunction(L, l_sid1_voice2_set_freq);
	lua_setglobal(L, "sid1_voice2_set_freq");
	lua_pushcfunction(L, l_sid1_voice2_set_pw);
	lua_setglobal(L, "sid1_voice2_set_pw");
	
	lua_pushcfunction(L, l_sid1_set_mode_volume);
	lua_setglobal(L, "sid1_set_mode_volume");
	
	lua_pushcfunction(L, l_mixer_sid1_left_set_volume);
	lua_setglobal(L, "mixer_sid1_left_set_volume");
	lua_pushcfunction(L, l_mixer_sid1_right_set_volume);
	lua_setglobal(L, "mixer_sid1_right_set_volume");
	
	luaL_dostring(L, lua_code);
	
	/*
	 * Start Blitter
	 */
	blitter = new blitter_ic(MAX_PIXELS_PER_SCANLINE, MAX_SCANLINES);
	
	blitter->reset();
	
	console = &blitter->blit[240];
	blitter->terminal_init(console->number, 0x0a, 0, 1, 1, 48, 24, BLUE_08, 0x0000);
	console->set_x_pos(0);
	console->set_y_pos(12);
	blitter->terminal_clear(console->number);
	blitter->terminal_printf(console->number, "E64 Computer System (C)2019-%i elmerucr\n", E64_YEAR);
	console_prompt();
	
	blitter->terminal_activate_cursor(console->number);
	
	console_command_history.clear();
	console_command_history.push_back("");
	
	monitor = &blitter->blit[241];
	blitter->terminal_init(monitor->number, 0x0a, 0, 1, 1, 44, 23, C64_LIGHTBLUE, C64_BLUE);
	monitor->set_x_pos(16);
	monitor->set_y_pos(16);
	blitter->terminal_clear(monitor->number);
	blitter->terminal_printf(monitor->number, "Monitor");
	monitor_prompt();
	blitter->terminal_activate_cursor(monitor->number);
}

E64::core_t::~core_t()
{
	/*
	 * Clean up blitter
	 */
	delete blitter;
	
	/*
	 * Clean up Lua
	 */
	printf("[core] Closing Lua\n");
	lua_close(L);
}

void E64::core_t::reset()
{
	console_current_command.erase();
}

void E64::core_t::timer0_callback()
{
	lua_getglobal(L, "timer0_callback");
	lua_pcall(L, 0, 0, 0);
}

void E64::core_t::timer1_callback()
{
	lua_getglobal(L, "timer1_callback");
	lua_pcall(L, 0, 0, 0);
}

//void E64::core_t::do_sound_and_timers(uint32_t cycles)
//{
//	uint32_t accumulated_cycles = 0;
//	
//	
//}

void E64::core_t::do_frame()
{
	switch (current_state) {
		case CONSOLE:
			blitter->terminal_process_cursor_state(console->number);
			if (keyboard->events_waiting()) {
				blitter->terminal_deactivate_cursor(console->number);
				console_process_keypresses();
				blitter->terminal_activate_cursor(console->number);
			}
			blitter->set_clear_color(BLUE_03);
			blitter->clear_framebuffer();
			
			blitter->add_operation_draw_blit(console);
			
			blitter->set_ver_border_size(0x00);
			blitter->set_ver_border_color(BLUE_01);
			blitter->add_operation_draw_ver_border();
			
			blitter->set_hor_border_size(0x0c);
			blitter->set_hor_border_color(BLUE_01);
			blitter->add_operation_draw_hor_border();
			
			while (blitter->run_next_operation()) {}
			break;
		case MON:
			blitter->terminal_process_cursor_state(monitor->number);
			if (keyboard->events_waiting()) {
				blitter->terminal_deactivate_cursor(monitor->number);
				monitor_process_keypresses();
				blitter->terminal_activate_cursor(monitor->number);
			}
			blitter->set_clear_color(C64_BLUE);
			blitter->clear_framebuffer();
			
			blitter->add_operation_draw_blit(monitor);
			
			blitter->set_ver_border_size(0x10);
			blitter->set_ver_border_color(C64_LIGHTBLUE);
			blitter->add_operation_draw_ver_border();
			
			blitter->set_hor_border_size(0x10);
			blitter->set_hor_border_color(C64_LIGHTBLUE);
			blitter->add_operation_draw_hor_border();
			
			while (blitter->run_next_operation()) {}
			break;
		case RUN:
			lua_getglobal(L, "frame");
			lua_pcall(L, 0, 0, 0);
			break;
	}
}

void E64::core_t::console_prompt()
{
	blitter->terminal_printf(console->number, "\n>");
	console_command_cursor_pos = 0;
}

void E64::core_t::console_process_keypresses()
{
	uint8_t symbol;
	while ((symbol = keyboard->pop_event())) {
		switch (symbol) {
			case ASCII_CURSOR_LEFT:
				if (console_command_cursor_pos > 0) {
					blitter->terminal_cursor_left(console->number);
					console_command_cursor_pos--;
				}
				break;
			case ASCII_CURSOR_RIGHT:
				if (console_command_cursor_pos < console_current_command.size()) {
					blitter->terminal_cursor_right(console->number);
					console_command_cursor_pos++;
				}
				break;
			case ASCII_CURSOR_UP:
				// TODO: Walk through list of former commands
				if (console_command_history.size()) {
//					blitter->blit[0].cursor_position = command_start_pos + command.size();
//					command_cursor_pos = command.size();
//					while (command_cursor_pos > 0) {
//						command.erase(command_cursor_pos - 1, 1);
//						command_cursor_pos--;
//						blitter->blit[0].cursor_position = command_start_pos;
//						blitter->terminal_printf(0, command.c_str());
//						blitter->terminal_putchar(0, ' ');
//						blitter->blit[0].cursor_position = command_start_pos + command_cursor_pos;
//					}
//					displayed_command ? displayed_command-- : 0;
//					blitter->terminal_printf(0, command_history[displayed_command].c_str());
//					command_cursor_pos = command_history[displayed_command].length();
				}
				break;
			case ASCII_CURSOR_DOWN:
				// TODO: Walk through list of former commands
				break;
			case ASCII_BACKSPACE:
				if (console_command_cursor_pos > 0) {
					console_current_command.erase(console_command_cursor_pos - 1, 1);
					console_command_cursor_pos--;
					blitter->terminal_cursor_left(console->number);
					blitter->terminal_printf(console->number, console_current_command.substr(console_command_cursor_pos, console_current_command.size() - console_command_cursor_pos).c_str());
					blitter->terminal_putchar(console->number, ' ');
					for (size_t i=console_current_command.size() - console_command_cursor_pos + 1; i > 0; i--)
						blitter->terminal_cursor_left(console->number);
				}
				break;
			case ASCII_LF:
				while (console_command_cursor_pos < console_current_command.size()) {
					blitter->terminal_cursor_right(console->number);
					console_command_cursor_pos++;
				}
				console_command_history.push_back(console_current_command);
				console_displayed_command = console_command_history.size();
				console_process_command();
				console_prompt();
				break;
			case ASCII_REVERSE_ON:
				console->reverse = true;
				break;
			case ASCII_REVERSE_OFF:
				console->reverse = false;
				break;
			default:
				if (console_current_command.size() < 256) {
					console_current_command.insert(console_command_cursor_pos, 1, symbol);
					console_command_cursor_pos++;
					uint16_t to_start = console_command_cursor_pos - 1;
					while (to_start--)
						blitter->terminal_backspace(console->number);
					blitter->terminal_printf(console->number, console_current_command.c_str());
					for (int i=0; i < (console_current_command.size() - console_command_cursor_pos); i++)
						blitter->terminal_cursor_left(console->number);
				}
				break;
		}
	}
}

/*
 * See: https://www.techiedelight.com/remove-leading-and-trailing-spaces-from-a-string-in-cpp/
 */
std::string& ltrim(std::string &str)
{
	auto it = std::find_if(str.begin(), str.end(), [](char &c) {
		return !std::iswspace(c);
	});
	str.erase(str.begin(), it);
	return str;
}

std::string& rtrim(std::string &str)
{
	auto it = std::find_if(str.rbegin(), str.rend(), [](char &c) {
		return !std::iswspace(c);
	});
	str.erase(it.base(), str.end());
	return str;
}

std::string& trim(std::string &str)
{
	return ltrim(rtrim(str));
}

void E64::core_t::console_process_command()
{
	trim(console_current_command);
	
	std::string delimiter = " ";
	
	std::string token = console_current_command.substr(0, console_current_command.find(delimiter));
	
	if (token.compare("cd") == 0) {
		// cd
		console_current_command = console_current_command.erase(0, token.length());
		trim(console_current_command);
		token = console_current_command.substr(0, console_current_command.find(delimiter));
		blitter->terminal_printf(console->number, "\ncd %s<<end>>", token.c_str());
	} else if (token.compare("clear") == 0) {
		blitter->terminal_clear(console->number);
	} else if (token.compare("ls") == 0) {
		console->reverse = true;
		blitter->terminal_printf(console->number, "\n%s", settings->working_dir);
		console->reverse = false;
		char buffer[2048];
		settings->read_working_dir(buffer);
		blitter->terminal_printf(console->number, buffer);
	} else if (token.compare("mon") == 0) {
		blitter->terminal_printf(console->number, "\nmonitor");
		current_state = MON;
	} else if (token.compare("pwd") == 0) {
		blitter->terminal_printf(console->number, "\n%s", settings->working_dir);
	} else if (token.compare("run") == 0) {
		// TODO: mode switch
		blitter->terminal_printf(console->number, "\nrun");
		current_state = RUN;
	} else if (token.compare("ver") == 0) {
		blitter->terminal_printf(console->number, "\nVersion %i.%i.%i", E64_MAJOR_VERSION, E64_MINOR_VERSION, E64_BUILD);
	} else {
		if (token.size()) blitter->terminal_printf(console->number, "\ncommand <%s>", token.c_str());
	}
	
	console_current_command.erase();
}

void E64::core_t::monitor_process_keypresses()
{
	uint8_t symbol;
	while ((symbol = keyboard->pop_event())) {
		switch (symbol) {
			case ASCII_CURSOR_LEFT:
				blitter->terminal_cursor_left(monitor->number);
				break;
			case ASCII_CURSOR_RIGHT:
				blitter->terminal_cursor_right(monitor->number);
				break;
			case ASCII_CURSOR_UP:
				blitter->terminal_cursor_up(monitor->number);
				break;
			case ASCII_CURSOR_DOWN:
				blitter->terminal_cursor_down(monitor->number);
				break;
			case ASCII_BACKSPACE:
				blitter->terminal_backspace(monitor->number);
				break;
			case ASCII_LF:
			default:
				blitter->terminal_putchar(monitor->number, symbol);
				break;
		}
	}
}

void E64::core_t::monitor_prompt()
{
	blitter->terminal_printf(monitor->number, "\n.");
}
