#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#include "ATtiny861_Pins.h"

typedef struct LedStruct * Led;

typedef enum
{
    LED_SUCCESS         =  0,
    LED_NULL_POINTER    = -1,
    LED_ERROR           = -2,
} LED_RETURN_CODE;

typedef enum
{
    LED_OFF = 0,
    LED_ON  = 1
} LED_STATE;

typedef enum
{
    LED_ACTIVE_LOW      = 0,
    LED_ACTIVE_HIGH     = 1,
} LED_ACTIVE_STATE;

/*
 * Create an LED handle and initialize the underlying GPIO as output.
 * LEDs default to the off state.
 */
typedef struct LedParams
{
    ATTINY861_PIN pin;
    LED_STATE initial_state;
    LED_ACTIVE_STATE active_state;
} LedParams;
Led Led_Create(LedParams *params);

/*
 * Free all resources associated with the LED handle
 * and set its pointer to NULL.
 * TODO should this turn the LED off and tri-state the GPIO?
 */
void Led_Destroy(Led * self);

/*
 * Get the state of the LED.
 * On error, state is unmodified and its value should not be used.
 */
LED_RETURN_CODE Led_GetState(Led self, LED_STATE *state);

/*
 * Turn an LED on.
 * On error, does nothing.
 */
LED_RETURN_CODE Led_TurnOn(Led self);

/*
 * Turn an LED off.
 * On error, does nothing.
 */
LED_RETURN_CODE Led_TurnOff(Led self);

/*
 * Toggle an LED.
 * On error, does nothing.
 */
LED_RETURN_CODE Led_Toggle(Led self);

#endif
