#ifndef HOST_HPP
#define HOST_HPP

#include <SDL2/SDL.h>
#include "blitter.hpp"
#include "hud.hpp"
#include "settings.hpp"

namespace E64
{

struct video_window_size {
	uint16_t x;
	uint16_t y;
};

enum events_output_state {
	QUIT_EVENT = -1,
	NO_EVENT = 0,
	KEYPRESS_EVENT = 1
};

class host_t {
private:
	hud_t *hud{nullptr};
	settings_t *settings{nullptr};
	
	/*
	 * Audio related
	 */
	SDL_AudioDeviceID audio_device;
	SDL_AudioSpec audio_spec_want;
	SDL_AudioSpec audio_spec_have;
	double audio_bytes_per_ms;
	uint8_t audio_bytes_per_sample;
	bool audio_running{false};
	void audio_init();
	void audio_start();
	void audio_stop();
	
	/*
	 * Video related
	 */
	const struct video_window_size video_window_sizes[9] = {
		{  384, 240 },
		{  480, 300 },
		{  512, 320 },
		{  640, 400 },
		{  768, 480 },
		{  960, 600 },
		{ 1024, 640 },
		{ 1152, 720 },
		{ 1280, 800 }
	};
	
	SDL_Window *video_window;
	SDL_Renderer *renderer;
	bool vsync;
	SDL_Texture *vm_texture;
	SDL_Texture *hud_texture;
	SDL_Texture *scanlines_texture;
	uint8_t current_window_size;
	int window_width;
	int window_height;
	
	uint16_t *scanline_buffer;
	
	void create_vm_texture(bool linear_filtering);
	void create_hud_texture(bool linear_filtering);
	void create_scanlines_texture(bool linear_filtering);
	void create_textures();
	
	//bool linear_filtering{false};
	bool scanlines_linear_filtering{true}; // should always be true
	
	void video_init();
	void video_stop();
	
	/*
	 * events related
	 */
	void events_wait_until_f_released();
	void events_wait_until_q_released();
	void events_wait_until_minus_released();
	void events_wait_until_equals_released();
public:
	host_t(settings_t *s);
	~host_t();
	
	char *sdl_preference_path;
	
	/*
	 * Audio related
	 */
	inline void queue_audio(void *buffer, unsigned size) { SDL_QueueAudio(audio_device, buffer, size); }
	inline unsigned int get_queued_audio_size_bytes() { return SDL_GetQueuedAudioSize(audio_device); }
	
	/*
	 * Video related
	 */
	void update_textures(E64::blitter_ic *vm_b, E64::blitter_ic *hud_b);
	void update_screen(blitter_ic *b);
	void update_title();
	void video_increase_window_size();
	void video_decrease_window_size();
	void video_toggle_fullscreen();
	void video_change_scanlines_intensity();
	void change_scanlines_intensity();
	void video_toggle_linear_filtering();
    
	// getters setters
	uint16_t current_window_width() { return video_window_sizes[current_window_size].x; }
	uint16_t current_window_height() { return video_window_sizes[current_window_size].y; }
	inline bool vsync_enabled() { return vsync; }
	inline bool vsync_disabled() { return !vsync; }
	inline uint8_t get_bytes_per_sample() { return audio_bytes_per_sample; }
	inline double get_bytes_per_ms() { return audio_bytes_per_ms; }
	
	//inline uint8_t get_scanlines_alpha() { return video_scanlines_alpha; }
	//inline void set_scanline_alpha(uint8_t a) { video_scanlines_alpha = a; }
	//inline bool is_using_vm_linear_filtering() { return linear_filtering; }
	//inline bool is_using_hud_linear_filtering() { return hud_linear_filtering; }
	inline bool is_using_scanlines_linear_filtering() { return scanlines_linear_filtering; }
	//inline bool is_fullscreen() { return fullscreen; }
	inline void set_hud(hud_t *h) { hud = h; }
	
	/*
	 * Events related
	 */
	const uint8_t *events_sdl2_keyboard_state;
	enum events_output_state events_process_events();
};

}

#endif
