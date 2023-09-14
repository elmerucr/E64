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

char lua_code[] = R"Lua(

-- set volume
poke_sound(0x208, 0x7f)
poke_sound(0x209, 0x7f)

-- initial value of ticks
local ticks = 50

-- callback function
function tick()
  if ticks == 0 then
    poke_sound(0x100, 0x03)
    ticks = 29
  else
    poke_sound(0x100, 0x02)
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
