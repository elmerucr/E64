/*
 * core.hpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#ifndef CORE_HPP
#define CORE_HPP

#include "lua.hpp"
#include "sound.hpp"

namespace E64 {

class core_t {
private:
	lua_State *L;
	
	bool timer0_active{false};
public:
	core_t(sound_ic *s);
	~core_t();
	
	void reset();
	
	void timer0_callback();
};

}


#endif
