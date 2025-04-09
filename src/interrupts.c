#include "interrupts.h"
#include "io.h"
#include "screen.h"

#define IDT_ENTRIES 256

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idt_reg;

extern void load_idt(struct idt_ptr*);
extern void isr_keyboard();

static const char* exception_messages[] = {
    "Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint",
    "Into Detected Overflow", "Out of Bounds", "Invalid Opcode",
    "No Coprocessor", "Double Fault", "Coprocessor Segment Overrun",
    "Bad TSS", "Segment Not Present", "Stack Fault", "General Protection Fault",
    "Page Fault", "Unknown Interrupt", "Coprocessor Fault",
    "Alignment Check", "Machine Check"
};

void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = handler & 0xFFFF;
    idt[n].selector = 0x08;              // Kernel code segment
    idt[n].zero = 0;
    idt[n].flags = 0x8E;                 // 10001110: Present, Ring 0, 32-bit interrupt gate
    idt[n].high_offset = (handler >> 16) & 0xFFFF;
}

void remap_pic() {
    // ICW1 - begin initialization
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2 - remap offset address of IDT
    outb(0x21, 0x20); // Master PIC to 0x20 (32)
    outb(0xA1, 0x28); // Slave PIC to 0x28 (40)

    // ICW3 - tell Master PIC that there is a slave at IRQ2 (0000 0100)
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4 - environment info
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Clear data registers
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void isr_handler(struct registers regs) {
    if (regs.int_no < 32) {
        print("Received interrupt: ");
        char num[3] = { '0' + regs.int_no / 10, '0' + regs.int_no % 10, '\0' };
        print(num);
        print(" - ");
        print(exception_messages[regs.int_no]);
        print("\n");
    }
}

void irq_handler(struct registers regs) {
    if (regs.int_no >= 40)
        outb(0xA0, 0x20); // Send reset signal to slave
    outb(0x20, 0x20);     // Send reset signal to master

    switch (regs.int_no) {
        case 33: // Keyboard
            isr_keyboard();
            break;
        default:
            break;
    }
}

void idt_init() {
    idt_reg.base = (uint32_t)&idt;
    idt_reg.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;

    for (int i = 0; i < IDT_ENTRIES; i++) {
        set_idt_gate(i, (uint32_t)isr_stub_table[i]);
    }

    remap_pic();

    // IRQs 0–15 to IDT 32–47
    for (int i = 32; i < 48; i++) {
        set_idt_gate(i, (uint32_t)isr_stub_table[i]);
    }

    load_idt(&idt_reg);
}
