#include <stdio.h>
#include <stdarg.h>

#include "render/video.h"
#include "assets.h"

#include "logger.h"

void logError(const char *format, ...) {
    FILE *file = fopen("error.log", "a");
    if (!file) return;

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    fprintf(file, "\n");
    va_end(args);

    fclose(file);
}

void logDebug(const char *format, ...) {
    FILE *file = fopen("debug.log", "a");
    if (!file) return;

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    fprintf(file, "\n");
    va_end(args);

    fclose(file);
}

#ifdef DEBUG_ENTITIES
void showEntityId(short x, short y, uint8_t id) {
    char buffer[4] = {'0' + id, '\0'};
    drawText(x, y, font, buffer, COLOR_WHITE, 2);
}
#endif