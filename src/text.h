#ifndef _TEXT_H
#define _TEXT_H

// Set langauge TODO: Allow compile for language with a make variable
#define LANG_ES 1
#define LANG_EN 2

#ifndef LANG
#define LANG LANG_ES
#endif

#define STR_BRAND "@ONEPOPCORN 2025\0"
#define STR_BRAND_LEN 16

// Use a buffer big enough to hold the biggest of the strings in any language
#define STR_MAX_TEXT_BUFFER 80

// ---------------------- SPANISH ---------------------- //
#if LANG == LANG_ES

#define STR_NAME "PESADILLA DEL '94"

#define STR_AUTHOR "[@onepopcorn 2025]"

#define STR_INSTRUCTIONS "CONTROLES:\n- Usa las flechas IZQUIERDA o DERECHA para mover al personaje.\n- Manten ARRIBA para buscar en las puertas o maquinas de vending.\n- Pulsar ARRIBA or ABAJO para moverte entre plantas con las escaleras.\n- Pulsa ESPACIO para disparar cuando el arma esta cargada.\n\n\nPulsa cualquier tecla para empezar."

#define STR_INTRO_L1 "FEDERICO! NO HAS ESTUDIADO PARA LOS EXAMENES FINALES!\0"
#define STR_INTRO_L2 "ES TARDE, SE TE CAEN LOS PARPADOS Y YA NO AGUANTAS MAS\0"

#define STR_MENU_L1 "PULSA ESPACIO\0"
#define STR_MENU_L1_LEN 13

#define STR_MENU_L2 "PARA EMPEZAR\0"
#define STR_MENU_L2_LEN 12

#define STR_HUD_SCORE "PUNTOS\0"
#define STR_HUD_SCORE_LEN 6

#define STR_HUD_LEVEL "NIVEL-"

#define STR_LEVEL_END "COMPLETADO!\0"
#define STR_LEVEL_END_LEN 11

#define STR_GAMEOVER "SE ACABO!\0"

#define STR_ENDING "FELICIDADES!             HAS CONSEGUIDO RECOGER   TODAS LAS RESPUESTAS DE  LOS EXAMENES PERO...     ESTABAS DURMIENDO. EN LA VIDA REAL SIGUES SIENDO  UN ESTUDIANTE DE MIERDA  Y NO TE QUEDA OTRA QUE   IR A LA RECUPERACION DE  SEPTIEMBRE.\0"
#define STR_ENDING2 "AHORA APAGA EL ORDENADOR Y PONTE A ESTUDIAR.\0"
// ---------------------- ENGLISH ---------------------- //
#elif LANG == LANG_EN

#define STR_NAME "Nightmare of '94\0"

#define STR_AUTHOR "[@onepopcorn 2025]"

#define STR_INSTRUCTIONS "CONTROLS:\n- Use LEFT or RIGHT arrow keys to move the player.\n- Hold UP to search behind doors or inside vending machines.\n- Press UP or DOWN to move between floors using the stairs.\n- Press SPACE to shoot when the weapon is ready.\n\n\nPress any key to start."

#define STR_INTRO_L1 "FEDERICO! YOU DID NOT STUDY FOR THE FINALS!\0"
#define STR_INTRO_L2 "IT'S LATE, YOU ARE NODDING OFF\0"

#define STR_MENU_L1 "PRESS SPACE\0"
#define STR_MENU_L1_LEN 11

#define STR_MENU_L2 "TO START\0"
#define STR_MENU_L2_LEN 8

#define STR_HUD_SCORE "SCORE\0"
#define STR_HUD_SCORE_LEN 5

#define STR_HUD_LEVEL "LEVEL-"

#define STR_LEVEL_END "DONE!\0"
#define STR_LEVEL_END_LEN 5

#define STR_GAMEOVER "GAME OVER!\0"

#define STR_ENDING "CONGRATULATIONS! YOU HAVE GOT ALL THE ANSWERS OF    THE EXAMS BUT... JUST     IN YOUR DREAMS. IN THE    REAL LIVE YOU STILL       ARE A SHITTY STUDENT      AND YOU'LL HAVE TO        RETAKE IT DURING          SUMMER BREAK.\0"
#define STR_ENDING2 "NOW TURN OFF THE PC AND  GET TO STUDY.\0"

#endif

#endif