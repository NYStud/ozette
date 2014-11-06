#ifndef CONTROL_H
#define CONTROL_H

#include <stddef.h>

namespace Control {
static const unsigned Count = 32;
struct Info {
	char mnemonic;
	char label[10];
};
extern Info keys[Count];

enum {
	// Dedicated keys for editor manipulation
	Backspace = 0x08, // BS ^H
	Tab = 0x09, // HT ^I
	Enter = 0x0A, //LF ^J
	Return = 0x0D, //CR ^M
	Escape = 0x1B, // ESC ^[

	Cut = 0x18, //CAN ^X
	Copy = 0x03, //ETX ^C
	Paste = 0x16, //SYN ^V
	Delete = 0x0B, //VT ^K
	Undo = 0x1A, //SUB ^Z
	Redo = 0x19, //EM ^Y

	Open = 0x0F, //SI ^O
	Close = 0x17, //ETB ^W
	NewFile = 0x0E, // SO ^N
	Save = 0x13, //DC3 ^S
	Revert = 0x12, //DC2 ^R

	Find = 0x06, //ACK ^F
	GoTo = 0x07, //BEL ^G

	Quit = 0x11, //DC1 ^Q

	// Ideas for the future
	// redo = ?
	// R = run, B = build, K = clean?
	// edit, diff, log, blame?
	// P = projects?
};

struct Panel {
	static const size_t width = 6;
	static const size_t height = 2;
	char label[height][width];
};

} // namespace Control

#endif	//CONTROL_H