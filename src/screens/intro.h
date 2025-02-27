#ifndef _INTRO_SCREEN_H
#define _INTRO_SCREEN_H

enum Sequence {
    SEQ_DRAW_BRANDING,
    SEQ_BRANDING,
    SEQ_DRAW_INTRO,
    SEQ_WAIT_INTRO,
    SEQ_INTRO,
    SEQ_OUTRO,
    SEQ_END,
};

enum Screen intro();

#endif  // _INTRO_SCREEN_H