#ifndef BUCKET_LIB_H
#define BUCKET_LIB_H

/**
 * ----------
 * - COMMON -
 * ----------
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/**
 * ----------------------
 * - PLATFORM DETECTION -
 * ----------------------
 */
#if defined(_WIN32) || defined(_WIN64)
#define BUCKET_LIB_WIN 1
#elif defined(__DJGPP__)
#define BUCKET_LIB_DJGPP 1
#elif defined(__unix__) || defined(__APPLE__)
#define BUCKET_LIB_UNIX 1
#endif

/**
 * ----------------------------------
 * - PLATFORM SPECIFIC DEPENDENCIES -
 * ----------------------------------
 */
#ifdef BUCKET_LIB_WIN
#include <windows.h>
#endif
#ifdef BUCKET_LIB_UNIX
#include <sys/mman.h>
#endif
#ifdef BUCKET_LIB_DJGPP
#include <stdlib.h>
#endif

/**
 * ----------------------------------
 * - ARENA ALLOCATOR - Declarations -
 * ----------------------------------
 */

#define KiB(x) ((x) * (1ULL << 10))
#define MiB(x) ((x) * (1ULL << 20))
#define GiB(x) ((x) * (1ULL << 30))

typedef struct Arena {
    uint8_t* base;  // pointer to memory block
    size_t size;    // total size
    size_t used;    // bytes used
} Arena;

typedef enum {
    ARENA_ALIGN_4 = 4,
    ARENA_ALIGN_8 = 8,
    ARENA_ALIGN_16 = 16,
    ARENA_ALIGN_PTR = sizeof(void*),

} ArenaAlign;

/**
 * Create a new arena allocator with given
 * size in bytes.
 *
 * If size can't be allocated, returned arena
 * will have size 0 and nothing can be allocated
 * into it.
 *
 */
Arena arena_create(size_t size);

/**
 * Completely destroy arena allocator and free
 * its memory. Base pointer is also nullified
 * to prevent "use after free".
 *
 */
void arena_destroy(Arena* arena);

/**
 * Allocate given number of bytes using given
 * allocator instance.
 *
 * It aligns to platform pointer size (i.e. sizeof(void*))
 * by default
 *
 * If OOM, allocator will return NULL pointer. If not,
 * allocator will return a memory pointer.
 *
 */
void* arena_alloc(Arena* arena, size_t size);

/**
 * Allocate given number of bytes using given
 * allocator instance and align.
 *
 * Same as arena_alloc but alignment can be
 * specified explicitly
 *
 * If OOM, allocator will return NULL pointer. If not,
 * allocator will return a memory pointer.
 *
 */
void* arena_alloc_align(Arena* arena, size_t size, ArenaAlign align);

/**
 * Get current "marker" to reset to.
 *
 * This marker can be passed to arena_reset to
 * reset the used marker to this marker.
 * This, effectively "frees" memory from the arena so it
 * can be reused for something else.
 *
 */
static inline size_t arena_get_marker(Arena* arena) {
    return arena->used;
}

/**
 * Frees memory on the arena allocator to given
 * "marker" (number of bytes).
 *
 * If marker is 0, the whole arena is reset and
 * can be reused for other things without needing
 * any "free" (e.g. arena_destroy) call.
 *
 */
static inline void arena_reset(Arena* arena, size_t marker) {
    arena->used = marker;
}

/**
 * -------------------------
 * - STRING - Declarations -
 * -------------------------
 */
typedef struct StringView {
    size_t length;
    char* buffer;
} StringView;

/**
 * Macro to create string views literals at compile time
 *
 */
#define STRING_VIEW_LITERAL(str) {.buffer = (str), .length = sizeof(str) - 1}

/**
 * Create a string view for given cstring
 *
 */
static inline StringView string_view_create(char* text) {
    return (StringView){
        .length = strlen(text),
        .buffer = text,
    };
}

/**
 * Create an empty string of given length to be used
 * later
 *
 */
static inline StringView string_view_from_buffer(char* buffer, size_t length) {
    return (StringView){
        .length = length,
        .buffer = buffer,
    };
}

/**
 *
 *
 */
StringView* string_from_int(Arena* arena, int number);

/**
 * ------------------------
 * - ARRAY - Declarations -
 * ------------------------
 */

/* TODO */

/* --- IMPLEMENTATIONS --- */
#ifdef BUCKET_LIB_IMPLEMENTATION

/**
 * ------------------------------------
 * - ARENA ALLOCATOR - Implementation -
 * ------------------------------------
 */

Arena arena_create(size_t size) {
    Arena arena = {0};
#ifdef BUCKET_LIB_WIN
    arena.base = (uint8_t*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
#elif BUCKET_LIB_UNIX
    arena.base = (uint8_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#else
    arena.base = (uint8_t*)malloc(size);
#endif

    arena.size = arena.base ? size : 0;
    arena.used = 0;
    return arena;
}

void arena_destroy(Arena* arena) {
    if (arena->base) {
#ifdef BUCKET_LIB_WIN
        VirtualFree(arena->base, 0, MEM_RELEASE);
#elif BUCKET_LIB_UNIX
        munmap(arena->base, arena->size);
#else
        free(arena->base);
#endif
        arena->base = NULL;
    }

    arena->size = 0;
    arena->used = 0;
}

void* arena_alloc(Arena* arena, size_t size) {
    size_t mask = sizeof(void*) - 1;
    size_t offset = (arena->used + mask) & ~mask;

    if (offset > arena->size - size) {
        // out of memory
        return NULL;
    }

    void* ptr = arena->base + offset;
    arena->used = offset + size;
    return ptr;
}

void* arena_alloc_align(Arena* arena, size_t size, ArenaAlign align) {
    size_t mask = align - 1;
    size_t offset = (arena->used + mask) & ~mask;

    if (offset > arena->size - size) {
        // out of memory
        return NULL;
    }

    void* ptr = arena->base + offset;
    arena->used = offset + size;
    return ptr;
}

/**
 * ---------------------------
 * - STRING - Implementation -
 * ---------------------------
 */

inline StringView string_create(char* text) {
    return (StringView){
        .length = strlen(text),
        .buffer = text,
    };
}
/* TODO */

/**
 * --------------------------
 * - ARRAY - Implementation -
 * --------------------------
 */

/* TODO */

#endif  // BUCKET_LIB_IMPLEMENTATION

#endif  // BUCKET_LIB_H