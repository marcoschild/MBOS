[bits 32]
global isr_keyboard
extern keyboard_handler

isr_keyboard:
    cli                         ; Clear interrupts
    pusha                       ; Push all general-purpose registers

    call keyboard_handler       ; Call the C function to handle input

    popa                        ; Restore registers
    mov al, 0x20
    out 0x20, al                ; Send End of Interrupt (EOI) to PIC
    sti                         ; Re-enable interrupts
    iretd                       ; Return from interrupt
