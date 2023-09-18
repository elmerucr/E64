/*
 * core.cpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#include "core.hpp"
#include "sound.hpp"
#include <cstdint>

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

char lua_code[] = R"Lua(  

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

E64::core_t::core_t(E64::sound_ic *s)
{
	L = luaL_newstate();
	
	if (!L) {
		// TODO: failure... exit?
	} else {
		printf("[core] %s\n", LUA_COPYRIGHT);
	}
	
	luaL_openlibs(L);
	
	sound = s;
	
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
}

E64::core_t::~core_t()
{
	printf("[core] Closing Lua\n");
	lua_close(L);
}

void E64::core_t::timer0_callback()
{
	lua_getglobal(L, "timer0_callback");
	lua_pcall(L, 0, 0, 0);
}
