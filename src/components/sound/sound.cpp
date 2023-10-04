/*
 * sound.cpp
 * E64
 *
 * Copyright © 2019-2023 elmerucr. All rights reserved.
 */

#include "sound.hpp"
#include "common.hpp"

E64::sound_ic::sound_ic(E64::settings_t *s) : analog0(0), analog1(1), analog2(2), analog3(3)
{
	settings = s;
	
	/*
	 * Remapping SID registers, rewiring necessary to have big endian
	 * support and even addresses for word access.
	 */

	/*
	 * voice 1
	 */
	register_index[0x00] = 0x01;    // frequency high byte
	register_index[0x01] = 0x00;    // frequency low byte
	register_index[0x02] = 0x03;    // pulsewidth high byte
	register_index[0x03] = 0x02;    // pulsewidth low byte
	register_index[0x04] = 0x04;    // control register
	register_index[0x05] = 0x05;    // attack decay
	register_index[0x06] = 0x06;    // sustain release

	register_index[0x07] = 0x1f;    // PADDING BYTE

	/*
	 * voice 2
	 */
	register_index[0x08] = 0x08;    // frequency high byte
	register_index[0x09] = 0x07;    // frequency low byte
	register_index[0x0a] = 0x0a;    // pulsewidth high byte
	register_index[0x0b] = 0x09;    // pulsewidth low byte
	register_index[0x0c] = 0x0b;    // control register
	register_index[0x0d] = 0x0c;    // attack decay
	register_index[0x0e] = 0x0d;    // sustain release

	register_index[0x0f] = 0x1f;    // PADDING BYTE

	/*
	 * voice 3
	 */
	register_index[0x10] = 0x0f;    // frequency high byte
	register_index[0x11] = 0x0e;    // frequency low byte
	register_index[0x12] = 0x11;    // pulsewidth high byte
	register_index[0x13] = 0x10;    // pulsewidth low byte
	register_index[0x14] = 0x12;    // control register
	register_index[0x15] = 0x13;    // attack decay
	register_index[0x16] = 0x14;    // sustain release

	register_index[0x17] = 0x1f;    // PADDING BYTE

	// filter
	register_index[0x18] = 0x15;    // filter cutoff low byte  (bits 0-2)
	register_index[0x19] = 0x16;    // filter cutoff high byte (bits 3-10)
	register_index[0x1a] = 0x17;    // res filt
	register_index[0x1b] = 0x18;    // filtermode / volume

	// misc
	register_index[0x1c] = 0x19;    // pot x
	register_index[0x1d] = 0x1a;    // pot y
	register_index[0x1e] = 0x1b;    // osc3_random
	register_index[0x1f] = 0x1c;    // env3

	for (int i = 0; i<4; i++) {
		/*
		 * set chip model
		 */
		sid[i].set_chip_model(MOS6581);

		/*
		 * In order to make SID produce the same pitch as it
		 * would in a PAL C64, we must use the exact same clock
		 * speed (985248Hz). We'll have the same frequencies,
		 * envelope, etc... as in the real thing.
		 *
		 * Using a sort of Bresenham algorithm it will be
		 * possible to "connect" the clock of SID to any other
		 * clock. One condition: the other one, usually the cpu
		 * clock, must be faster.
		 */
		sid[i].set_sampling_parameters(SID_CLOCK_SPEED, SAMPLE_FAST, SAMPLE_RATE);
		sid[i].enable_filter(true);
		sid[i].reset();
	}
	
	/*
	 * reset cycle counters for sid chips
	 */
	delta_t_sid0 = 0;
	delta_t_sid1 = 0;
	delta_t_sid2 = 0;
	delta_t_sid3 = 0;

	/*
	 * silence all balance registers
	 */
	for (int i=0; i<0x10; i++) {
		balance_registers[i] = 0x00;
	}
}

E64::sound_ic::~sound_ic()
{
	/*
	 * nothing
	 */
}

uint8_t E64::sound_ic::read_byte(uint16_t address)
{
	switch (address & 0x300) {
		case 0x000:
			// sids
			switch (address & 0x1c) {
				case 0x1c:
					switch (address & 0x60) {
						case 0x00:
							return sid[0].read(register_index[address & 0x1f]);
						case 0x20:
							return sid[1].read(register_index[address & 0x1f]);
						case 0x40:
							return sid[2].read(register_index[address & 0x1f]);
						case 0x60:
							return sid[3].read(register_index[address & 0x1f]);
						default:
							return 0x00;
					}
				default:
					switch (address & 0xe0) {
						case 0x00:
							return sid[0].read(register_index[address & 0x1f]);
						case 0x20:
							return sid[1].read(register_index[address & 0x1f]);
						case 0x40:
							return sid[2].read(register_index[address & 0x1f]);
						case 0x60:
							return sid[3].read(register_index[address & 0x1f]);
						case 0x80:
						case 0xa0:
						case 0xc0:
						case 0xe0:
							return sid_shadow[address & 0x7f];
						default:
							return 0x00;
					}
			}
		case 0x100:
			// analogs
			switch (address & 0xe0) {
				case 0x00:
					return analog0.read_byte(address & 0x1f);
				case 0x20:
					return analog1.read_byte(address & 0x1f);
				case 0x40:
					return analog2.read_byte(address & 0x1f);
				case 0x60:
					return analog3.read_byte(address & 0x1f);
				default:
					return 0x00;
			}
		case 0x200:
			// mixer etc...
			if ((address & 0xf0) == 0) {
				return balance_registers[address & 0x0f];
			} else {
				return 0x00;
			}
		default:
			return 0x00;
	}
}

