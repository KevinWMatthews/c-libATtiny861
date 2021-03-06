#ifndef ATTINY861_TIMER0_INCLUDED
#define ATTINY861_TIMER0_INCLUDED

#include <stdint.h>
#include "ATtiny861_ReturnCodes.h"

typedef enum
{
    ATTN861_TIMER0_STOPPED                          = 0,    // No clock source
    ATTN861_TIMER0_INTERNAL_CLOCK                   = 1,
    // ATTN861_TIMER0_INTERNAL_CLOCK_PRESCALER_8       = 2
    // ATTN861_TIMER0_INTERNAL_CLOCK_PRESCALER_64      = 3,
    // ATTN861_TIMER0_INTERNAL_CLOCK_PRESCALER_256     = 4,
    ATTN861_TIMER0_INTERNAL_CLOCK_PRESCALER_1024    = 5,
    // ATTN861_TIMER0_EXTERNAL_CLOCK_FALLING_EDGE      = 6,
    // ATTN861_TIMER0_EXTERNAL_CLOCK_RISING_EDGE       = 7,
} ATTN861_TIMER0_CLOCK_SOURCE;

/*
 * Callback for timer interrupts.
 */
typedef void (*TIMER0_CALLBACK)(void);

/*
 * Create a Timer using the ATtiny's Timer/Counter0 and
 * enable Timer/Counter0 Output Compare Match A interrupts.
 *
 * Creating this timer a second time will change the settings of Timer0 -
 * there is only a single Timer0 available in hardware.
 *
 * match_value_A and clock_source must be set.
 * Valid match values are 0-255.
 *
 * Calculate the interrupt frequency of a match value:
 *      Hz = (internal_clock MHz) / (prescaler * match_value)
 * Calculate the match value of a interrupt frequency:
 *      match_value = (internal_clock MHz) / (prescaler * Hz)
 * The system clock defaults to 8MHz, but mine seems to be set for 1MHz.
 * The system clock is set with fuse bits.
 *
 * params may not be null.
 */
typedef struct ATtiny861_Timer0_Params
{
    ATTN861_TIMER0_CLOCK_SOURCE clock_source;
    uint8_t match_value_A;
} ATtiny861_Timer0_Params;
ATTINY861_RETURN_CODE ATtiny861_Timer0_Create(ATtiny861_Timer0_Params *params);

/*
 * Destroy Timer0.
 *
 * Multiple destroys have no effect.
 *
 * Implement as needed.
 */
// ATTINY861_RETURN_CODE ATtiny861_Timer0_Destroy(void);

/*
 * Start Timer0.
 *
 * Sets the clock source as specified during Create(), which starts the timer
 * and enables Timer0
 *
 * Global interrupts must be enabled for the timer callback to hit.
 */
void ATtiny861_Timer0_Start(void);

/*
 * Stop Timer0.
 *
 * Implement as needed.
 */
// ATTINY861_RETURN_CODE ATtiny861_Timer0_Stop(void);

/*
 * Register a callback that will be executed with the
 * Timer/Counter0 Compare Match A interrupt.
 * The Timer0 module will define AVR's ISR vector and execute this callback there.
 *
 * Currently only allows a single callback to be registered.
 * To clear a callback, register a NULL pointer.
 */
ATTINY861_RETURN_CODE ATtiny861_Timer0_RegisterCallback_MatchA(TIMER0_CALLBACK callback);


#endif
