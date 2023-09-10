/*
 * settings.cpp
 * E64
 *
 * Copyright © 2021-2023 elmerucr. All rights reserved.
 */

#include "settings.hpp"
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include "common.hpp"

E64::settings_t::settings_t(host_t *h)
{
	host = h;
	
	snprintf(home_dir, 256, "%s", getenv("HOME"));
	printf("[Settings] User home directory: %s\n", home_dir);
	
	settings_dir = host->sdl_preference_path;
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
		fullscreen_at_init = lua_toboolean(L, -1);
	} else {
		fullscreen_at_init = false;
	}
	
	lua_getglobal(L, "working_dir");
	if (lua_isstring(L, -1)) {
		strcpy(working_dir, lua_tolstring(L, -1, nullptr));
		printf("[Settings] Working dir set to: %s\n", working_dir);
	} else {
		working_dir[0] = '\0';
	}
	
	lua_getglobal(L, "vm_linear_filtering");
	if (lua_isboolean(L, -1)) {
		vm_linear_filtering_at_init = lua_toboolean(L, -1);
	} else {
		vm_linear_filtering_at_init = false;
	}
	
	lua_getglobal(L, "hud_linear_filtering");
	if (lua_isboolean(L, -1)) {
		hud_linear_filtering_at_init = lua_toboolean(L, -1);
	} else {
		hud_linear_filtering_at_init = false;
	}
	
	lua_getglobal(L, "scanlines_alpha");
	if (lua_isinteger(L, -1)) {
		scanlines_alpha_at_init = (lua_tointeger(L, -1)) & 0xff;
	} else {
		scanlines_alpha_at_init = 192;
	}
	
	scanlines_linear_filtering_at_init = true;	// always
	
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
}

E64::settings_t::~settings_t()
{
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

		if (host->is_fullscreen()) {
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
		number_of_chars = snprintf(buffer, 64, "\nscanlines_alpha = %u", host->get_scanlines_alpha());
		fwrite(buffer, 1, number_of_chars, temp_file);
		
		if (host->is_using_vm_linear_filtering()) {
			fwrite("\nvm_linear_filtering = true", 1, 27, temp_file);
		} else {
			fwrite("\nvm_linear_filtering = false", 1, 28, temp_file);
		}
		
		if (host->is_using_hud_linear_filtering()) {
			fwrite("\nhud_linear_filtering = true", 1, 28, temp_file);
		} else {
			fwrite("\nhud_linear_filtering = false", 1, 29, temp_file);
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
