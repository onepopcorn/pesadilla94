#ifndef _KEYB_H
#define _KEYB_H

#define IRS_KEYBOARD 0x09

extern volatile uint8_t keys[0xff];

void keyboardInit(void);
void keyboardFree(void);

// https://kbdlayout.info/kbdusx/scancodes+virtualkeys
#define KEY_ESC 0x01
#define KEY_TAB 0x10
#define KEY_ENTER 0x1C
#define KEY_SPACE 0x39

#define KEY_A 0x1E
#define KEY_S 0x1F
#define KEY_Z 0x2C
#define KEY_X 0x2D
#define KEY_P 0x19

#define KEY_UP 0x48
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D
#define KEY_DOWN 0x50

// #define KEY_F1    0x3B
// #define KEY_F2    0x3C
// #define KEY_F3    0x3D
// #define KEY_F4    0x3E
// #define KEY_F5    0x3F
// #define KEY_F6    0x40
// #define KEY_F7    0x41
// #define KEY_F8    0x42
// #define KEY_F9    0x43
// #define KEY_F10   0x44
// #define KEY_F11   0x57
// #define KEY_F12   0x58

#endif