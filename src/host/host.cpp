
#include "host.hpp"
#include "common.hpp"
#include <thread>
#include <chrono>

E64::host_t::host_t()
{
	printf("E64 version %i.%i.%i (C)2019-%i elmerucr\n",
	       E64_MAJOR_VERSION,
	       E64_MINOR_VERSION,
	       E64_BUILD, E64_YEAR);
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	/*
	 * Each call to SDL_PollEvent invokes SDL_PumpEvents() that
	 * updates this array.
	 */
	events_sdl2_keyboard_state = SDL_GetKeyboardState(NULL);
	
	SDL_version compiled;
	SDL_VERSION(&compiled);
	printf("[SDL] Compiled against SDL version %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
	
	SDL_version linked;
	SDL_GetVersion(&linked);
	printf("[SDL] Linked against SDL version %d.%d.%d\n", linked.major, linked.minor, linked.patch);

	char *base_path = SDL_GetBasePath();
	printf("[SDL] Base path is: %s\n", base_path);
	SDL_free(base_path);

	sdl_preference_path = SDL_GetPrefPath("elmerucr", "E64");
	printf("[SDL] Preference path is: %s\n", sdl_preference_path);
	
	audio_init();
	video_init();
}

E64::host_t::~host_t()
{
	video_stop();
	audio_stop();
	SDL_Quit();
}

void E64::host_t::audio_init()
{
	/*
	 * Print the list of audio backends
	 */
	int numAudioDrivers = SDL_GetNumAudioDrivers();
	printf("[SDL] audio backend(s): %d compiled into SDL: ", numAudioDrivers);
	for (int i=0; i<numAudioDrivers; i++) {
		printf(" \'%s\' ", SDL_GetAudioDriver(i));
	}
	printf("\n");
	
	// What's this all about???
	SDL_zero(audio_spec_want);
	
	/*
	 * Define audio specification
	 */
	audio_spec_want.freq = SAMPLE_RATE;
	audio_spec_want.format = AUDIO_F32SYS;
	audio_spec_want.channels = 2;
	audio_spec_want.samples = 512;
	audio_spec_want.callback = nullptr;
	
	/*
	 * Open audio device, allowing any changes to the specification
	 */
	audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec_want, &audio_spec_have,
						 SDL_AUDIO_ALLOW_ANY_CHANGE);
	if(!audio_device) {
		printf("[SDL] failed to open audio device: %s\n", SDL_GetError());
		// this is not enough and even wrong...
		// consider a system without audio support?
		SDL_Quit();
	}
	
	printf("[SDL] audio now using backend '%s'\n", SDL_GetCurrentAudioDriver());
	printf("[SDL] audio information:        want\thave\n");
	printf("[SDL]         frequency         %d\t%d\n", audio_spec_want.freq, audio_spec_have.freq);
	printf("[SDL]         format\n"
	       "[SDL]          float            %s\t%s\n",
	       SDL_AUDIO_ISFLOAT(audio_spec_want.format) ? "yes" : "no",
	       SDL_AUDIO_ISFLOAT(audio_spec_have.format) ? "yes" : "no");
	printf("[SDL]          signed           %s\t%s\n",
	       SDL_AUDIO_ISSIGNED(audio_spec_want.format) ? "yes" : "no",
	       SDL_AUDIO_ISSIGNED(audio_spec_have.format) ? "yes" : "no");
	printf("[SDL]          big endian       %s\t%s\n",
	       SDL_AUDIO_ISBIGENDIAN(audio_spec_want.format) ? "yes" : "no",
	       SDL_AUDIO_ISBIGENDIAN(audio_spec_have.format) ? "yes" : "no");
	printf("[SDL]          bitsize          %d\t%d\n",
	       SDL_AUDIO_BITSIZE(audio_spec_want.format),
	       SDL_AUDIO_BITSIZE(audio_spec_have.format));
	printf("[SDL]          channels         %d\t%d\n", audio_spec_want.channels, audio_spec_have.channels);
	printf("[SDL]          samples          %d\t%d\n", audio_spec_want.samples, audio_spec_have.samples);
	
	audio_bytes_per_sample = SDL_AUDIO_BITSIZE(audio_spec_have.format) / 8;
	printf("[SDL] audio is using %d bytes per sample per channel\n", audio_bytes_per_sample);
	
	audio_bytes_per_ms = (double)SAMPLE_RATE * audio_spec_have.channels * audio_bytes_per_sample / 1000;
	printf("[SDL] audio is using %f bytes per ms\n", audio_bytes_per_ms);
	
	audio_running = false;
	
	audio_start();
}

