/*
 * settings.cpp
 * E64
 *
 * Copyright © 2021-2023 elmerucr. All rights reserved.
 */

#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <sys/stat.h>
#include <SDL2/SDL.h>
#include <unistd.h>
#include "common.hpp"
#include "settings.hpp"

E64::settings_t::settings_t()
{
	snprintf(home_dir, 256, "%s", getenv("HOME"));
	printf("[Settings] User home directory: %s\n", home_dir);
	
	settings_dir = SDL_GetPrefPath("elmerucr", "E64");
	printf("[Settings] Opening settings directory: %s\n", settings_dir);

	/*
	 * Switch to settings path and read settings file using Lua
	 */
	chdir(settings_dir);

	L = luaL_newstate();
	luaL_openlibs(L);
	
	if (luaL_dofile(L, "settings.lua") == LUA_OK) {
		printf("[Settings] Reading 'settings.lua'\n");
	} else {
		printf("[Settings] settings.lua not found, using default settings\n");
	}
	
	lua_getglobal(L, "fullscreen");
	if (lua_isboolean(L, -1)) {
		video_fullscreen = lua_toboolean(L, -1);
	} else {
		video_fullscreen = false;
	}
	
	lua_getglobal(L, "working_dir");
	if (lua_isstring(L, -1)) {
		strcpy(working_dir, lua_tolstring(L, -1, nullptr));
		printf("[Settings] Working dir set to: %s\n", working_dir);
	} else {
		working_dir[0] = '\0';
	}
	
	lua_getglobal(L, "linear_filtering");
	if (lua_isboolean(L, -1)) {
		video_linear_filtering = lua_toboolean(L, -1);
	} else {
		video_linear_filtering = false;
	}
	
	lua_getglobal(L, "scanlines_alpha");
	if (lua_isinteger(L, -1)) {
		video_scanlines_alpha = (lua_tointeger(L, -1)) & 0xff;
	} else {
		video_scanlines_alpha = 64;
	}
	
	lua_close(L);
	
	/*
	 * Prepare sound export
	 */
	wav_file = nullptr;
	
	wav_header = {
		{ 0x52, 0x49, 0x46, 0x46 },	// "RIFF"
		{ 0x00, 0x00, 0x00, 0x00 },	// chunksize = needs to be calculated
		{ 0x57, 0x41, 0x56, 0x45 },	// "WAVE"
		
		{ 0x66, 0x6d, 0x74, 0x20 },	// "fmt "
		{ 0x10, 0x00, 0x00, 0x00 },	// 16
		{ 0x03, 0x00             },	// type of format = floating point PCM
		{ 0x02, 0x00             },	// two channels
		{ SAMPLE_RATE&0xff, (SAMPLE_RATE>>8)&0xff, (SAMPLE_RATE>>16)&0xff, (SAMPLE_RATE>>24)&0xff },
		{ (8*SAMPLE_RATE)&0xff, ((8*SAMPLE_RATE)>>8)&0xff, ((8*SAMPLE_RATE)>>16)&0xff, ((8*SAMPLE_RATE)>>24)&0xff },
		{ 0x08, 0x00             },	// block align
		{ 0x20, 0x00             },	// 32 bits per sample
		
		{ 0x64, 0x61, 0x74, 0x61 },	// "data"
		{ 0x00, 0x00, 0x00, 0x00 }	// subchunk2size
	};
	
	audio_clear_record_buffer();
}

E64::settings_t::~settings_t()
{
	if (audio_recording) {
		audio_stop_recording();
	}
	write_settings();
}

void E64::settings_t::write_settings()
{
	/*
	 * This is currently some kind of hack.
	 */
	chdir(settings_dir);
	FILE *temp_file = fopen("settings.lua", "w");
	
	if(!temp_file) {
		printf("Error: Can't open file 'settings.lua' for writing\n");
	} else {
		fwrite("-- Automatically generated settings file for E64", 1, 48, temp_file);

		if (video_fullscreen) {
			fwrite("\nfullscreen = true", 1, 18, temp_file);
		} else {
			fwrite("\nfullscreen = false", 1, 19, temp_file);
		}
		
		fwrite("\nworking_dir = \"", 1, 16, temp_file);
		char *temp_char = working_dir;
		while (*temp_char != '\0') {
			fwrite(temp_char, 1, 1, temp_file);
			temp_char++;
		}
		fwrite("\"", 1, 1, temp_file);
		
		char buffer[64];
		int number_of_chars;
		number_of_chars = snprintf(buffer, 64, "\nscanlines_alpha = %u", video_scanlines_alpha);
		fwrite(buffer, 1, number_of_chars, temp_file);
		
		if (video_linear_filtering) {
			fwrite("\nlinear_filtering = true", 1, 24, temp_file);
		} else {
			fwrite("\nlinear_filtering = false", 1, 25, temp_file);
		}
		
		fclose(temp_file);
	}
}

bool E64::settings_t::create_wav()
{
	chdir(settings_dir);
	
	wav_file = fopen("output.wav", "wb");
	
	if (!wav_file) return false;
	
	// write unfinished header to file
	fwrite((void *)&wav_header, 1, 44, wav_file);

	return true;
}

void E64::settings_t::finish_wav()
{
	long filesize;
	
	if (wav_file) {
		fseek(wav_file, 0L, SEEK_END);
		filesize = ftell(wav_file);
		
		// enforce little endian
		uint32_t sc2s = (uint32_t)(filesize - 44);
		wav_header.Subchunk2Size[0] = sc2s         & 0xff;
		wav_header.Subchunk2Size[1] = (sc2s >> 8)  & 0xff;
		wav_header.Subchunk2Size[2] = (sc2s >> 16) & 0xff;
		wav_header.Subchunk2Size[3] = (sc2s >> 24) & 0xff;
		
		// enforce little endian
		uint32_t cs = sc2s + 36;
		wav_header.ChunkSize[0] = cs         & 0xff;
		wav_header.ChunkSize[1] = (cs >> 8)  & 0xff;
		wav_header.ChunkSize[2] = (cs >> 16) & 0xff;
		wav_header.ChunkSize[3] = (cs >> 24) & 0xff;
		
		printf("[Settings] wav chunksize is: %i\n", cs);
		printf("[Settings] wav subchunk2size is: %i\n", sc2s);
		
		// write correct/final header
		fseek(wav_file, 0L, SEEK_SET);
		fwrite((void *)&wav_header, 1, 44, wav_file);
		
		fclose(wav_file);
	}
}

void E64::settings_t::audio_toggle_recording()
{
	if (!audio_recording) {
		audio_start_recording();
	} else {
		audio_stop_recording();
	}
}

void E64::settings_t::audio_start_recording()
{
	audio_recording = true;
	audio_clear_record_buffer();
	create_wav();
	//hud.show_notification("start recording sound");
}

void E64::settings_t::audio_stop_recording()
{
	audio_recording = false;
	finish_wav();
	//hud.show_notification("stop recording sound");
}

void E64::settings_t::audio_clear_record_buffer()
{
	audio_record_buffer_head = audio_record_buffer_tail = 0;
}
