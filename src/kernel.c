#include "limine.h"
#include <stdint.h>
#include <stddef.h>

// Set Limine base revision to 3 (Limine v8/v9/v12 protocol standard)
__attribute__((used, section(".requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(3);

__attribute__((used, section(".requests_start_marker")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

// Serial COM1 (0x3F8) I/O functions
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static void serial_init(void) {
    outb(0x3F8 + 1, 0x00);    // Disable interrupts
    outb(0x3F8 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(0x3F8 + 0, 0x03);    // Set divisor to 3 (38400 baud)
    outb(0x3F8 + 1, 0x00);    // High byte
    outb(0x3F8 + 3, 0x03);    // 8 bits, no parity, 1 stop bit
    outb(0x3F8 + 2, 0xC7);    // Enable FIFO, clear them, 14-byte threshold
    outb(0x3F8 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

static void serial_putc(char c) {
    while ((inb(0x3F8 + 5) & 0x20) == 0);
    outb(0x3F8, (uint8_t)c);
}

static void serial_print(const char *str) {
    while (*str) {
        if (*str == '\n') {
            serial_putc('\r');
        }
        serial_putc(*str++);
    }
}

// Halt loop helper
static void hlt(void) {
    while (1) {
        __asm__ volatile ("hlt");
    }
}

// Kernel entry point called by Limine
void kernel_main(void) {
    // Check if Limine base revision is supported
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == 0) {
        hlt();
    }

    serial_init();

    serial_print("\n");
    serial_print("====================================================\n");
    serial_print("  AnimeOS Kernel (x86_64) - Milestone 1: FIRST BOOT\n");
    serial_print("  Status: AnimeOS booted successfully via Limine!\n");
    serial_print("  Mode: 64-bit Long Mode (Freestanding C)\n");
    serial_print("====================================================\n\n");

    // Halt safely
    hlt();
}
