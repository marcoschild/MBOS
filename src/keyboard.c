#include "keyboard.h"
#include "io.h"
#include "screen.h"

#define KEYBOARD_DATA_PORT 0x60

char scancode_to_char[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // backspace
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',     // enter
    0,  // control
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  // left shift
    '\\','z','x','c','v','b','n','m',',','.','/', 0, // right shift
    '*',
    0,  // alt
    ' ',// space
    0,  // caps lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1-F10
    0,  // num lock
    0,  // scroll lock
    0,  // home
    0,  // up
    0,  // page up
    '-',
    0,  // left
    0,
    0,  // right
    '+',
    0,  // end
    0,  // down
    0,  // page down
    0,  // insert
    0,  // delete
    0, 0, 0,
    0, 0, 0,
    0,  // F11
    0   // F12
};

void keyboard_handler() {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    if (scancode & 0x80) {
        // Key release, ignore
        return;
    } else {
        char c = scancode_to_char[scancode];
        if (c) {
            char str[2] = {c, '\0'};
            print(str);
        }
    }
}
