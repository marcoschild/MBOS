#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

struct idt_entry {
    uint16_t low_offset;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t high_offset;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void* isr_stub_table[256];

void set_idt_gate(int, uint32_t);
void idt_init();
void isr_handler(struct registers regs);
void irq_handler(struct registers regs);

#endif
