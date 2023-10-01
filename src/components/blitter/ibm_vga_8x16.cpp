// The original xxxxxxx characters are copyright xxxxxxxxxxxx
//
// 2023-10-01 elmerucr
//
// Part of E64 - Character ROM 4kb

#include <cstdint>

uint8_t ibm_vga_8x16_font[4096] = {
	0b00000000,		// $00 (space)
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	
	0b00000000,	// $01 (smiley)
	0b00000000,
	0b00000000,
	0b00000000,
	0b01111110,
	0b10000001,
	0b10100101,
	0b10100101,
	0b10000001,
	0b10100101,
	0b10100101,
	0b10011001,
	0b10000001,
	0b01111110,
	0b00000000,
	0b00000000,
	
	0b00000000,	// $02 (smiley invert)
	0b00000000,
	0b00000000,
	0b00000000,
	0b01111110,
	0b11111111,
	0b11011011,
	0b11011011,
	0b11111111,
	0b11011011,
	0b11011011,
	0b11100111,
	0b11111111,
	0b01111110,
	0b00000000,
	0b00000000,
	
	0b00110110,	// $03 (hearts)
	0b00110110,
	0b01111111,
	0b01111111,
	0b01111111,
	0b01111111,
	0b01111111,
	0b01111111,
	0b00111110,
	0b00111110,
	0b00011100,
	0b00011100,
	0b00001000,
	0b00001000,
	0b00000000,
	0b00000000,
	
	0b00001000,	// $04 (diamonds)
	0b00001000,
	0b00011100,
	0b00011100,
	0b00111110,
	0b00111110,
	0b01111111,
	0b01111111,
	0b00111110,
	0b00111110,
	0b00011100,
	0b00011100,
	0b00001000,
	0b00001000,
	0b00000000,
	0b00000000,
	
	0b00011000,	// $05 (clubs)
	0b00011000,
	0b00011000,
	0b00011000,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00111100,
	0b00111100,
	0b00000000,
	0b00000000,
	
	0b00001000,	// $06 (spades)
	0b00001000,
	0b00011100,
	0b00011100,
	0b00111110,
	0b00111110,
	0b01111111,
	0b01111111,
	0b01111111,
	0b01111111,
	0b00011100,
	0b00011100,
	0b00111110,
	0b00111110,
	0b00000000,
	0b00000000,
	
	0b00000000,	// $07 (ball)
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00111100,
	0b00111100,
	0b00111100,
	0b00111100,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b11111111,	// $08 (reverse ball)
	0b11111111,
	0b11111111,
	0b11111111,
	0b11100111,
	0b11100111,
	0b11000011,
	0b11000011,
	0b11000011,
	0b11000011,
	0b11100111,
	0b11100111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	
	0b00000000,	// $09 (circle)
	0b00000000,
	0b00111100,
	0b00111100,
	0b01100110,
	0b01100110,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01100110,
	0b01100110,
	0b00111100,
	0b00111100,
	0b00000000,
	0b00000000,
	
	0b11111111,	// $0A (reverse circle)
	0b11111111,
	0b11000011,
	0b11000011,
	0b10011001,
	0b10011001,
	0b10111101,
	0b10111101,
	0b10111101,
	0b10111101,
	0b10011001,
	0b10011001,
	0b11000011,
	0b11000011,
	0b11111111,
	0b11111111,
	
	0b00000000,	// $0B
	0b00000000,
	0b00001111,
	0b00000111,
	0b00000111,
	0b00001101,
	0b00111100,
	0b00111100,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b00111100,
	0b00111100,
	0b00000000,
	0b00000000,
	
	0b00000000,	// $0C
	0b00000000,
	0b00111100,
	0b00111100,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b00111100,
	0b00111100,
	0b00011000,
	0b00011000,
	0b01111110,
	0b00011000,
	0b00011000,
	0b00000000,
	
	0b00011111,	// $0D
	0b00011111,
	0b00011000,
	0b00011000,
	0b00011111,
	0b00011111,
	0b00011000,
	0b00011000,
	0b00111000,
	0b00111000,
	0b01111000,
	0b01111000,
	0b00111000,
	0b00111000,
	0b00000000,
	0b00000000,
	
	0b01111111,	// $0E
	0b01111111,
	0b01100011,
	0b01100011,
	0b01111111,
	0b01111111,
	0b01100011,
	0b01100011,
	0b01100011,
	0b01100111,
	0b01100111,
	0b11100111,
	0b11100011,
	0b11100000,
	0b01100000,
	0b00000000,
	
	0b10011001,	// $0F
	0b10011001,
	0b01011010,
	0b01011010,
	0b00111100,
	0b00111100,
	0b11100111,
	0b11100111,
	0b11100111,
	0b11100111,
	0b00111100,
	0b00111100,
	0b01011010,
	0b01011010,
	0b10011001,
	0b10011001,
	
	0b01000000,	// $10
	0b01100000,
	0b01110000,
	0b01111000,
	0b01111100,
	0b01111110,
	0b01111111,
	0b01111111,
	0b01111110,
	0b01111100,
	0b01111000,
	0b01110000,
	0b01100000,
	0b01000000,
	0b00000000,
	0b00000000,
	
	0b00000001,	// $11
	0b00000011,
	0b00000111,
	0b00001111,
	0b00011111,
	0b00111111,
	0b01111111,
	0b01111111,
	0b00111111,
	0b00011111,
	0b00001111,
	0b00000111,
	0b00000011,
	0b00000001,
	0b00000000,
	0b00000000,
	
	0b00011000,	// $12
	0b00011000,
	0b00111100,
	0b00111100,
	0b01111110,
	0b01111110,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b01111110,
	0b01111110,
	0b00111100,
	0b00111100,
	0b00011000,
	0b00011000,
	
	0b01100110,	// $13
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b00000000,
	0b00000000,
	0b01100110,
	0b01100110,
	0b00000000,
	0b00000000,
	
	0b01111111,	// $14
	0b01111111,
	0b11011011,
	0b11011011,
	0b11011011,
	0b11011011,
	0b01111011,
	0b01111011,
	0b00011011,
	0b00011011,
	0b00011011,
	0b00011011,
	0b00011011,
	0b00011011,
	0b00000000,
	0b00000000,
	
	0b00111100,	// $15 §
	0b00111100,
	0b01000000,
	0b01000000,
	0b00111100,
	0b00111100,
	0b01100110,
	0b01100110,
	0b00111100,
	0b00111100,
	0b00000010,
	0b00000010,
	0b00111100,
	0b00111100,
	0b00000000,
	0b00000000,

	0b00000000,	// $16
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01111110,
	0b01111110,
	0b01111110,
	0b01111110,
	0b01111110,
	0b01111110,
	0b00000000,
	0b00000000,

	0b00011000,	// $17
	0b00011000,
	0b00111100,
	0b00111100,
	0b01111110,
	0b01111110,
	0b00011000,
	0b00011000,
	0b01111110,
	0b01111110,
	0b00111100,
	0b00111100,
	0b00011000,
	0b00011000,
	0b11111111,
	0b11111111,
	
	0b00011000,	// $18 (arrow up)
	0b00011000,
	0b00111100,
	0b00111100,
	0b01111110,
	0b01111110,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,

	0b00011000,	// $19 (arrow down)
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b01111110,
	0b01111110,
	0b00111100,
	0b00111100,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,

	0b00000000,	// $1A (arrow right)
	0b00000000,
	0b00000100,
	0b00000100,
	0b00000110,
	0b00000110,
	0b01111111,
	0b01111111,
	0b01111111,
	0b01111111,
	0b00000110,
	0b00000110,
	0b00000100,
	0b00000100,
	0b00000000,
	0b00000000,
	
	0b00000000,	// $1B (arrow left)
	0b00000000,
	0b00010000,
	0b00010000,
	0b00110000,
	0b00110000,
	0b01111111,
	0b01111111,
	0b01111111,
	0b01111111,
	0b00110000,
	0b00110000,
	0b00010000,
	0b00010000,
	0b00000000,
	0b00000000,
	
	0b00000000,	// $1C
	0b00000000,
	0b00000000,
	0b00000000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b01111111,
	0b01111111,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,	// $1D
	0b00000000,
	0b00010100,
	0b00010100,
	0b00110110,
	0b00110110,
	0b01111111,
	0b01111111,
	0b01111111,
	0b01111111,
	0b00110110,
	0b00110110,
	0b00010100,
	0b00010100,
	0b00000000,
	0b00000000,

	0b00000000,	// $1E
	0b00000000,
	0b00011000,
	0b00011000,
	0b00111100,
	0b00111100,
	0b01111110,
	0b01111110,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b00000000,
	0b00000000,
	
	0b00000000,	// $1F
	0b00000000,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b01111110,
	0b01111110,
	0b00111100,
	0b00111100,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $20 space
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00011000,	// $21 !
	0b00011000,
	0b00111100,
	0b00111100,
	0b00111100,
	0b00111100,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	
	0b01101100,	// $22 "
	0b01101100,
	0b01101100,
	0b01101100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b01100110,	// $23 #
	0b01100110,
	0b01100110,
	0b01100110,
	0b11111111,
	0b11111111,
	0b01100110,
	0b01100110,
	0b11111111,
	0b11111111,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b00000000,
	0b00000000,
	
	0b00011000,	// $24 $
	0b00011000,
	0b00111110,
	0b00111110,
	0b01100000,
	0b01100000,
	0b00111100,
	0b00111100,
	0b00000110,
	0b00000110,
	0b01111100,
	0b01111100,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	
	0b00000000,	// $25 %
	0b00000000,
	0b11000110,
	0b11000110,
	0b11001100,
	0b11001100,
	0b00011000,
	0b00011000,
	0b00110000,
	0b00110000,
	0b01100110,
	0b01100110,
	0b11000110,
	0b11000110,
	0b00000000,
	0b00000000,
	
	0b00111000,	// $26 &
	0b00111000,
	0b01101100,
	0b01101100,
	0b01101000,
	0b01101000,
	0b01110110,
	0b01110110,
	0b11011100,
	0b11011100,
	0b11001100,
	0b11001100,
	0b01110110,
	0b01110110,
	0b00000000,
	0b00000000,
	
	0b00011000,	// $27 '
	0b00011000,
	0b00011000,
	0b00011000,
	0b00110000,
	0b00110000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $28 (
	0b00000000,
	0b00001100,
	0b00011000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00011000,
	0b00001100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $29 )
	0b00000000,
	0b00110000,
	0b00011000,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00011000,
	0b00110000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $2A *
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01100110,
	0b00111100,
	0b11111111,
	0b00111100,
	0b01100110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $2B +
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b01111110,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $2C ,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00110000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $2D -
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11111110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $2E .
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000011,	// $2F /
	0b00000011,
	0b00000110,
	0b00000110,
	0b00001100,
	0b00001100,
	0b00011000,
	0b00011000,
	0b00110000,
	0b00110000,
	0b01100000,
	0b01100000,
	0b11000000,
	0b11000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $30 0
	0b00000000,
	0b00111000,
	0b01101100,
	0b11000110,
	0b11000110,
	0b11010110,
	0b11010110,
	0b11000110,
	0b11000110,
	0b01101100,
	0b00111000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $31 1
	0b00000000,
	0b00011000,
	0b00111000,
	0b01111000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b01111110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $32 2
	0b00000000,
	0b01111100,
	0b11000110,
	0b00000110,
	0b00001100,
	0b00011000,
	0b00110000,
	0b01100000,
	0b11000000,
	0b11000110,
	0b11111110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $33 3
	0b00000000,
	0b01111100,
	0b11000110,
	0b00000110,
	0b00000110,
	0b00111100,
	0b00000110,
	0b00000110,
	0b00000110,
	0b11000110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $34 4
	0b00000000,
	0b00001100,
	0b00011100,
	0b00111100,
	0b01101100,
	0b11001100,
	0b11111110,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00011110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $35 5
	0b00000000,
	0b11111110,
	0b11000000,
	0b11000000,
	0b11000000,
	0b11111100,
	0b00000110,
	0b00000110,
	0b00000110,
	0b11000110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $36 6
	0b00000000,
	0b00111000,
	0b01100000,
	0b11000000,
	0b11000000,
	0b11111100,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $37 7
	0b00000000,
	0b11111110,
	0b11000110,
	0b00000110,
	0b00000110,
	0b00001100,
	0b00011000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $38 8
	0b00000000,
	0b01111100,
	0b11000110,
	0b11000110,
	0b11000110,
	0b01111100,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $39 9
	0b00000000,
	0b01111100,
	0b11000110,
	0b11000110,
	0b11000110,
	0b01111110,
	0b00000110,
	0b00000110,
	0b00000110,
	0b00001100,
	0b01111000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $3A :
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $3B ;
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00110000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $3C <
	0b00000000,
	0b00000000,
	0b00000110,
	0b00001100,
	0b00011000,
	0b00110000,
	0b01100000,
	0b00110000,
	0b00011000,
	0b00001100,
	0b00000110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $3D =
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01111110,
	0b00000000,
	0b00000000,
	0b01111110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $3E >
	0b00000000,
	0b00000000,
	0b01100000,
	0b00110000,
	0b00011000,
	0b00001100,
	0b00000110,
	0b00001100,
	0b00011000,
	0b00110000,
	0b01100000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $3F ?
	0b00000000,
	0b01111100,
	0b11000110,
	0b11000110,
	0b00001100,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $40 @
	0b00000000,
	0b00000000,
	0b01111100,
	0b11000110,
	0b11000110,
	0b11011110,
	0b11011110,
	0b11011110,
	0b11011100,
	0b11000000,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $41 A
	0b00000000,
	0b00010000,
	0b00111000,
	0b01101100,
	0b11000110,
	0b11000110,
	0b11111110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $42 B
	0b00000000,
	0b11111100,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01111100,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b11111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $43 C
	0b00000000,
	0b00111100,
	0b01100110,
	0b11000010,
	0b11000000,
	0b11000000,
	0b11000000,
	0b11000000,
	0b11000010,
	0b01100110,
	0b00111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $44 D
	0b00000000,
	0b11111000,
	0b01101100,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01101100,
	0b11111000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $45 E
	0b00000000,
	0b11111110,
	0b01100110,
	0b01100010,
	0b01101000,
	0b01111000,
	0b01101000,
	0b01100000,
	0b01100010,
	0b01100110,
	0b11111110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $46 F
	0b00000000,
	0b11111110,
	0b01100110,
	0b01100010,
	0b01101000,
	0b01111000,
	0b01101000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b11110000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $47 G
	0b00000000,
	0b00111100,
	0b01100110,
	0b11000010,
	0b11000000,
	0b11000000,
	0b11011110,
	0b11000110,
	0b11000110,
	0b01100110,
	0b00111010,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $48 H
	0b00000000,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11111110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $49 I
	0b00000000,
	0b00111100,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $4A J
	0b00000000,
	0b00011110,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b11001100,
	0b11001100,
	0b11001100,
	0b01111000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $4B K
	0b00000000,
	0b11100110,
	0b01100110,
	0b01100110,
	0b01101100,
	0b01111000,
	0b01111000,
	0b01101100,
	0b01100110,
	0b01100110,
	0b11100110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $4C L
	0b00000000,
	0b11110000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b01100010,
	0b01100110,
	0b11111110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $4D M
	0b00000000,
	0b11000110,
	0b11101110,
	0b11111110,
	0b11111110,
	0b11010110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $4E N
	0b00000000,
	0b11000110,
	0b11100110,
	0b11110110,
	0b11111110,
	0b11011110,
	0b11001110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $4F O
	0b00000000,
	0b01111100,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $50 P
	0b00000000,
	0b11111100,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01111100,
	0b01100000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b11110000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $51 Q
	0b00000000,
	0b01111100,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11010110,
	0b11011110,
	0b01111100,
	0b00001100,
	0b00001110,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $52 R
	0b00000000,
	0b11111100,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01111100,
	0b01101100,
	0b01100110,
	0b01100110,
	0b01100110,
	0b11100110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $53 S
	0b00000000,
	0b01111100,
	0b11000110,
	0b11000110,
	0b01100000,
	0b00111000,
	0b00001100,
	0b00000110,
	0b00000110,
	0b11000110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $54 T
	0b00000000,
	0b01111110,
	0b01111110,
	0b01011010,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $55 U
	0b00000000,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $56 V
	0b00000000,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b01101100,
	0b00111000,
	0b00010000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $57 W
	0b00000000,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11010110,
	0b11010110,
	0b11010110,
	0b11111110,
	0b11101110,
	0b01101100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $58 X
	0b00000000,
	0b11000110,
	0b11000110,
	0b01101100,
	0b01101100,
	0b00111000,
	0b00111000,
	0b01101100,
	0b01101100,
	0b11000110,
	0b11000110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $59 Y
	0b00000000,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b00111100,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $5A Z
	0b00000000,
	0b11111110,
	0b11000110,
	0b10000110,
	0b00001100,
	0b00011000,
	0b00110000,
	0b01100000,
	0b11000010,
	0b11000110,
	0b11111110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00111100,	// $5B [
	0b00111100,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00111100,
	0b00111100,
	0b00000000,
	0b00000000,
	
	0b11000000,	// $5C backslash
	0b11000000,
	0b01100000,
	0b01100000,
	0b00110000,
	0b00110000,
	0b00011000,
	0b00011000,
	0b00001100,
	0b00001100,
	0b00000110,
	0b00000110,
	0b00000011,
	0b00000011,
	0b00000000,
	0b00000000,
	
	0b00111100,	// $5D ]
	0b00111100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00001100,
	0b00111100,
	0b00111100,
	0b00000000,
	0b00000000,
	
	0b00010000,	// $5E ^
	0b00010000,
	0b00111000,
	0b00111000,
	0b01101100,
	0b01101100,
	0b11000110,
	0b11000110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,	// $5F _
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01111110,
	0b01111110,
	
	0b00011000,	// $60 `
	0b00011000,
	0b00011000,
	0b00011000,
	0b00001100,
	0b00001100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $61 a
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01111000,
	0b00001100,
	0b01111100,
	0b11001100,
	0b11001100,
	0b11001100,
	0b01110110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $62 b
	0b00000000,
	0b11100000,
	0b01100000,
	0b01100000,
	0b01111000,
	0b01101100,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $63 c
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01111100,
	0b11000110,
	0b11000000,
	0b11000000,
	0b11000000,
	0b11000110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $64 d
	0b00000000,
	0b00011100,
	0b00001100,
	0b00001100,
	0b00111100,
	0b01101100,
	0b11001100,
	0b11001100,
	0b11001100,
	0b11001100,
	0b01110110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $65 e
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01111100,
	0b11000110,
	0b11111110,
	0b11000000,
	0b11000000,
	0b11000110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $66 f
	0b00000000,
	0b00111000,
	0b01101100,
	0b01100100,
	0b01100000,
	0b11110000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b01100000,
	0b11110000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $67 g
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01110110,
	0b11001100,
	0b11001100,
	0b11001100,
	0b11001100,
	0b11001100,
	0b01111100,
	0b00001100,
	0b11001100,
	0b01111000,
	0b00000000,
	
	0b00000000,		// $68 h
	0b00000000,
	0b11100000,
	0b01100000,
	0b01100000,
	0b01101100,
	0b01110110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b11100110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $69 i
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00111000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $6A j
	0b00000000,
	0b00000110,
	0b00000110,
	0b00000000,
	0b00001110,
	0b00000110,
	0b00000110,
	0b00000110,
	0b00000110,
	0b00000110,
	0b00000110,
	0b01100110,
	0b01100110,
	0b00111100,
	0b00000000,
	
	0b00000000,		// $6B k
	0b00000000,
	0b11100000,
	0b01100000,
	0b01100000,
	0b01100110,
	0b01101100,
	0b01111000,
	0b01111000,
	0b01101100,
	0b01100110,
	0b11100110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $6C l
	0b00000000,
	0b00111000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $6D m
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11101100,
	0b11111110,
	0b11010110,
	0b11010110,
	0b11010110,
	0b11010110,
	0b11000110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $6E n
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11011100,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $6F o
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01111100,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $70 p
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11011100,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01111100,
	0b01100000,
	0b01100000,
	0b11110000,
	0b00000000,
	
	0b00000000,		// $71 q
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01110110,
	0b11001100,
	0b11001100,
	0b11001100,
	0b11001100,
	0b11001100,
	0b01111100,
	0b00001100,
	0b00001100,
	0b00011110,
	0b00000000,
	
	0b00000000,		// $72 r
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11011100,
	0b01110110,
	0b01100110,
	0b01100000,
	0b01100000,
	0b01100000,
	0b11110000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $73 s
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01111100,
	0b11000110,
	0b01100000,
	0b00111000,
	0b00001100,
	0b11000110,
	0b01111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $74 t
	0b00000000,
	0b00010000,
	0b00110000,
	0b00110000,
	0b11111100,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110000,
	0b00110110,
	0b00011100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $75 u
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11001100,
	0b11001100,
	0b11001100,
	0b11001100,
	0b11001100,
	0b11001100,
	0b01110110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $76 v
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b01100110,
	0b00111100,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $77 w
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11000110,
	0b11000110,
	0b11010110,
	0b11010110,
	0b11010110,
	0b11111110,
	0b01101100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $78 x
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11000110,
	0b01101100,
	0b00111000,
	0b00111000,
	0b00111000,
	0b01101100,
	0b11000110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $79 y
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11000110,
	0b01111110,
	0b00000110,
	0b00001100,
	0b11111000,
	0b00000000,
	
	0b00000000,		// $7A z
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11111110,
	0b11001100,
	0b00011000,
	0b00110000,
	0b01100000,
	0b11000110,
	0b11111110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $7B {
	0b00000000,
	0b00001110,
	0b00011000,
	0b00011000,
	0b00011000,
	0b01110000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00001110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $7C |
	0b00000000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $7D }
	0b00000000,
	0b01110000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00001110,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b01110000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $7E ~
	0b00000000,
	0b01110110,
	0b11011100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	0b00000000,		// $7F
	0b00000000,
	0b00000000,
	0b00000000,
	0b00010000,
	0b00111000,
	0b01101100,
	0b11000110,
	0b11000110,
	0b11000110,
	0b11111110,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	
	// empty inverse (generated by blitter)
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
