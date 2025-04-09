c

#include "paging.h"
#include <stdint.h>
#include "io.h"
#include "kernel.h"
#include "memory.h"
#include "keyboard.h"


// VGA text mode buffer
#define VIDEO_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* vga_buffer = (uint16_t*) VIDEO_ADDRESS;
static int cursor_row = 0;
static int cursor_col = 0;

// Basic VGA print with color
void put_char(char c, uint8_t color) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else {
        vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = (color << 8) | c;
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= VGA_HEIGHT) {
        for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
            vga_buffer[i] = (0x07 << 8) | ' ';
        }
        cursor_row = 0;
        cursor_col = 0;
    }
}

void print(const char* str) {
    while (*str) put_char(*str++, 0x0F); // White on black
}

void println(const char* str) {
    print(str);
    put_char('\n', 0x0F);
}

// Clear the screen
void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga_buffer[i] = (0x07 << 8) | ' ';
    cursor_row = 0;
    cursor_col = 0;
}

// Panic screen
void kernel_panic(const char* message) {
    clear_screen();
    print("KERNEL PANIC: ");
    println(message);
    while (1) __asm__ volatile ("hlt");
}

// Input (PS/2 keyboard)
char read_char() {
    while (!(inb(0x64) & 1)); // Wait for data
    return inb(0x60);
}

// Simple scancode-to-char (US QWERTY, minimal)
char scancode_to_char(uint8_t sc) {
    static char map[] = {
        0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
        'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
        'z','x','c','v','b','n','m',',','.','/',0,'*',0,' '
    };
    return (sc < sizeof(map)) ? map[sc] : 0;
}

// Command-mode input
void input_loop() {
    char buffer[128];
    int index = 0;

    print("> ");
    while (1) {
        uint8_t sc = read_char();
        char c = scancode_to_char(sc);

        if (c == '\n') {
            buffer[index] = 0;
            put_char('\n', 0x0F);

            if (index == 0) {
                print("> ");
                continue;
            }

            // Handle command
            if (!strcmp(buffer, "clear")) {
                clear_screen();
            } else if (!strcmp(buffer, "about")) {
                println("Marco OS - minimalist writer OS");
            } else if (!strcmp(buffer, "panic")) {
                kernel_panic("Manual test triggered.");
            } else {
                print("Unknown command: ");
                println(buffer);
            }

            index = 0;
            print("> ");
        } else if (c && index < 127) {
            buffer[index++] = c;
            put_char(c, 0x0F);
        }
    }
}

// Entry point
void kernel_main(void) {
    init_paging();
    clear_screen();
    println("Welcome to MBOS ");
    println("Type 'about', 'clear', or 'panic'");

    input_loop();
}

void print_hex(uint32_t num) {
    char* hex_chars = "0123456789ABCDEF";
    for (int i = 28; i >= 0; i -=4 ) {
        put_char(hex_chars[(num >> i) & 0xF], 0x0F);
    }

