; kernel_entry.asm 
 [bits 32]
 [extern kernel_main]       ; Declare kernel_main is defined in another file (kernel.c)


 global _start 
 _start:
    call kernel_main        ; Jump to your C kernel function
    hlt                     ; Halt the CPU after kernel_main finishes (if ever)
