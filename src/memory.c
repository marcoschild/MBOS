#include <stdint.h>
#include "memory.h"

#define KERNEL_HEAP_START 0x1000000 // 16 MB
#define KERNEL_HEAP_END 0x2000000  // 32 MB

static uint32_t heap_pointer = KERNEL_HEAP_START;

void * kmalloc(uint32_t size) {
    if (heap_pointer + size > KERNEL_HEAP_END) {
        return 0; // out of memory 
    }

    void* block = (void*)heap_pointer;
    heap_pointer += size;
    return block;
}

void kfree(void* ptr) {
    // Not implemeented (bump allocators can't free)
}