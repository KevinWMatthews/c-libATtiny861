#ifndef ATTINY861_TIMER0_INCLUDED
#define ATTINY861_TIMER0_INCLUDED

#include <stdint.h>

typedef enum
{
    ATTN861_TIMER0_SUCCESS      = 0,
    ATTN861_TIMER0_NULL_POINTER = -1
} ATTN861_TIMER0_RETURN_CODE;

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
 * Create a Timer using the ATtiny's Timer/Counter0.
 *
 * Can not be created again without first being destroyed -
 * there is only a single Timer0 available in hardware.
 *
 * match_value and clock_source must be set.
 * Calculate the frequency of the timer interrupt:
 *  (internal_clock MHz) / (prescaler * match_value)
 * The system clock defaults to 8MHz.
 *
 * params may not be null.
 */
typedef struct ATtiny861Timer0_Params
{
    ATTN861_TIMER0_CLOCK_SOURCE clock_source;
    uint8_t match_value;
} ATtiny861Timer0_Params;
ATTN861_TIMER0_RETURN_CODE ATtiny861Timer0_Create(ATtiny861Timer0_Params *params);

/*
 * Destroy Timer0.
 *
 * Multiple destroys have no effect.
 */
ATTN861_TIMER0_RETURN_CODE ATtiny861Timer0_Destroy(void);

ATTN861_TIMER0_RETURN_CODE ATtiny861Timer0_Start(void);

#endif