void E64::host_t::audio_start()
{
	if (!audio_running) {
		printf("[SDL] start audio\n");
		// Unpause audiodevice, and process audiostream
		SDL_PauseAudioDevice(audio_device, 0);
		audio_running = true;
	}
}

void E64::host_t::audio_stop()
{
	if (audio_running) {
		printf("[SDL] stop audio\n");
		// Pause audiodevice
		SDL_PauseAudioDevice(audio_device, 1);
		audio_running = false;
	}
}

void E64::host_t::video_init()
{
	/*
	 * Print the list of video backends
	 */
	int num_video_drivers = SDL_GetNumVideoDrivers();
	printf("[SDL] Display %d video backend(s) compiled into SDL: ",
	       num_video_drivers);
	for (int i=0; i<num_video_drivers; i++)
		printf(" \'%s\' ", SDL_GetVideoDriver(i));
	printf("\n[SDL] Display now using backend '%s'\n", SDL_GetCurrentVideoDriver());

	/*
	 * TODO: add config option to lua?
	 */
	current_window_size = 4;
	
	/*
	 * Start with windowed screen
	 */
	fullscreen = false;
	
	/*
	 * Create window - title will be set later on by update_title()
	 * Note: Usage of SDL_WINDOW_ALLOW_HIGHDPI actually helps: interpolation
	 * of pixels at unlogical window sizes looks a lot better!
	 */
	video_window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED,
				  SDL_WINDOWPOS_CENTERED,
				  video_window_sizes[current_window_size].x,
				  video_window_sizes[current_window_size].y,
				  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE |
				  SDL_WINDOW_ALLOW_HIGHDPI);
    
	SDL_GetWindowSize(video_window, &window_width, &window_height);
	printf("[SDL] Display window dimension: %u x %u pixels\n",
	       window_width, window_height);
	
	update_title();

	SDL_DisplayMode current_mode;

	SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(video_window), &current_mode);

	printf("[SDL] Display current desktop dimension: %i x %i\n",
	       current_mode.w, current_mode.h);

	printf("[SDL] Display refresh rate of current display is %iHz\n",
	       current_mode.refresh_rate);
	
	/*
	 * Create renderer and link it to window
	 */
    
	if (current_mode.refresh_rate == FPS) {
		printf("[SDL] Display: this is equal to the FPS of E64, trying for vsync\n");
		renderer = SDL_CreateRenderer(video_window, -1,
					      SDL_RENDERER_ACCELERATED |
					      SDL_RENDERER_TARGETTEXTURE |
					      SDL_RENDERER_PRESENTVSYNC);
	} else {
		printf("[SDL] Display: this differs from the FPS of E64, going for software FPS\n");
		renderer = SDL_CreateRenderer(video_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	}
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);	// black used for clearing actions
	
	SDL_RendererInfo current_renderer;
	SDL_GetRendererInfo(renderer, &current_renderer);
	vsync = (current_renderer.flags & SDL_RENDERER_PRESENTVSYNC) ? true : false;

	printf("[SDL] Renderer Name: %s\n", current_renderer.name);
	printf("[SDL] Renderer %saccelerated\n",
	       (current_renderer.flags & SDL_RENDERER_ACCELERATED) ? "" : "not ");
	printf("[SDL] Renderer vsync is %s\n", vsync ? "enabled" : "disabled");
	
	/*
	 * Create two textures that are able to refresh very frequently
	 */
	vm_texture = nullptr;
	hud_texture = nullptr;
	create_vm_texture(linear_filtering);
	create_hud_texture(linear_filtering);
	
	/*
	 * Scanlines: A static texture that mimics scanlines
	 */
	scanline_buffer = new uint16_t[4 * VM_MAX_PIXELS_PER_SCANLINE * VM_MAX_SCANLINES];
	scanlines_texture = nullptr;
	create_scanlines_texture(scanlines_linear_filtering);

	/*
	 * Make sure mouse cursor isn't visible
	 */
	SDL_ShowCursor(SDL_DISABLE);
}

void E64::host_t::video_stop()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(video_window);
}

void E64::host_t::update_title()
{
//	if (machine.mode == E64::PAUSED) {
//		SDL_SetWindowTitle(window, "E64 Debug Mode");
//		// TODO: ?
//		//SDL_SetWindowIcon(SDL_Window *window, SDL_Surface *icon);
//	} else {
		SDL_SetWindowTitle(video_window, "E64");
//		// TODO: ?
//		//SDL_SetWindowIcon(SDL_Window *window, SDL_Surface *icon);
//	}
}

