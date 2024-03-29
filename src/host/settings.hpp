/*
 * settings.hpp
 * E64
 *
 * Copyright © 2021-2023 elmerucr. All rights reserved.
 */
 
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <cstdio>
#include <cstdint>
#include <dirent.h>

#include "lua.hpp"

namespace E64 {

struct wav_header_t {
	/*
	 * The "RIFF" chunk descriptor (12 bytes)
	 */
	uint8_t	ChunkID[4];		// "RIFF"
	uint8_t	ChunkSize[4];
	uint8_t	Format[4];		// "WAVE"
	
	/*
	 * The "fmt" sub-chunk (24 bytes)
	 */
	uint8_t	Subchunk1ID[4];		// "fmt "
	uint8_t	Subchunk1Size[4];	// 16
	uint8_t AudioFormat[2];
	uint8_t NumChannels[2];
	uint8_t SampleRate[4];
	uint8_t ByteRate[4];
	uint8_t BlockAlign[2];
	uint8_t BitsPerSample[2];
	
	/*
	 * The "data" subchunk
	 */
	uint8_t Subchunk2ID[4];		// "data"
	uint8_t Subchunk2Size[4];
};

class settings_t
{
private:
	FILE *wav_file;

	void write_settings();
	
	struct wav_header_t wav_header;
	
	/*
	 * Lua virtual machine for reading settings file settings.lua
	 */
	lua_State *L;
public:
	settings_t();
	~settings_t();

	char home_dir[256];
	char *settings_dir;
	char working_dir[256];
	
	DIR *dir;
	
	bool video_fullscreen;
	bool video_linear_filtering;
	uint8_t video_scanlines_alpha;
	
	inline void audio_record_push_sample(float sample)
	{
		if (audio_recording) fwrite((void *)&sample, 4, 1, wav_file);
	}
	
	bool audio_recording{false};
	void audio_toggle_recording();
	void audio_start_recording();
	void audio_stop_recording();
	
	bool create_wav();
	
	void finish_wav();
	
	/*
	 * file system
	 */
	void read_working_dir(char *t);
};

}

#endif
