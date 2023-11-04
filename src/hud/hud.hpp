/*
 * hud.hpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#ifndef HUD_HPP
#define HUD_HPP

#include "common.hpp"
#include "blitter.hpp"

namespace E64 {

class hud_t {
private:
	
	bool stats_visible{false};
	
	uint16_t notify_frame_counter;
	uint16_t notify_frames;
public:
	hud_t();
	~hud_t();
	
	char extra_stats[64] = "\0";
	
	blitter_ic *blitter;
	
	void redraw();
	inline void toggle_stats() { stats_visible = !stats_visible; }
	void print_stats(const char *text);
	
	void show_notification(const char *format, ...);
};


}

#endif
