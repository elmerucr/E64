/*
 * keyboard.hpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#include "keyboard.hpp"
#include "common.hpp"
#include <iostream>

#define SHIFT_PRESSED   0b00000001
#define CTRL_PRESSED    0b00000010
//#define ALT_PRESSED     0b00000100

inline char event_to_ascii(uint8_t scancode, uint8_t modifiers)
{
	using namespace E64;
	
	switch (scancode) {
		case SCANCODE_ESCAPE: return ASCII_ESCAPE;
		case SCANCODE_F1:     return ASCII_F1;
		case SCANCODE_F2:     return ASCII_F2;
		case SCANCODE_F3:     return ASCII_F3;
		case SCANCODE_F4:     return ASCII_F4;
		case SCANCODE_F5:     return ASCII_F5;
		case SCANCODE_F6:     return ASCII_F6;
		case SCANCODE_F7:     return ASCII_F7;
		case SCANCODE_F8:     return ASCII_F8;
		case SCANCODE_GRAVE:  return (modifiers & SHIFT_PRESSED) ? ASCII_TILDE : ASCII_GRAVE;
		case SCANCODE_1:      return (modifiers & SHIFT_PRESSED) ? ASCII_EXCL_MARK : ASCII_1;
		case SCANCODE_2:      return (modifiers & SHIFT_PRESSED) ? ASCII_AT : ASCII_2;
		case SCANCODE_3:      return (modifiers & SHIFT_PRESSED) ? ASCII_NUMBER : ASCII_3;
		case SCANCODE_4:      return (modifiers & SHIFT_PRESSED) ? ASCII_DOLLAR : ASCII_4;
		case SCANCODE_5:      return (modifiers & SHIFT_PRESSED) ? ASCII_PERCENT : ASCII_5;
		case SCANCODE_6:      return (modifiers & SHIFT_PRESSED) ? ASCII_CARET : ASCII_6;
		case SCANCODE_7:      return (modifiers & SHIFT_PRESSED) ? ASCII_AMPERSAND : ASCII_7;
		case SCANCODE_8:      return (modifiers & SHIFT_PRESSED) ? ASCII_ASTERISK : ASCII_8;
		case SCANCODE_9:      return (modifiers & SHIFT_PRESSED) ? ASCII_OPEN_PAR : ((modifiers & CTRL_PRESSED) ? ASCII_REVERSE_ON : ASCII_9);
		case SCANCODE_0:      return (modifiers & SHIFT_PRESSED) ? ASCII_CLOSE_PAR : ((modifiers & CTRL_PRESSED) ? ASCII_REVERSE_OFF : ASCII_0);
		case SCANCODE_MINUS:  return (modifiers & SHIFT_PRESSED) ? ASCII_UNDERSCORE : ASCII_HYPHEN;
		case SCANCODE_EQUALS: return (modifiers & SHIFT_PRESSED) ? ASCII_PLUS : ASCII_EQUALS;
		case SCANCODE_BACKSPACE: return ASCII_BACKSPACE;
		case SCANCODE_TAB:    return ASCII_SPACE;	// TODO: temp hack to make things work
		case SCANCODE_Q:      return (modifiers & SHIFT_PRESSED) ? ASCII_Q : ASCII_q;
		case SCANCODE_W:      return (modifiers & SHIFT_PRESSED) ? ASCII_W : ASCII_w;
		case SCANCODE_E:      return (modifiers & SHIFT_PRESSED) ? ASCII_E : ASCII_e;
		case SCANCODE_R:      return (modifiers & SHIFT_PRESSED) ? ASCII_R : ASCII_r;
		case SCANCODE_T:      return (modifiers & SHIFT_PRESSED) ? ASCII_T : ASCII_t;
		case SCANCODE_Y:      return (modifiers & SHIFT_PRESSED) ? ASCII_Y : ASCII_y;
		case SCANCODE_U:      return (modifiers & SHIFT_PRESSED) ? ASCII_U : ASCII_u;
		case SCANCODE_I:      return (modifiers & SHIFT_PRESSED) ? ASCII_I : ASCII_i;
		case SCANCODE_O:      return (modifiers & SHIFT_PRESSED) ? ASCII_O : ASCII_o;
		case SCANCODE_P:      return (modifiers & SHIFT_PRESSED) ? ASCII_P : ASCII_p;
		case SCANCODE_LEFTBRACKET: return (modifiers & SHIFT_PRESSED) ? ASCII_OPEN_BRACE : ASCII_OPEN_BRACK;
		case SCANCODE_RIGHTBRACKET: return (modifiers & SHIFT_PRESSED) ? ASCII_CLOSE_BRACE : ASCII_CLOSE_BRACK;
		case SCANCODE_RETURN: return ASCII_LF;
		case SCANCODE_A:      return (modifiers & SHIFT_PRESSED) ? ASCII_A : ASCII_a;
		case SCANCODE_S:      return (modifiers & SHIFT_PRESSED) ? ASCII_S : ASCII_s;
		case SCANCODE_D:      return (modifiers & SHIFT_PRESSED) ? ASCII_D : ASCII_d;
		case SCANCODE_F:      return (modifiers & SHIFT_PRESSED) ? ASCII_F : ASCII_f;
		case SCANCODE_G:      return (modifiers & SHIFT_PRESSED) ? ASCII_G : ASCII_g;
		case SCANCODE_H:      return (modifiers & SHIFT_PRESSED) ? ASCII_H : ASCII_h;
		case SCANCODE_J:      return (modifiers & SHIFT_PRESSED) ? ASCII_J : ASCII_j;
		case SCANCODE_K:      return (modifiers & SHIFT_PRESSED) ? ASCII_K : ASCII_k;
		case SCANCODE_L:      return (modifiers & SHIFT_PRESSED) ? ASCII_L : ASCII_l;
		case SCANCODE_SEMICOLON: return (modifiers & SHIFT_PRESSED) ? ASCII_COLON : ASCII_SEMI_COLON;
		case SCANCODE_APOSTROPHE: return (modifiers & SHIFT_PRESSED) ? ASCII_DOUBLE_QUOTES : ASCII_SINGLE_QUOTE;
		case SCANCODE_BACKSLASH: return (modifiers & SHIFT_PRESSED) ? ASCII_VERT_BAR : ASCII_BACKSLASH;
		case SCANCODE_Z:      return (modifiers & SHIFT_PRESSED) ? ASCII_Z : ASCII_z;
		case SCANCODE_X:      return (modifiers & SHIFT_PRESSED) ? ASCII_X : ASCII_x;
		case SCANCODE_C:      return (modifiers & SHIFT_PRESSED) ? ASCII_C : ASCII_c;
		case SCANCODE_V:      return (modifiers & SHIFT_PRESSED) ? ASCII_V : ASCII_v;
		case SCANCODE_B:      return (modifiers & SHIFT_PRESSED) ? ASCII_B : ASCII_b;
		case SCANCODE_N:      return (modifiers & SHIFT_PRESSED) ? ASCII_N : ASCII_n;
		case SCANCODE_M:      return (modifiers & SHIFT_PRESSED) ? ASCII_M : ASCII_m;
		case SCANCODE_COMMA:  return (modifiers & SHIFT_PRESSED) ? ASCII_LESS : ASCII_COMMA;
		case SCANCODE_PERIOD: return (modifiers & SHIFT_PRESSED) ? ASCII_GREATER : ASCII_PERIOD;
		case SCANCODE_SLASH:  return (modifiers & SHIFT_PRESSED) ? ASCII_QUESTION_M : ASCII_SLASH;
		case SCANCODE_SPACE:  return ASCII_SPACE;
		case SCANCODE_LEFT:   return ASCII_CURSOR_LEFT;
		case SCANCODE_UP:     return ASCII_CURSOR_UP;
		case SCANCODE_DOWN:   return ASCII_CURSOR_DOWN;
		case SCANCODE_RIGHT:  return ASCII_CURSOR_RIGHT;
	}
	return ASCII_NULL;
}

bool scancode_not_modifier[] =
{
    true,                       // 0x00
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true,                       // 0x08
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true,                       // 0x10
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true,                       // 0x18
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true,                       // 0x20
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true,                       // 0x28
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true,                       // 0x30
    true,
    false,                      // left shift
    true,
    true,
    true,
    true,
    true,
    true,                       // 0x38
    true,
    true,
    true,
    true,
    false,                      // right shift
    false,                      // left control
    //false,                      // left alt
    true,
    //false,                      // right alt
    false,                      // right control
    true,
    true,
    true,
    true
};

E64::keyboard_t::keyboard_t(host_t *h)
{
	host = h;
	
	repeat_delay_ms = 500;
	repeat_speed_ms = 50;
	
	reset();
}

void E64::keyboard_t::process()
{
       uint8_t modifier_keys_status =
		((host->keyboard_state[SCANCODE_LSHIFT] & 0b1) ? SHIFT_PRESSED : 0) |
		((host->keyboard_state[SCANCODE_RSHIFT] & 0b1) ? SHIFT_PRESSED : 0) |
		((host->keyboard_state[SCANCODE_LCTRL ] & 0b1) ? CTRL_PRESSED  : 0) |
		((host->keyboard_state[SCANCODE_RCTRL ] & 0b1) ? CTRL_PRESSED  : 0) ;
	
	for (int i=0; i<128; i++) {
		switch (host->keyboard_state[i] & 0b11) {
			case 0b01:
				// event key down
				if (generate_events && scancode_not_modifier[i]) {
					key_down = true;
					last_char = i;
					time_to_next = repeat_delay_ms * 1000;
					push_event(event_to_ascii(last_char, modifier_keys_status)); // immediate
				}
				break;
			case 0b10:
				// event key up
				if (generate_events) {
					if (i == last_char) {
						key_down = false;
					}
				}
			default:
				// do nothing
				break;
		}
	}

	microseconds_remaining = microseconds_per_frame;
	
	if (key_down) {
		while (microseconds_remaining) {
			if (time_to_next < microseconds_remaining) {
				microseconds_remaining -= time_to_next;
				time_to_next = repeat_speed_ms * 1000;
				// TODO: see old E64 here... avoid two call to push_event!
				push_event(event_to_ascii(last_char, modifier_keys_status));
			} else {
				time_to_next -= microseconds_remaining;
				microseconds_remaining = 0;
			}
		}
	}
}

void E64::keyboard_t::push_event(uint8_t event)
{
	event_list[head] = event;
	
	head++;
	
	if (head == tail) {
		tail++;
	}
}

uint8_t E64::keyboard_t::pop_event()
{
	uint8_t result;
	if( head == tail ) {
		// no events in queue / stack return empty scancode
		result = E64::SCANCODE_EMPTY;
	} else {
		result = event_list[tail];
		tail++;
	}
	return result;
}

void E64::keyboard_t::reset()
{
	head = tail = 0;
}
