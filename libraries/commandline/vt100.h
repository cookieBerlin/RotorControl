#ifndef VT100_H_
#define VT100_H_

// Special Character
#define ASCII_BEL				0x07
#define ASCII_BS				0x08
#define ASCII_TAB				0x09
#define ASCII_CR				0x0D
#define ASCII_LF				0x0A
#define ASCII_ESC				0x1B
#define ASCII_DEL				0x7F
#define ASCII_POS1				0x32
#define ASCII_END				0x35

// Arrow Keys
#define VT100_ARROWUP			'A'	// 41 Hex
#define VT100_ARROWDOWN			'B'	// 42 Hex
#define VT100_ARROWRIGHT		'C'	// 43 Hex
#define VT100_ARROWLEFT			'D'	// 44 Hex
#define VT100_POS1				'H'	// 48 Hex
#define VT100_END				'K'	// 4B Hex


// text attributes
#define VT100_ATTR_OFF			0
#define VT100_BOLD				1
#define VT100_USCORE			4
#define VT100_BLINK				5
#define VT100_REVERSE			7
#define VT100_BOLD_OFF			21
#define VT100_USCORE_OFF		24
#define VT100_BLINK_OFF			25
#define VT100_REVERSE_OFF		27

#endif /*VT100_H_ */