void E64::host_t::update_textures(E64::blitter_ic *vm_b, E64::blitter_ic *hud_b)
{
	SDL_UpdateTexture(vm_texture, NULL, vm_b->fb, VM_MAX_PIXELS_PER_SCANLINE * sizeof(uint16_t));
	SDL_UpdateTexture(hud_texture, NULL, hud_b->fb, VM_MAX_PIXELS_PER_SCANLINE * sizeof(uint16_t));
}

void E64::host_t::update_screen(E64::blitter_ic *vm_b)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, vm_texture, &vm_b->screen_size, NULL);
	SDL_SetTextureAlphaMod(scanlines_texture, video_scanlines_alpha);
	SDL_RenderCopy(renderer, scanlines_texture, &vm_b->scanline_screen_size, NULL);

	SDL_RenderCopy(renderer, hud_texture, NULL, NULL);

	SDL_RenderPresent(renderer);
}

void E64::host_t::create_vm_texture(bool linear_filtering)
{
	if (vm_texture) SDL_DestroyTexture(vm_texture);
	
	if (linear_filtering) {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	} else {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	}
	
	vm_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB4444,
				    SDL_TEXTUREACCESS_STREAMING,
				    VM_MAX_PIXELS_PER_SCANLINE, VM_MAX_SCANLINES);
	SDL_SetTextureBlendMode(vm_texture, SDL_BLENDMODE_NONE);
}

void E64::host_t::create_hud_texture(bool linear_filtering)
{
	if (hud_texture) SDL_DestroyTexture(hud_texture);
	
	if (linear_filtering) {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	} else {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	}

	hud_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB4444,
				    SDL_TEXTUREACCESS_STREAMING,
				    VM_MAX_PIXELS_PER_SCANLINE, VM_MAX_SCANLINES);
	SDL_SetTextureBlendMode(hud_texture, SDL_BLENDMODE_BLEND);
}

void E64::host_t::create_scanlines_texture(bool linear_filtering)
{
	if (scanlines_texture) SDL_DestroyTexture(scanlines_texture);

	if (linear_filtering) {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	} else {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	}
	
	scanlines_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB4444,
				    SDL_TEXTUREACCESS_STATIC,
				    VM_MAX_PIXELS_PER_SCANLINE, 4 * VM_MAX_SCANLINES);
	SDL_SetTextureBlendMode(scanlines_texture, SDL_BLENDMODE_BLEND);
	
	for (int i=0; i<4*VM_MAX_SCANLINES; i++) {
		for (int j=0; j < VM_MAX_PIXELS_PER_SCANLINE; j++) {
			uint16_t color;
			switch (i & 0b11) {
				case 0b00: color = 0xf000; break;
				case 0b01: color = 0x0000; break;
				case 0b10: color = 0x0000; break;
				case 0b11: color = 0xf000; break;
				default:   color = 0x0000;
			};
			scanline_buffer[(i * VM_MAX_PIXELS_PER_SCANLINE) + j] = color;
		}
	}
	SDL_UpdateTexture(scanlines_texture, NULL, scanline_buffer,
		VM_MAX_PIXELS_PER_SCANLINE * sizeof(uint16_t));
}

enum E64::events_output_state E64::host_t::events_process_events()
{
	enum events_output_state return_value = NO_EVENT;
	
	SDL_Event event;
	
