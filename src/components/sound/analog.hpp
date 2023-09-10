/* analog.hpp
 * E64
 *
 * Copyright © 2021-2023 elmerucr. All rights reserved.
 */

/*
 * All 2 byte pairs are unsigned 16bit BIG ENDIAN
 *
 * 0x00     :Waveforms and Gate
 *   7 6 5 4 3 2 1 0
 *   | | | |   | | |
 *   | | | |   | | +-- Gate bit, 1=open, 0=closed
 *   | | | |   | +---- Pitch Bend, 1=on, 0=off
 *   | | | |   +------ Pitch Up, 1=up, 0=down
 *   | | | |
 *   +-+-+-+---------- Waveform:
 *                      0000 = Sine
 *                      0001 = Triangle
 *                      0010 = Square
 *                      0011 = Sawtooth
 *                      0100 = Noise
 *
 * 0x01     : Pitch Fact
 or in semitones difference
 * 0x02/0x03: Digital frequency like SID chip @985248Hz
 * 0x04/0x05: Square wave duty, 0=0% and 65535=100%
 * 0x06/0x07: Attack, 0=0.0000s and 65535=6.5535s
 * 0x08/0x09: Decay, 0=0.000s and 65535=65.535s
 * 0x0a/0x0b: Sustain, 0=zero volume, 65535=max volume
 * 0x0c/0x0d: Release, 0=0.000s and 65535=65.535s
 * 0x0e/0x0f: Pitch bend duration, 0=0.000s and 65535=65.535s
 *
 */

#ifndef ANALOG_HPP
#define ANALOG_HPP

#include <cstdint>

/*
 * Maximum wavelength (in seconds) at full resolution. This results in
 * a minimum frequency of 1/MAX_WAVELENGTH at full resolution.
 */
#define MAX_WAVELENGTH	8
#define TABLE_SIZE	(MAX_WAVELENGTH*SAMPLE_RATE)

/*
 * Steepness defines the shape of the several envelop curves according
 * to:
 *
 * amplitude(t) = t^steepness
 * for t element of [0.000, 1.000]
 */
#define STEEPNESS	4

namespace E64
{

/*
 * rca: random number generation with cellular automaton
 *
 * 2012-01-21 c version
 * 2013-11-11 c++ version
 *
 */

class rca {
private:
	/*
	 * Will be a 29 cell prng, fits perfectly in a 32 bit register
	 */
	uint32_t stat;
public:
	/*
	 * Default constructor, simple init, turn on 1 bit in the
	 * "middle column", this bit serves also as the least significant
	 * bit of the byte that's returned.
	 */
	rca();
	
	/*
	 * Alternative constructor,uses an uint32_t for init.
	 */
	rca(uint32_t number);

	/*
	 * Generates a "random byte", this function currently actually
	 * performs two tasks:
	 *
	 * (1) Shifts internal status according to rule 30 (Wolfram)
	 * (2) Outputs part of internal status as 8 bit unsigned integer
	 */
	uint8_t byte();
	
	/*
	 * Returns status (e.g. to seed another generator), doesn't
	 * apply rule 30 on internal status.
	 */
	uint32_t status();
};

enum waveforms {
	SINE = 0,
	TRIANGLE,
	SQUARE,
	SAWTOOTH,
	NOISE
};

enum envelope_stages {
	OFF = 0,
	ATTACK,
	DECAY,
	SUSTAIN,
	RELEASE
};

class analog_ic {
private:
	uint8_t id;
	
	int16_t old_buffer;
	/*
	 * Basic waveforms
	 */
	double *sinus_amplitude;
	double *triangle_amplitude;
	double *sawtooth_amplitude;
	// how to do noise?
	
	/*
	 * Envelopes
	 */
	double *exponential_increase;
	double *exponential_decrease;
	
	bool gate_open;
	
	uint32_t phase;
	uint32_t phase_delta;
	double   phase_remainder;
	
	double	frequency, _frequency;
	void 	set_frequency();		// translates digital frequency to real frequency
	
	enum waveforms waveform;

	uint16_t square_duty;
	uint16_t digital_freq;

	// volume envelope is a finite stage machine
	enum envelope_stages envelope_stage;	// current stage
	double envelope;			// current value
	double envelope_target;			// what is aimed for within current stage

	uint32_t stage_samples;			// no of samples needed to finish current stage
						// calculated at each start of a new stage
	uint32_t stage_samples_remaining;
	
	uint32_t envelope_phase;
	uint32_t envelope_phase_delta;

	double   envelope_change;
	
	uint16_t attack;	// 0.0 -  6553.5 ms = 0.0000-6.5536s
	uint16_t decay;		// 0   - 65535   ms
	uint16_t sustain;	// 0   - 65535   level value
	uint16_t release;	// 0   - 65536   ms
	
	// pitch bend envelope
	uint16_t pitch_bend_duration;	// 0   - 65536   ms
	bool     pitch_bend_on;
	bool     pitch_up;
	
	uint8_t  pitch_factor;		// uses only bits 0-6 (and transposed +1
					// so values 1-128
	double   pitch_equal_tempered_scale[256];
	
	uint32_t pitch_samples;
	uint32_t pitch_samples_remaining;
	
	uint32_t pitch_bend_phase;
	uint32_t pitch_bend_phase_delta;
	
	void     pitch_bend_reset();
	double   pitch_bend();
	
	rca	uniform_white_noise;

public:
	analog_ic(uint8_t no);
	~analog_ic();
	uint8_t read_byte(uint8_t address);
	void write_byte(uint8_t address, uint8_t byte);
	
	void run(uint16_t no_samples, int16_t *buffer);
};

}

#endif
