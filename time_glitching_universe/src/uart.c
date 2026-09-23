#include "uart.h"

#define UART0_BASE 0x4000C000u
#define UARTDR   (*(volatile uint32_t *)(UART0_BASE + 0x000))
#define UARTFR   (*(volatile uint32_t *)(UART0_BASE + 0x018))

#define FR_TXFF (1u << 5)
#define FR_RXFE (1u << 4)

void uart_init(void) {
    // QEMU lm3s6965evb: UART works without extra init for this challenge.
}

void uart_putc(char c) {
    while (UARTFR & FR_TXFF) {}
    UARTDR = (uint32_t)c;
}

char uart_getc(void) {
    while (UARTFR & FR_RXFE) {}
    return (char)(UARTDR & 0xFF);
}

void uart_puts(const char *s) {
    while (*s) {
        if (*s == '\n') uart_putc('\r');
        uart_putc(*s++);
    }
}
