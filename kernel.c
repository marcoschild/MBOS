void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void print_char(char c, int position) {
    char *video_memory = (char *)0xb8000;
    video_memory[position * 2] = c;
    video_memory[position * 2 + 1] = 0x07;
}

void print_string(const char *str, int *offset) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i], (*offset)++);
    }
}

void backspace(int *offset) {
    if (*offset > 0) {
        (*offset)--;
        print_char(' ', *offset);  // Erase character
    }
}

void main() {
    int offset = 0;
    char buffer[128];
    int buf_index = 0;

    print_string("Welcome to ScriptOS\n", &offset);
    print_string("> ", &offset);

    while (1) {
        unsigned char scancode = inb(0x60);

        // Ignore key releases
        if (scancode & 0x80) continue;

        if (scancode == 0x0E) {  // Backspace
            if (buf_index > 0) {
                buf_index--;
                backspace(&offset);
            }
            continue;
        }

        if (scancode == 0x1C) {  // Enter
            buffer[buf_index] = '\0';
            print_char('\n', offset++);
            print_string("You typed: ", &offset);
            print_string(buffer, &offset);
            print_char('\n', offset++);
            buf_index = 0;
            print_string("> ", &offset);
            continue;
        }

        // Convert scan code to ASCII
        char c = '?';
        if (scancode >= 0x02 && scancode <= 0x0A)
            c = "1234567890"[scancode - 0x02];
        else if (scancode >= 0x10 && scancode <= 0x19)
            c = "qwertyuiop"[scancode - 0x10];
        else if (scancode >= 0x1E && scancode <= 0x26)
            c = "asdfghjkl"[scancode - 0x1E];
        else if (scancode >= 0x2C && scancode <= 0x32)
            c = "zxcvbnm"[scancode - 0x2C];
        else if (scancode == 0x39)
            c = ' ';

        if (buf_index < 127 && c != '?') {
            buffer[buf_index++] = c;
            print_char(c, offset++);
        }
    }
}
