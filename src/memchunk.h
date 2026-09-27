#ifndef MEMCHUNK_H
#include <stdint.h>
typedef struct memchunk {
    uint32_t size;
    void* pointer;
} memchunk;
#define MEMCHUNK_H
#endif
