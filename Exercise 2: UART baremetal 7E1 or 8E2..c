#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

static void uart_init_115200_7E2(void) {
    // Use double speed for better baud accuracy
    UCSR0A = (1 << U2X0);

    // Baud: UBRR = F_CPU/(8*BAUD) - 1  (because U2X0=1)

    UBRR0H = 0;
    UBRR0L = 16;

    // Enable transmitter
    UCSR0B = (1 << TXEN0); // UCSZ02=0 -> 7-bit works with UCSZ01=1,UCSZ00=0

    UCSR0C = (1 << UPM01) | (1 << USBS0) | (1 << UCSZ01);
}

static void uart_putc_7bit(char c) {
    // wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0))) {}
    // Only 7 data bits in frame
    UDR0 = (uint8_t)(c & 0x7F);
}

static void uart_puts(const char *s) {
    while (*s) uart_putc_7bit(*s++);
}

int main(void) {
    uart_init_115200_7E2();

    while (1) {
        uart_puts("UART 115200 7E2 bare metal\r\n");
        _delay_ms(1000);
    }
}
