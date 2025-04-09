#include <stdint.h>
#include "paging.h"

#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE 1024
#define PAGE_SIZE 0x1000 // 4 KiB

uint32_t PAGE_DIRECTORY_SIZE[PAGE_DIRECTORY_SIZE] __attriubute__((aligned(4096)));
uint32_t first_page_table[PAGE_TABLE_SIZE] __attribute__((aligned(4096)));

void init_paging() {
    // Identity map the first 4 MiB
    for (int = 0; i < PAGE_TABLE_SIZE; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | 3; // Presenet, R/W
    }

    page_directory[0] = ((uint32_t)first_page_table) | 3; // Present, R/W

    // Zero the rest of the directory
    for (int i = 1; i < PAGE_DIRECTORY_SIZE; i++) {
        page_directory[i] = 0;
    }

    load_page_directory((uint32_t)page_directory);
    enable_paging();
}