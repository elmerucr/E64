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

)Lua";

E64::core_t::core_t(E64::host_t *h, E64::keyboard_t *k, E64::sound_ic *s)
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
	blitter->set_clear_color(BLUE_03);
	blitter->set_hor_border_size(12);
	blitter->set_hor_border_color(BLUE_01);
	blitter->set_ver_border_size(0x00);
	blitter->set_ver_border_color(BLUE_01);
	blitter->terminal_init(0, 0x0a, 0, 1, 1, 48, 24, BLUE_08, 0x0000);
	blitter->blit[0].set_x_pos(0);
	blitter->blit[0].set_y_pos(12);
	blitter->terminal_clear(0);
	blitter->terminal_printf(0, "E64 Computer System v%i.%i.%i", E64_MAJOR_VERSION, E64_MINOR_VERSION, E64_BUILD);
	blitter->terminal_printf(0, "\n\n(C)2019-%i elmerucr\n", E64_YEAR);
	prompt();
	
	blitter->terminal_activate_cursor(0);
	
	command_history.clear();
	command_history.push_back("");
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
	command.erase();
}

void E64::core_t::timer0_callback()
{
	lua_getglobal(L, "timer0_callback");
	lua_pcall(L, 0, 0, 0);
}

void E64::core_t::do_frame()
{
	switch (current_state) {
		case CONSOLE:
			blitter->terminal_process_cursor_state(0);
			if (keyboard->events_waiting()) {
				blitter->terminal_deactivate_cursor(0);
				process_keypresses();
				blitter->terminal_activate_cursor(0);
			}
			blitter->clear_framebuffer();
			blitter->add_operation_draw_blit(&blitter->blit[0]);
			blitter->add_operation_draw_ver_border();
			blitter->add_operation_draw_hor_border();
			while (blitter->run_next_operation()) {}
			break;
		case RUN:
			break;
	}
}

void E64::core_t::prompt()
{
	blitter->terminal_printf(0, "\n>");
	command_cursor_pos = 0;
}

void E64::core_t::process_keypresses()
{
	uint8_t symbol;
	while ((symbol = keyboard->pop_event())) {
		switch (symbol) {
			case ASCII_CURSOR_LEFT:
				if (command_cursor_pos > 0) {
					blitter->terminal_cursor_left(0);
					command_cursor_pos--;
				}
				break;
			case ASCII_CURSOR_RIGHT:
				if (command_cursor_pos < command.size()) {
					blitter->terminal_cursor_right(0);
					command_cursor_pos++;
				}
				break;
			case ASCII_CURSOR_UP:
				// TODO: Walk through list of former commands
				if (command_history.size()) {
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
				if (command_cursor_pos > 0) {
					command.erase(command_cursor_pos - 1, 1);
					command_cursor_pos--;
					blitter->terminal_cursor_left(0);
					blitter->terminal_printf(0, command.substr(command_cursor_pos, command.size() - command_cursor_pos).c_str());
					blitter->terminal_putchar(0, ' ');
					for (size_t i=command.size(); i >= command_cursor_pos; i--)
						blitter->terminal_cursor_left(0);
				}
				break;
			case ASCII_LF:
				while (command_cursor_pos < command.size()) {
					blitter->terminal_cursor_right(0);
					command_cursor_pos++;
				}
				command_history.push_back(command);
				displayed_command = command_history.size();
				process_command();
				prompt();
				break;
			default:
				command.insert(command_cursor_pos, 1, symbol);
				command_cursor_pos++;
				uint16_t to_start = command_cursor_pos - 1;
				while (to_start--)
					blitter->terminal_backspace(0);
				blitter->terminal_printf(0, command.c_str());
				for (int i=0; i < (command.size() - command_cursor_pos); i++)
					blitter->terminal_cursor_left(0);
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

void E64::core_t::process_command()
{
	trim(command);
	if (command.size()) blitter->terminal_printf(0, "\ncommand: %s<END>", command.c_str());
//	for (int i=0; i < command_history.size(); i++) {
//		std::cout << command_history[i] << std::endl;
//	}
	command.erase();
}
