#include "screen.h"
#include "io.h"

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0F

#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

uint16_t* video_memory = (uint16_t*) VIDEO_ADDRESS;

int get_cursor() {
    outb(REG_SCREEN_CTRL, 14);
    int position = inb(REG_SCREEN_DATA) << 8;
    outb(REG_SCREEN_CTRL, 15);
    position += inb(REG_SCREEN_DATA);
    return position;
}

void set_cursor(int offset) {
    outb(REG_SCREEN_CTRL, 14);
    outb(REG_SCREEN_DATA, (offset >> 8) & 0xFF);
    outb(REG_SCREEN_CTRL, 15);
    outb(REG_SCREEN_DATA, offset & 0xFF);
}

int print_char(char character, int col, int row, char attribute_byte) {
    if (!attribute_byte) attribute_byte = WHITE_ON_BLACK;

    if (col >= MAX_COLS || row >= MAX_ROWS) return -1;

    int offset = row * MAX_COLS + col;

    if (character == '\n') {
        int rows = offset / MAX_COLS;
        offset = (rows + 1) * MAX_COLS;
    } else if (character == '\b') {
        if (offset > 0) {
            video_memory[offset - 1] = ' ' | (attribute_byte << 8);
            offset--;
        }
    } else {
        video_memory[offset] = character | (attribute_byte << 8);
        offset++;
    }

    set_cursor(offset);
    return offset;
}

void print(const char* str) {
    int offset = get_cursor();
    int row = offset / MAX_COLS;
    int col = offset % MAX_COLS;

    for (int i = 0; str[i] != 0; i++) {
        offset = print_char(str[i], col, row, WHITE_ON_BLACK);
        row = offset / MAX_COLS;
        col = offset % MAX_COLS;
    }
}

void put_char(char c, char attr) {
    int offset = get_cursor();
    int row = offset / MAX_COLS;
    int col = offset % MAX_COLS;
    print_char(c, col, row, attr);
}

void clear_screen() {
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        video_memory[i] = ' ' | (WHITE_ON_BLACK << 8);
    }
    set_cursor(0);
}