	//bool shift_pressed = E64_sdl2_keyboard_state[SDL_SCANCODE_LSHIFT] | E64_sdl2_keyboard_state[SDL_SCANCODE_RSHIFT];
	bool alt_pressed   = events_sdl2_keyboard_state[SDL_SCANCODE_LALT] | events_sdl2_keyboard_state[SDL_SCANCODE_RALT];
	//bool gui_pressed   = E64_sdl2_keyboard_state[SDL_SCANCODE_LGUI] | E64_sdl2_keyboard_state[SDL_SCANCODE_RGUI];
	
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
				return_value = KEYPRESS_EVENT;          // default at keydown, may change to QUIT_EVENT
				if( (event.key.keysym.sym == SDLK_f) && alt_pressed ) {
					events_wait_until_f_released();
					video_toggle_fullscreen();
				} else if( (event.key.keysym.sym == SDLK_s) && alt_pressed ) {
					video_change_scanlines_intensity();
				} else if ((event.key.keysym.sym == SDLK_b) && alt_pressed) {
					video_toggle_linear_filtering();
				} else if( (event.key.keysym.sym == SDLK_q) && alt_pressed ) {
					events_wait_until_q_released();
					return_value = QUIT_EVENT;
				} else if ((event.key.keysym.sym == SDLK_MINUS) && alt_pressed) {
					// decrease screen size
					events_wait_until_minus_released();
					video_decrease_window_size();
				} else if ((event.key.keysym.sym == SDLK_EQUALS) && alt_pressed) {
					// decrease screen size
					events_wait_until_equals_released();
					video_increase_window_size();
				} else if(event.key.keysym.sym == SDLK_F10) {
					hud->toggle_stats();
				}
				break;
			case SDL_QUIT:
				return_value = QUIT_EVENT;
				break;
		}
	}
	if (return_value == QUIT_EVENT) printf("[SDL] detected quit event\n");
	return return_value;
}

void E64::host_t::events_wait_until_f_released()
{
	SDL_Event event;
	bool wait = true;
	while(wait) {
	    SDL_PollEvent(&event);
	    if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_f)) wait = false;
	    std::this_thread::sleep_for(std::chrono::microseconds(40000));
	}
} 

void E64::host_t::events_wait_until_q_released()
{
	SDL_Event event;
	bool wait = true;
	while(wait) {
	    SDL_PollEvent(&event);
	    if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_q)) wait = false;
	    std::this_thread::sleep_for(std::chrono::microseconds(40000));
	}
}

void E64::host_t::events_wait_until_minus_released()
{
	SDL_Event event;
	bool wait = true;
	while(wait) {
	    SDL_PollEvent(&event);
	    if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_MINUS)) wait = false;
	    std::this_thread::sleep_for(std::chrono::microseconds(40000));
	}
}

void E64::host_t::events_wait_until_equals_released()
{
	SDL_Event event;
	bool wait = true;
	while(wait) {
	    SDL_PollEvent(&event);
	    if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_EQUALS)) wait = false;
	    std::this_thread::sleep_for(std::chrono::microseconds(40000));
	}
}

void E64::host_t::video_toggle_fullscreen()
{
	fullscreen = !fullscreen;
	if (fullscreen) {
		SDL_SetWindowFullscreen(video_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	} else {
		SDL_SetWindowFullscreen(video_window, SDL_WINDOW_RESIZABLE);
	}
	SDL_GetWindowSize(video_window, &window_width, &window_height);
//	hud.show_notification("Switched to %s mode with size %ix%i",
//			      fullscreen ? "fullscreen" : "window",
//			      window_width,
//			      window_height);
}

void E64::host_t::video_change_scanlines_intensity()
{
	if (video_scanlines_alpha < 64) {
		video_scanlines_alpha = 64;
	} else if (video_scanlines_alpha < 128) {
		video_scanlines_alpha = 128;
	} else if (video_scanlines_alpha < 192) {
		video_scanlines_alpha = 192;
	} else if (video_scanlines_alpha < 255) {
		video_scanlines_alpha = 255;
	} else {
		video_scanlines_alpha = 0;
	}
	//hud.show_notification("                 scanlines alpha = %3u/255", scanlines_alpha);
}

void E64::host_t::video_toggle_linear_filtering()
{
	linear_filtering = !linear_filtering;
	create_vm_texture(linear_filtering);
	create_hud_texture(linear_filtering);
	//hud.show_notification("                   vm linear filtering = %s", vm_linear_filtering ? "on" : "off");
}

void E64::host_t::video_increase_window_size()
{
	if (current_window_size < 8) current_window_size++;
	SDL_SetWindowSize(video_window, video_window_sizes[current_window_size].x,
			  video_window_sizes[current_window_size].y);
	SDL_GetWindowSize(video_window, &window_width, &window_height);
	SDL_SetWindowPosition(video_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	//hud.show_notification("Set host window size to %ix%i", window_width, window_height);
}

void E64::host_t::video_decrease_window_size()
{
	if (current_window_size > 0) current_window_size--;
	SDL_SetWindowSize(video_window, video_window_sizes[current_window_size].x,
			  video_window_sizes[current_window_size].y);
	SDL_GetWindowSize(video_window, &window_width, &window_height);
	SDL_SetWindowPosition(video_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	//hud.show_notification("Set host window size to %ix%i", window_width, window_height);
}
