#ifndef _INTRO_SCREEN_H
#define _INTRO_SCREEN_H

enum Sequence {
    SEQ_DRAW_BRANDING,
    SEQ_DRAW_INTRO,
    SEQ_DRAW_INTRO2,
    SEQ_WAIT,
    SEQ_INTRO,
    SEQ_INTRO2,
    SEQ_END,
};

enum Screen intro();

#endif  // _INTRO_SCREEN_H