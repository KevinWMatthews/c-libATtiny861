#include "avr/io.h"

// These registers are mapped to specific memory registers in chip-specific avr/io*.h files.
// Mock registers are defined here to enable testing.

// GPIO
uint8_t DDRA;
uint8_t PORTA;
uint8_t PINA;

uint8_t DDRB;
uint8_t PORTB;
uint8_t PINB;