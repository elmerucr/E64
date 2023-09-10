/* analog.cpp
 * E64
 *
 * Copyright © 2021-2023 elmerucr. All rights reserved.
 */

#include "common.hpp"
#include "analog.hpp"
#include <cmath>
#include <cstdio>

E64::analog_ic::analog_ic(uint8_t no)
{
	id = no;
	
	sinus_amplitude = new double[TABLE_SIZE];
	triangle_amplitude = new double[TABLE_SIZE];
	sawtooth_amplitude = new double[TABLE_SIZE];
	
	exponential_increase = new double[TABLE_SIZE];
	exponential_decrease = new double[TABLE_SIZE];
	
	printf("[Analog%u] Creating wave lookup tables\n", id);
	for (int i=0; i<TABLE_SIZE; i++) {
		/*
		 * Using SAMPLE_RATE * MAX_WAVELENGTH no. of samples for
		 * lookup table.
		 */
		
		/* Sinus */
		sinus_amplitude[i] = sin(2*M_PI*((double)i / TABLE_SIZE));
		
		/* Triangle */
		if (i < (TABLE_SIZE / 4)) {
			triangle_amplitude[i] = (double)i / (TABLE_SIZE / 4);
		} else if (i < (3 * (TABLE_SIZE / 4))) {
			triangle_amplitude[i] = 2.0 - ((double)i / (TABLE_SIZE / 4));
		} else {
			triangle_amplitude[i] = -4.0 + ((double)i / (TABLE_SIZE / 4));
		}
		
		/* Sawtooth */
		if (i < (TABLE_SIZE / 2)) {
			sawtooth_amplitude[i] = (double)i / (TABLE_SIZE / 2);
		} else {
			sawtooth_amplitude[i] = -1.0 + ((double)(i - (TABLE_SIZE / 2)) / (TABLE_SIZE / 2));
		}
		
		/*
		 * Envelopes
		 */
		exponential_increase[i] = pow((double)i / (TABLE_SIZE - 1), STEEPNESS);
		exponential_decrease[(TABLE_SIZE - 1) - i] = exponential_increase[i];

		
		//printf("%i:\tsin %lf\ttri %lf\tsaw %lf\texpinc %lf\n", i, sinus_amplitude[i], triangle_amplitude[i], sawtooth_amplitude[i], exponential_increase[i]);
	}
	
	gate_open = false;
	envelope_stage = OFF;
	phase = 0;
	phase_remainder = 0;

	digital_freq = 0x02be;		// E1 kick
	//digital_freq = 0x0342;		// with sinus wave: G1 kick drum @49Hz!!!
	set_frequency();
	pitch_up = false;
	pitch_bend_on = true;
	pitch_factor = 36;		// 3 octaves = 8x higher
	pitch_bend_duration = 256;
	
	for (int i=0; i<256; i++) {
		pitch_equal_tempered_scale[i] = pow(2.0, (double)i / 12);
		//printf("%d, %f\n", i, pitch_tempered_scale[i]);
	}
	
	attack	= 2;		// 0.2 ms (not 2)
	decay	= 384;		// 384 ms
	sustain	= 0x0000;	// level
	release = 200;		// 200 ms
}

E64::analog_ic::~analog_ic()
{
	delete [] exponential_decrease;
	delete [] exponential_increase;
	
	delete [] sawtooth_amplitude;
	delete [] triangle_amplitude;
	delete [] sinus_amplitude;
}

uint8_t E64::analog_ic::read_byte(uint8_t address)
{
	switch (address) {
		case 0x00:
			return (waveform << 4) | (pitch_up ? 0b100 : 0b000) | (pitch_bend_on ? 0b10 : 0b00) | (gate_open ? 0b1 : 0b0);
		case 0x01:
			return pitch_factor;
		case 0x02:
			return digital_freq >> 8;
		case 0x03:
			return digital_freq & 0xff;
		case 0x04:
			return square_duty >> 8;
		case 0x05:
			return square_duty & 0xff;
		case 0x06:
			return attack >> 8;
		case 0x07:
			return attack & 0xff;
		case 0x08:
			return decay >> 8;
		case 0x09:
			return decay & 0xff;
		case 0x0a:
			return sustain >> 8;
		case 0x0b:
			return sustain & 0xff;
		case 0x0c:
			return release >> 8;
		case 0x0d:
			return release & 0xff;
		case 0x0e:
			return pitch_bend_duration >> 8;
		case 0x0f:
			return pitch_bend_duration & 0xff;
		default:
			return 0;
	}
}

