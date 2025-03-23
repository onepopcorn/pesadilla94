#ifndef _TEXT_H
#define _TEXT_H

// Set langauge TODO: Allow compile for language with a make variable
#define LANG_ES 1
#define LANG_EN 2

#define LANG LANG_ES

#define STR_BRAND "@ONEPOPCORN 2025\0"
#define STR_BRAND_LEN 16

// Use a buffer big enough to hold the biggest of the strings in any language
#define STR_MAX_TEXT_BUFFER 80

// ---------------------- SPANISH ---------------------- //
#if LANG == LANG_ES

#define STR_INTRO_L1 "FEDERICO! NO HAS ESTUDIADO PARA LOS EXAMENES FINALES!\0"
#define STR_INTRO_L2 "ES TARDE, SE TE CAEN LOS       PARPADOS Y YA NO AGUANTAS MAS\0"

#define STR_MENU_L1 "PULSA DISPARO PARA EMPEZAR\0"
#define STR_MENU_L1_LEN 26

#define STR_HUD_SCORE "PUNTOS\0"
#define STR_HUD_SCORE_LEN 6

#define STR_LEVEL_END "COMPLETADO!\0"
#define STR_LEVEL_END_LEN 11

// ---------------------- ENGLISH ---------------------- //
#elif LANG == LANG_EN

#define STR_INTRO_L1 "FEDERICO! YOU DID NOT STUDY FOR THE FINALS!\0"
#define STR_INTRO_L2 "IT'S LATE, YOU ARE NODDING OFF\0"

#define STR_MENU_L1 "PRESS FIRE TO START\0"
#define STR_MENU_L1_LEN 19

#define STR_HUD_SCORE "SCORE\0"
#define STR_HUD_SCORE_LEN 5

#define STR_LEVEL_END "DONE!\0"
#define STR_LEVEL_END_LEN 5

#endif

#endif