#ifndef ROM_HPP
#define ROM_HPP

#include <cstdint>

#define	CBM_CP437_FONT_ELEMENTS		2048	// 0b00
#define AMIGA_CP437_FONT_ELEMENTS	4096	// 0b01
#define	IBM_CP437_FONT_ELEMENTS		2048	// 0b10
#define IBM_VGA_8X16_FONT_ELEMENTS	4096	// 0b11

extern uint8_t cbm_cp437_font[];
extern uint8_t amiga_cp437_font[];
extern uint8_t ibm_cp437_font[];
extern uint8_t ibm_vga_8x16_font[];

#endif