void E64::analog_ic::write_byte(uint8_t address, uint8_t byte) {
	switch (address) {
		case 0x00:
			// TODO: situation 5 6 and 7
			waveform = (enum waveforms)(byte >> 4);
			(byte & 0b00000001) ? gate_open = true : gate_open = false;
			(byte & 0b00000010) ? pitch_bend_on = true : pitch_bend_on = false;
			(byte & 0b00000100) ? pitch_up = true : pitch_up = false;
			break;
		case 0x01:
			//pitch_factor = byte ? byte : 1;
			pitch_factor = byte;
			break;
		case 0x02:
			digital_freq = (byte << 8) | (digital_freq & 0xff);
			set_frequency();
			break;
		case 0x03:
			digital_freq = (digital_freq & 0xff00) | byte;
			set_frequency();
			break;
		case 0x04:
			square_duty = (byte << 8) | (square_duty & 0xff);
			break;
		case 0x05:
			square_duty = (square_duty & 0xff00) | byte;
			break;
		case 0x06:
			attack = (byte << 8) | (attack & 0xff);
			break;
		case 0x07:
			attack = (attack & 0xff00) | byte;
			break;
		case 0x08:
			decay = (byte << 8) | (decay & 0xff);
			break;
		case 0x09:
			decay = (decay & 0xff00) | byte;
			break;
		case 0x0a:
			sustain = (byte << 8) | (sustain & 0xff);
			break;
		case 0x0b:
			sustain = (sustain & 0xff00) | byte;
			break;
		case 0x0c:
			release = (byte << 8) | (release & 0xff);
			break;
		case 0x0d:
			release = (release & 0xff00) | byte;
			break;
		case 0x0e:
			pitch_bend_duration = (byte << 8) | (pitch_bend_duration & 0xff);
			break;
		case 0x0f:
			pitch_bend_duration = (pitch_bend_duration & 0xff00) | byte;
			break;
	}
}

void E64::analog_ic::run(uint16_t no_samples, int16_t *buffer)
{
	/*
	 * Calculate things that remain the same during this run
	 */
	uint32_t duty = ((double)square_duty/65535) * TABLE_SIZE;
	
	
	uint16_t no_samples_temp = no_samples;
	
	while (no_samples_temp--) {
		/*
		 * volume envelope
		 */
		switch (envelope_stage) {
			case OFF:
				envelope = 0.0;

				if (gate_open) {
					envelope_stage = ATTACK;
					phase = phase_delta = 0;
					envelope_target = 1.0;
					envelope_change = envelope_target;
					envelope_phase = envelope_phase_delta = 0;
					stage_samples = stage_samples_remaining =
						((SAMPLE_RATE * attack) / 10000) + 1;
					//printf("Attack: %0.1f ms --> %u samples\n", (double)attack/10, stage_samples);
					
					pitch_bend_reset();
				}
				break;
			case ATTACK:
				if (gate_open) {
					stage_samples_remaining--;
					
					envelope_phase += (TABLE_SIZE + envelope_phase_delta) / stage_samples;
					envelope_phase_delta = (TABLE_SIZE + envelope_phase_delta) % stage_samples;
					
					//printf("%u\n", envelope_phase);
					
					envelope = exponential_increase[envelope_phase - 1] * envelope_change;
					
					if (stage_samples_remaining == 0) {
						envelope = 1.0;
						envelope_stage = DECAY;
						envelope_target = (double)sustain / 0xffff;
						envelope_change = envelope - envelope_target;
						envelope_phase = envelope_phase_delta = 0;
						stage_samples = stage_samples_remaining
							= ((SAMPLE_RATE * decay) / 1000) + 1;
						//printf("Decay: %u ms --> %u samples\n", decay, stage_samples);
					}
				} else {
					envelope_stage = RELEASE;
					envelope_target = 0.0;
					envelope_change = envelope;
					envelope_phase = envelope_phase_delta = 0;
					stage_samples = stage_samples_remaining =
						((SAMPLE_RATE * release) / 1000) + 1;
					//printf("Release: %u ms --> %u samples\n", release, stage_samples);
				}
				break;
			case DECAY:
				//envelope = 1.0;
				stage_samples_remaining--;
				
				envelope_phase += (TABLE_SIZE + envelope_phase_delta) / stage_samples;
				envelope_phase_delta = (TABLE_SIZE + envelope_phase_delta) % stage_samples;
				
				envelope = envelope_target + (exponential_decrease[envelope_phase] * envelope_change);
				
				if (gate_open) {
					//stage_samples--;
					if (stage_samples_remaining == 0) {
						envelope_stage = SUSTAIN;
						// target ....
						//printf("Sustain at: %u\n", sustain);
					}
				} else {
					envelope_stage = RELEASE;
					envelope_target = 0.0;
					envelope_change = envelope;
					envelope_phase = envelope_phase_delta = 0;
					stage_samples = stage_samples_remaining =
						((SAMPLE_RATE * release) / 1000) + 1;
					//printf("Release: %u ms --> %u samples\n", release, stage_samples);
				}
				break;
			case SUSTAIN:
				envelope = (double)sustain / 65535.0;
				
				if (gate_open) {
					// don't change anything
				} else {
					envelope_stage = RELEASE;
					envelope_target = 0.0;
					envelope_change = envelope;
					envelope_phase = envelope_phase_delta = 0;
					stage_samples = stage_samples_remaining =
						((SAMPLE_RATE * release) / 1000) + 1;
					//printf("Release: %u ms --> %u samples\n", release, stage_samples);
				}
				break;
			case RELEASE:
				stage_samples_remaining--;
				
				envelope_phase += (TABLE_SIZE + envelope_phase_delta) / stage_samples;
				envelope_phase_delta = (TABLE_SIZE + envelope_phase_delta) % stage_samples;
				
				envelope = exponential_decrease[envelope_phase] * envelope_change;
				
				if (gate_open) {
					envelope_stage = ATTACK;
					phase = phase_delta = 0;
					envelope_target = 1.0;
					envelope_change = envelope_target - envelope;
					envelope_phase = envelope_phase_delta = 0;
					stage_samples = stage_samples_remaining =
						((SAMPLE_RATE * attack) / 10000) + 1;
					//printf("Attack: %0.1f ms --> %u samples\n", (double)attack/10, stage_samples);
					
					pitch_bend_reset();
				} else {
					if (stage_samples_remaining == 0) {
						envelope_stage = OFF;
						// envelope target = 0.0 ....
						//printf("Quiet\n");
					}
				}
				break;
		}
		
		/*
		 * Generate current sample
		 */
		if (envelope_stage != OFF) {

			switch (waveform) {
				case SINE:
					*buffer = 32767 * sinus_amplitude[phase];
					break;
				case SQUARE:
					(phase < duty) ? *buffer = 32767 : *buffer = -32767;
					break;
				case TRIANGLE:
					*buffer = 32767 * triangle_amplitude[phase];
					break;
				case SAWTOOTH:
					*buffer = 32767 * sawtooth_amplitude[phase];
				case NOISE:
					*buffer = (int16_t)((uniform_white_noise.byte() << 8) | uniform_white_noise.byte());
					break;
				default:
					*buffer = 0;
					break;
			}
			
			*buffer *= envelope;
			
			set_frequency();
			frequency *= pitch_bend();

			_frequency = frequency + (phase_remainder / MAX_WAVELENGTH);
			phase_delta = _frequency * MAX_WAVELENGTH;
			phase += phase_delta;
			phase %= TABLE_SIZE;
			phase_remainder = (MAX_WAVELENGTH * _frequency) - phase_delta;
			
		} else {
			// quiet
			*buffer = 0;
		}
		
		*buffer *= envelope;
		buffer++;
	}
}

