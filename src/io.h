#ifndef IO_H
#define IO_H

#include <stdint.h>

/**
 * Write a byte to an I/O port
 * @param port The I/O port number
 * @param val  The value to write
 */
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/**
 * Read a byte from an I/O port
 * @param port The I/O port number
 * @return     The value read
 */
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * I/O wait (adds small delay by pinging port 0x80)
 */
static inline void io_wait() {
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

#endif
