#ifndef _HUD_H
#define _HUD_H

void writeMessage(char* message);

#ifdef DEBUG_ENTITIES
void writeNumber(short x, short y, uint8_t num);
#endif

#endif  // _HUD_H