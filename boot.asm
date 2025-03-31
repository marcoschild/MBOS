; === boot.asm ===
[BITS 16]
[ORG 0x7C00]

start:
    cli                         ; Disable interrupts
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00              ; Setup temporary stack
    cld                         ; Clear direction flag

    ; Print splash screen
    mov si, welcome_msg
.print_loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .print_loop
.done:

    ; Load GDT
    lgdt [gdt_descriptor]

    ; Enable A20 line
    call enable_a20

    ; Enter protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump into protected mode
    jmp 0x08:protected_mode_start

; === 32-bit PROTECTED MODE ===
[BITS 32]
protected_mode_start:
    ; Setup segment registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000            ; Set up kernel stack
    call main                   ; Jump into 32-bit kernel.c

.hang:
    jmp .hang                   ; Hang

; === GDT Setup ===
gdt_start:
    dq 0x0000000000000000       ; Null segment
    dq 0x00CF9A000000FFFF       ; Code segment: base=0, limit=4GB
    dq 0x00CF92000000FFFF       ; Data segment

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start
gdt_end:

; === A20 Enabling ===
enable_a20:
    in al, 0x92
    or al, 00000010b
    out 0x92, al
    ret

; === Welcome Message ===
welcome_msg db 'Booting ScriptOS...', 0

; === Boot Signature ===
times 510 - ($ - $$) db 0
dw 0xAA55
