BITS 16
ORG 0x7C00

start:
    cli                     ; Disable interrupts
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00          ; Stack pointer

    ; Print boot message
    mov si, boot_msg
.print_char:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .print_char
.done:

    ; Load kernel from disk (assuming it's at sector 2+)
    mov bx, 0x1000          ; Load to 0x0000:1000 (later moved)
    mov dh, 1               ; Number of sectors to read
    call load_kernel

    ; Switch to protected mode
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:init_pm    ; Far jump to flush pipeline

; ================================
; 32-bit Protected Mode Section
; ================================
[BITS 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; Jump to the kernel
    jmp 0x100000

; ================================
; Kernel Loader (Real Mode)
; ================================
[BITS 16]
load_kernel:
    pusha

    mov ah, 0x02           ; INT 13h read
    mov al, 10             ; Number of sectors (adjust if needed)
    mov ch, 0              ; Cylinder
    mov cl, 2              ; Start at sector 2
    mov dh, 0              ; Head
    mov dl, 0x80           ; Drive (first hard disk)
    int 0x13
    jc disk_error

    popa
    ret

disk_error:
    mov si, disk_error_msg
.print_err:
    lodsb
    or al, al
    jz .
    mov ah, 0x0E
    int 0x10
    jmp .print_err

; ================================
; Boot Messages
; ================================
boot_msg db "Marco OS Bootloader starting...", 0
disk_error_msg db "Disk read error!", 0

; ================================
; Global Descriptor Table (GDT)
; ================================
gdt_start:
    ; Null descriptor
    dd 0x0
    dd 0x0

    ; Code segment
    dw 0xFFFF              ; Limit (low)
    dw 0x0000              ; Base (low)
    db 0x00                ; Base (mid)
    db 10011010b           ; Access byte
    db 11001111b           ; Flags + Limit (high)
    db 0x00                ; Base (high)

    ; Data segment
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; Segment selectors
CODE_SEG equ 0x08
DATA_SEG equ 0x10

; ================================
; Boot Signature (MBR)
; ================================
times 510 - ($ - $$) db 0
dw 0xAA55
