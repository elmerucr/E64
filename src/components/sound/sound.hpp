/*
 * sound.hpp
 * E64
 *
 * Copyright © 2019-2023 elmerucr. All rights reserved.
 */

#include <cstdio>
#include <cstdint>

#ifndef SOUND_HPP
#define SOUND_HPP

#include "sid.h" // resid header
#include "analog.hpp"
#include "host.hpp"
#include "settings.hpp"

/*
 * TODO: Write description of how dealing with shadow registers: they're alway written to!
 */

namespace E64
{

class sound_ic {
private:
	settings_t *settings;
	/*
	 * sid variables etc...
	 */
	cycle_count delta_t_sid0;
	int16_t sample_buffer_mono_sid0[65536];
	cycle_count delta_t_sid1;
	int16_t sample_buffer_mono_sid1[65536];
	cycle_count delta_t_sid2;
	int16_t sample_buffer_mono_sid2[65536];
	cycle_count delta_t_sid3;
	int16_t sample_buffer_mono_sid3[65536];
	
	uint8_t sid_shadow[128];
	
	/*
	 * Used to rewire several lo/hi registers from sid to big endian
	 * and make it more convenient.
	 */
	uint8_t register_index[32];

	/*
	 * Analog
	 */
	analog_ic analog0;
	analog_ic analog1;
	analog_ic analog2;
	analog_ic analog3;
	int16_t sample_buffer_mono_analog0[65536];
	int16_t sample_buffer_mono_analog1[65536];
	int16_t sample_buffer_mono_analog2[65536];
	int16_t sample_buffer_mono_analog3[65536];
	
	/*
	 * General
	 */
	uint8_t balance_registers[0x10];
	float sample_buffer_stereo[131072];
	
	uint16_t sound_starting;
public:
	sound_ic(settings_t *s);
	~sound_ic();
	
	SID sid[4];
	
	// read and write functions to data registers of sid array and mixer
	uint8_t read_byte(uint16_t address);
	void write_byte(uint16_t address, uint8_t byte);
	// run the no of cycles that need to be processed by the sid chips on the sound device
	// and process all the accumulated cycles (flush into soundbuffer)
	void run(uint32_t number_of_cycles, host_t *h);
	void reset();
};

}

#endif
