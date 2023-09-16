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

static int l_sid0_voice1_set_freq(lua_State *L)
{
	uint16_t freq = lua_tointeger(L, 1);
	sound->sid[0].write(0, freq & 0xff);
	sound->sid[0].write(1, freq >> 8);
	return 0;
}

static int l_sid0_voice2_set_freq(lua_State *L)
{
	uint16_t freq = lua_tointeger(L, 1);
	sound->sid[0].write(7, freq & 0xff);
	sound->sid[0].write(8, freq >> 8);
	return 0;
}

char lua_code[] = R"Lua(  

-- spy vs spy i track 1
local track1 = {
    0x04b4, 0x0968, 0x0e18, 0x12d1, 0x04b4, 0x0968, 0x0000, 0x0000,
    0x03bb, 0x0777, 0x0b30, 0x0eef, 0x03bb, 0x0777, 0x0000, 0x0000,
    0x0323, 0x0647, 0x0968, 0x0c8f, 0x0323, 0x0647, 0x0000, 0x0000,
    0x0430, 0x0861, 0x0c8f, 0x10c3, 0x0386, 0x070c, 0x0a8f, 0x0e18,

    0x04b4, 0x0968, 0x0e18, 0x12d1, 0x04b4, 0x0968, 0x0000, 0x0000,
    0x03bb, 0x0777, 0x0b30, 0x0eef, 0x03bb, 0x0777, 0x0000, 0x0000,
    0x0323, 0x0647, 0x0968, 0x0c8f, 0x0323, 0x0647, 0x0000, 0x0000,
    0x0430, 0x0861, 0x0c8f, 0x10c3, 0x0386, 0x070c, 0x0a8f, 0x0e18,

    0x04b4, 0x0968, 0x0e18, 0x12d1, 0x04b4, 0x0968, 0x0000, 0x0000,
    0x03bb, 0x0777, 0x0b30, 0x0eef, 0x03bb, 0x0777, 0x0000, 0x0000,
    0x0323, 0x0647, 0x0968, 0x0c8f, 0x0323, 0x0647, 0x0000, 0x0000,
    0x0430, 0x0861, 0x0c8f, 0x10c3, 0x0386, 0x070c, 0x0a8f, 0x0e18
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
    0x04b4, 0x0968, 0x0e18, 0x12d1, 0x04b4, 0x0968, 0x0000, 0x0000,
    0x04b4, 0x0968, 0x0e18, 0x12d1, 0x04b4, 0x0968, 0x0000, 0x0000,
    0x04b4, 0x0968, 0x0e18, 0x12d1, 0x04b4, 0x0968, 0x0000, 0x0000,
    0x04b4, 0x0968, 0x0e18, 0x12d1, 0x04b4, 0x0968, 0x0000, 0x0000
 }

 -- volume
poke_sound(0x200,128)
poke_sound(0x201,128)
poke_sound(0x1b,15)

-- pulse width
poke_sound(2, 15)
poke_sound(3, 15)

poke_sound(10, 15)
poke_sound(11, 15)


-- initial value of ticks
local ticks = 50
local counter = 1

-- callback function
function tick()
    if ticks == 0 then
        if track1[counter] ~= 0 then
            sid0_voice1_set_freq(track1[counter])
            poke_sound(0x05, 0x04)
            poke_sound(0x06, 0x15)
            poke_sound(0x04, 0x41)
        end
        if track2[counter] ~= 0 then
            sid0_voice2_set_freq(track2[counter])
            poke_sound(13, 0x04)
            poke_sound(14, 0x15)
            poke_sound(12, 0x41)
        end
        ticks = 10
        counter = counter + 1
        if counter == 97 then
            counter = 1
        end
    elseif ticks == 2 then
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
	lua_pushcfunction(L, l_sid0_voice1_set_freq);
	lua_setglobal(L, "sid0_voice1_set_freq");
	lua_pushcfunction(L, l_sid0_voice2_set_freq);
	lua_setglobal(L, "sid0_voice2_set_freq");
	
	luaL_dostring(L, lua_code);
}

E64::core_t::~core_t()
{
	printf("[core] Closing Lua\n");
	lua_close(L);
}

void E64::core_t::tick()
{
	lua_getglobal(L, "tick");
	lua_pcall(L, 0, 0, 0);
}
