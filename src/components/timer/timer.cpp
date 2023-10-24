/*
 * timer.cpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#include "timer.hpp"
#include "common.hpp"

#include <cstdio>

E64::timer_t::timer_t()
{
	// defaults to 1s
	clock_interval = SID_CLOCK_SPEED;
	
	activated = false;
	once = false;
}

void E64::timer_t::check_clock_interval()
{
	if (clock_interval < ((uint64_t)SID_CLOCK_SPEED / 1000)) {
		printf("[timer] warning: clock_interval too small, set to 0.001s\n");
		clock_interval = (uint64_t)SID_CLOCK_SPEED / 1000;
	}
	if (clock_interval > ((uint64_t)SID_CLOCK_SPEED * 3600)) {
		printf("[timer] warning: clock interval too large, set to 3600s\n");
		clock_interval = (uint64_t)SID_CLOCK_SPEED * 3600;
	}
}

void E64::timer_t::set_interval_frequency(double f)
{
	if (f <= 0.0) f = 0.000001;
	clock_interval = SID_CLOCK_SPEED / f;
	check_clock_interval();
}

void E64::timer_t::set_interval_time(double t)
{
	if (t < 0.0) t = 0.0;
	clock_interval = (uint64_t)SID_CLOCK_SPEED * t;
	check_clock_interval();
}

void E64::timer_t::set_interval_bpm(uint16_t bpm)
{
	if (bpm == 0) bpm = 1;
	double t = 60.0 / bpm;
	set_interval_time(t);
}

void E64::timer_t::start_once()
{
	counter = clock_interval;
	activated = true;
	once = true;
}

void E64::timer_t::start_repeat()
{
	counter = clock_interval;
	activated = true;
	once = false;
}
