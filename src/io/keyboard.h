#ifndef _KEYB_H
#define _KEYB_H

#define IRS_KEYBOARD 0x09

extern volatile uint8_t keys[0xff];
#define m_isKeyDown(k) keys[k]

uint8_t isKeyJustPressed(uint8_t key);

void keyboardInit(void);
void keyboardFree(void);

// https://kbdlayout.info/kbdusx/scancodes+virtualkeys
#define KEY_ESC 0x01
#define KEY_ENTER 0x1C
#define KEY_SPACE 0x39
#define KEY_CTRL_L 0x1D
#define KEY_P 0x19

#define KEY_S 0x1F
#define KEY_K 0x25
#define KEY_W 0x11

#define KEY_UP 0x48
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D
#define KEY_DOWN 0x50

#endif  // _KEYB_H