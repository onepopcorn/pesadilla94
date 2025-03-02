#ifndef _DEBUG_LOG_H
#define _DEBUG_LOG_H

void logError(const char *format, ...);
void logDebug(const char *format, ...);

#ifdef DEBUG_ENTITIES
void showEntityId(short x, short y, uint8_t id);
#endif

#endif  // _DEBUG_LOG_H