void E64::sound_ic::write_byte(uint16_t address, uint8_t byte)
{
	switch (address & 0x300) {
		case 0x000:
			// sids
			switch (address & 0x60) {
				case 0x00:
					sid[0].write(register_index[address & 0x1f], byte);
					break;
				case 0x20:
					sid[1].write(register_index[address & 0x1f], byte);
					break;
				case 0x40:
					sid[2].write(register_index[address & 0x1f], byte);
					break;
				case 0x60:
					sid[3].write(register_index[address & 0x1f], byte);
					break;
				default:
					break;
			}
			sid_shadow[address & 0x7f] = byte;
			break;
		case 0x100:
			// analogs
			switch (address & 0xe0) {
				case 0x00:
					analog0.write_byte(address & 0x1f, byte);
					break;
				case 0x20:
					analog1.write_byte(address & 0x1f, byte);
					break;
				case 0x40:
					analog2.write_byte(address & 0x1f, byte);
					break;
				case 0x60:
					analog3.write_byte(address & 0x1f, byte);
					break;
				default:
					break;
			}
			break;
		case 0x200:
			// mixer etc...
			if ((address & 0xf0) == 0) {
				balance_registers[address & 0x0f] = byte;
			}
			break;
		default:
			break;
	}
}

void E64::sound_ic::run(uint32_t number_of_cycles, E64::host_t *h)
{
	delta_t_sid0 += number_of_cycles;
	delta_t_sid1 = delta_t_sid0;
	delta_t_sid2 = delta_t_sid0;
	delta_t_sid3 = delta_t_sid0;
	/*
	 * clock(delta_t, buf, maxNoOfSamples) function:
	 *
	 * This function returns the number of samples written by the SID chip.
	 * delta_t is a REFERENCE to the number of cycles to be processed
	 * buf is the memory area in which data should be written
	 * maxNoOfSamples (internal size of the presented buffer)
	 */
	int n = sid[0].clock(delta_t_sid0, sample_buffer_mono_sid0, 65536);
	sid[1].clock(delta_t_sid1, sample_buffer_mono_sid1, 65536);
	sid[2].clock(delta_t_sid2, sample_buffer_mono_sid2, 65536);
	sid[3].clock(delta_t_sid3, sample_buffer_mono_sid3, 65536);

	/*
	 * Analog is not connected to the cycles made by the machine,
	 * it only needs to know the amount of samples to produce.
	 */
	analog0.run(n, sample_buffer_mono_analog0);
	analog1.run(n, sample_buffer_mono_analog1);
	analog2.run(n, sample_buffer_mono_analog2);
	analog3.run(n, sample_buffer_mono_analog3);

	for (int i=0; i<n; i++) {
		// left channel
		sample_buffer_stereo[2 * i] =
			(sample_buffer_mono_sid0[i]    * balance_registers[0x0]) +
			(sample_buffer_mono_sid1[i]    * balance_registers[0x2]) +
			(sample_buffer_mono_sid2[i]    * balance_registers[0x4]) +
			(sample_buffer_mono_sid3[i]    * balance_registers[0x6]) +
			(sample_buffer_mono_analog0[i] * balance_registers[0x8]) +
			(sample_buffer_mono_analog1[i] * balance_registers[0xa]) +
			(sample_buffer_mono_analog2[i] * balance_registers[0xc]) +
			(sample_buffer_mono_analog3[i] * balance_registers[0xe]);

		// right channel
		sample_buffer_stereo[(2 * i) + 1] =
			(sample_buffer_mono_sid0[i]    * balance_registers[0x1]) +
			(sample_buffer_mono_sid1[i]    * balance_registers[0x3]) +
			(sample_buffer_mono_sid2[i]    * balance_registers[0x5]) +
			(sample_buffer_mono_sid3[i]    * balance_registers[0x7]) +
			(sample_buffer_mono_analog0[i] * balance_registers[0x9]) +
			(sample_buffer_mono_analog1[i] * balance_registers[0xb]) +
			(sample_buffer_mono_analog2[i] * balance_registers[0xd]) +
			(sample_buffer_mono_analog3[i] * balance_registers[0xf]);

		/*
		 * Normalize both channels
		 *
		 * Output of both sids and analogs is int16_t (-32768,32767).
		 * Balance registers are 0-255. So if divided by 32768 * 255,
		 * maximum (float) output ranges from -4.0 to 4.0. That should
		 * be allright.
		 */
		sample_buffer_stereo[2 * i] /= 32768 * 255;		// left
		sample_buffer_stereo[(2 * i) + 1] /= 32768 * 255;	// right
		
		if (sound_starting) {
			sample_buffer_stereo[2 * i] *= (float)(4000-sound_starting) / 4000;
			sample_buffer_stereo[(2 * i) + 1] *= (float)(4000-sound_starting) / 4000;
			sound_starting--;
		}

		settings->audio_record_push_sample(sample_buffer_stereo[ 2 * i     ]);
		settings->audio_record_push_sample(sample_buffer_stereo[(2 * i) + 1]);
	}

	h->queue_audio((void *)sample_buffer_stereo, 2 * n * h->get_bytes_per_sample());
}

void E64::sound_ic::reset()
{
	sid[0].reset();
	sid[1].reset();
	sid[2].reset();
	sid[3].reset();
	
	sound_starting = 4000;
}