void E64::analog_ic::set_frequency()
{
	// PAL sid translation from 16bit register to real frequency
	frequency = digital_freq / ((256*256*256) / 985248);
}

void E64::analog_ic::pitch_bend_reset()
{
	pitch_samples = pitch_samples_remaining =
		((SAMPLE_RATE * pitch_bend_duration) / 10000);
	pitch_bend_phase = pitch_bend_phase_delta = 0;
	
	//printf("E64::analog_ic::pitch_bend_reset(); %u samples\n", pitch_samples);
}

double E64::analog_ic::pitch_bend()
{
	if (pitch_samples_remaining) {
		pitch_samples_remaining--;
		pitch_bend_phase += (TABLE_SIZE + pitch_bend_phase_delta) / pitch_samples;
		pitch_bend_phase_delta = (TABLE_SIZE + pitch_bend_phase_delta) % pitch_samples;
		
		double result =
			(exponential_decrease[pitch_bend_phase] *
			(pitch_equal_tempered_scale[pitch_factor] - 1.0)) + 1.0;
		
		if (pitch_bend_on) {
			if (pitch_up) {
				return 1.0 / result;
			} else {
				return result;
			}
		}
	}
	return 1.0;
}

E64::rca::rca()
{
	/*
	 * Initial state of the 32bit register (29 bits are used):
	 *
	 * |x0000000|00000001|00000000|000000xx|
	 *
	 * One bit on the left side and two bits on the right side not
	 * used.
	 */
	stat = (1 << 16);
}

E64::rca::rca(uint32_t number)
{
	/*
	 * Alternative manual constructor
	 */
	stat = number;
}

uint8_t E64::rca::byte()
{
	/*
	 * Clean up the left and right side of the ca, make sure
	 * things are clean before processing
	 */
	stat = (((stat<<1)>>3)<<2);

	/*
	 * Need two temporary registers to hold data and combine...
	 */
	uint32_t temp1, temp2;

	/*
	 * Make the status has periodic boundary conditions: bit 1 gets a
	 * copy of bit 30. Bit 31 (leftmost) gets a copy of bit 2.
	 */
	temp1 = stat >> 29;
	temp2 = stat << 29;
	stat = stat | temp1 | temp2;

	temp1 = stat >> 1;
	temp2 = stat << 1;

	/*
	 * The actual rule 30 from Wolfram
	 */
	stat = (stat | temp2) ^ temp1;

	return (uint8_t)((stat<<8)>>24);
}

uint32_t E64::rca::status() {
	return stat;
}
