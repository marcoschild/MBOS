c

#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

void init_paging();

// 2 functions are defined in paging.asm
extern void load_page_directory(uint32_t);
extern void enable_paging();

#endif

