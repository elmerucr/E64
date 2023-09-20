/*
 * hud.hpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#ifndef HUD_HPP
#define HUD_HPP

#include "blitter.hpp"

namespace E64 {

class hud_t {
private:
	blitter_ic *blitter;
	
	bool stats_visible{false};
public:
	hud_t(E64::blitter_ic *b);
	
	void redraw();
	inline void toggle_stats() { stats_visible = !stats_visible; }
	void print_stats(const char *text);
};


}

#endif
