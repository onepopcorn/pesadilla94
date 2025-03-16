#ifndef _CONTROLS_H
#define _CONTROLS_H

#define CONTROLS_NUM 10

enum ControlKey {
    CK_Left = 0,
    CK_Right = 1,
    CK_Up = 2,
    CK_Down = 3,
    CK_Shoot = 4,
    CK_Quit = 5,
    CK_Pause = 6,
};

#define m_LEFT Controls[CK_Left]
#define m_RIGHT Controls[CK_Right]
#define m_UP Controls[CK_Up]
#define m_DOWN Controls[CK_Down]
#define m_SHOOT Controls[CK_Shoot]
#define m_QUIT Controls[CK_Quit]
#define m_PAUSE Controls[CK_Pause]

extern uint8_t Controls[CONTROLS_NUM];

void defineKey(enum ControlKey controlKey, uint8_t key);

#endif  // _CONTROLS_H