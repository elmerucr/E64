
#include "host.hpp"
#include "common.hpp"

E64::host_t::host_t()
{
	printf("[Host] E64 version %i.%i.%i (C)2019-%i elmerucr\n",
	       E64_MAJOR_VERSION,
	       E64_MINOR_VERSION,
	       E64_BUILD, E64_YEAR);
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
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
	
	init_audio();
	init_video();
}

E64::host_t::~host_t()
{
	stop_video();
	stop_audio();
	SDL_Quit();
}

void E64::host_t::init_audio()
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
	SDL_zero(want);
	
	/*
	 * Define audio specification
	 */
	want.freq = SAMPLE_RATE;
	want.format = AUDIO_F32SYS;
	want.channels = 2;
	want.samples = 512;
	want.callback = nullptr;
	
	/*
	 * Open audio device, allowing any changes to the specification
	 */
	audio_dev = SDL_OpenAudioDevice(NULL, 0, &want, &have,
						 SDL_AUDIO_ALLOW_ANY_CHANGE);
	if(!audio_dev) {
		printf("[SDL] failed to open audio device: %s\n", SDL_GetError());
		// this is not enough and even wrong...
		// consider a system without audio support?
		SDL_Quit();
	}
	
	printf("[SDL] audio now using backend '%s'\n", SDL_GetCurrentAudioDriver());
	printf("[SDL] audio information:        want\thave\n");
	printf("[SDL]         frequency         %d\t%d\n", want.freq, have.freq);
	printf("[SDL]         format\n"
	       "[SDL]          float            %s\t%s\n",
	       SDL_AUDIO_ISFLOAT(want.format) ? "yes" : "no",
	       SDL_AUDIO_ISFLOAT(have.format) ? "yes" : "no");
	printf("[SDL]          signed           %s\t%s\n",
	       SDL_AUDIO_ISSIGNED(want.format) ? "yes" : "no",
	       SDL_AUDIO_ISSIGNED(have.format) ? "yes" : "no");
	printf("[SDL]          big endian       %s\t%s\n",
	       SDL_AUDIO_ISBIGENDIAN(want.format) ? "yes" : "no",
	       SDL_AUDIO_ISBIGENDIAN(have.format) ? "yes" : "no");
	printf("[SDL]          bitsize          %d\t%d\n",
	       SDL_AUDIO_BITSIZE(want.format),
	       SDL_AUDIO_BITSIZE(have.format));
	printf("[SDL]          channels         %d\t%d\n", want.channels, have.channels);
	printf("[SDL]          samples          %d\t%d\n", want.samples, have.samples);
	
	bytes_per_sample = SDL_AUDIO_BITSIZE(have.format) / 8;
	printf("[SDL] audio is using %d bytes per sample per channel\n", bytes_per_sample);
	
	bytes_per_ms = (double)SAMPLE_RATE * have.channels * bytes_per_sample / 1000;
	printf("[SDL] audio is using %f bytes per ms\n", bytes_per_ms);
	
	audio_running = false;
	
	start_audio();
}

void E64::host_t::start_audio()
{
	if (!audio_running) {
		printf("[SDL] start audio\n");
		// Unpause audiodevice, and process audiostream
		SDL_PauseAudioDevice(audio_dev, 0);
		audio_running = true;
	}
}

void E64::host_t::stop_audio()
{
	if (audio_running) {
		printf("[SDL] stop audio\n");
		// Pause audiodevice
		SDL_PauseAudioDevice(audio_dev, 1);
		audio_running = false;
	}
}

void E64::host_t::init_video()
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

	/* default starts at 3
	 * TODO: add config option to lua?
	 */
	current_window_size = 3;
	
	/*
	 * Start with windowed screen
	 */
	fullscreen = false;
	
	/*
	 * Create window - title will be set later on by update_title()
	 * Note: Usage of SDL_WINDOW_ALLOW_HIGHDPI actually helps: interpolation
	 * of pixels at unlogical window sizes looks a lot better!
	 */
	window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED,
				  SDL_WINDOWPOS_CENTERED,
				  window_sizes[current_window_size].x,
				  window_sizes[current_window_size].y,
				  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE |
				  SDL_WINDOW_ALLOW_HIGHDPI);
    
	SDL_GetWindowSize(window, &window_width, &window_height);
	printf("[SDL] Display window dimension: %u x %u pixels\n",
	       window_width, window_height);
	
	update_title();
	
//	/*
//	 * now change to former window/fullscreen setting
//	 */
//	if (host.settings->fullscreen_at_init) {
//		toggle_fullscreen();
//	}

	SDL_DisplayMode current_mode;

	SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(window), &current_mode);

	printf("[SDL] Display current desktop dimension: %i x %i\n",
	       current_mode.w, current_mode.h);

	printf("[SDL] Display refresh rate of current display is %iHz\n",
	       current_mode.refresh_rate);
	
	/*
	 * Create renderer and link it to window
	 */
    
	if (current_mode.refresh_rate == FPS) {
		printf("[SDL] Display: this is equal to the FPS of E64, trying for vsync\n");
		renderer = SDL_CreateRenderer(window, -1,
					      SDL_RENDERER_ACCELERATED |
					      SDL_RENDERER_TARGETTEXTURE |
					      SDL_RENDERER_PRESENTVSYNC);
	} else {
		printf("[SDL] Display: this differs from the FPS of E64, going for software FPS\n");
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
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
}

void E64::host_t::stop_video()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void E64::host_t::update_title()
{
//	if (machine.mode == E64::PAUSED) {
//		SDL_SetWindowTitle(window, "E64 Debug Mode");
//		// TODO: ?
//		//SDL_SetWindowIcon(SDL_Window *window, SDL_Surface *icon);
//	} else {
		SDL_SetWindowTitle(window, "E64");
//		// TODO: ?
//		//SDL_SetWindowIcon(SDL_Window *window, SDL_Surface *icon);
//	}
}

void E64::host_t::update_screen()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
//	SDL_RenderCopy(renderer, vm_texture, &machine.blitter->screen_size, NULL);
//	SDL_SetTextureAlphaMod(scanlines_texture, scanlines_alpha);
//	SDL_RenderCopy(renderer, scanlines_texture, &machine.blitter->scanline_screen_size, NULL);
//	
//	SDL_RenderCopy(renderer, hud_texture, NULL, NULL);

	SDL_RenderPresent(renderer);
}
