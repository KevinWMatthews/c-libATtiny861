#include "Avr_Led_Blink_Config.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
    // Configure Timer0
    TCCR0A &= ~_BV(TCW0);   // Enable 8-bit mode
    TCCR0A |= _BV(CTC0);    // CTC mode; clear timer on compare match rather than overflowing
    TIMSK |= _BV(OCIE0A);   // Enable Compare Match A interrupts
    OCR0A = 255;            // Set Compare Register A match value: 1 MHz / (1024*255) = 3.83 Hz

    // Configure PA0 as gpio
    DDRA |= _BV(DDA0);      // Set PA0 as output
    PORTA &= ~_BV(PORTA0);  // Set initial state low

    // Start timer
    TCCR0B |= _BV(CS02) | _BV(CS00);    // Start timer with prescaler 1024

    // Enable interrupts
    sei();

    while (1)
        ;

    return 0;
}

ISR(TIMER0_COMPA_vect)
{
    // Toggle PA0
    PINA |= _BV(PINA0);
}
