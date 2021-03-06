#include <cstdlib>
#include <cstdint>

#include "lua.hpp"
#include "blitter.hpp"
#include "cia.hpp"
#include "timer.hpp"
#include "exceptions.hpp"

#ifndef HUD_HPP
#define HUD_HPP

#define MAXINPUT 1024

namespace E64 {

class hud_t {
private:
	bool irq_line;
	
	void process_command(char *buffer);
public:
	hud_t();
	~hud_t();
	
	bool paused;
	
	void flip_modes();

	void memory_dump(uint16_t address, int rows);
	void blit_memory_dump(uint32_t address, int rows);
	void enter_monitor_line(char *buffer);
	void enter_monitor_blit_line(char *buffer);
	bool hex_string_to_int(const char *temp_string, uint32_t *return_value);
	
	exceptions_ic *exceptions;
	blitter_ic *blitter;
	cia_ic *cia;
	timer_ic *timer;
	
	lua_State *L;
	
	blit_t *stats_view;
	blit_t *terminal;
	blit_t *cpu_view;
	blit_t *disassembly_view;
	blit_t *stack_view;
	blit_t *bar_single_height;
	blit_t *bar_double_height;
	blit_t *bar_single_height_small_1;
	blit_t *bar_single_height_small_2;
	blit_t *other_info;
	
	bool stats_visible;
	
	void reset();
	void run(uint16_t cycles);
	void update();
	void update_stats_view();
	void process_keypress();
	void redraw();
	
	// events
	void timer_0_event();
	void timer_1_event();
	void timer_2_event();
	void timer_3_event();
	void timer_4_event();
	void timer_5_event();
	void timer_6_event();
	void timer_7_event();
};

}

#endif
