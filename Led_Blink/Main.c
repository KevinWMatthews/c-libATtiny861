#include "LedControllerConfig.h"
#include "Led.h"
#include "ATtiny861Timer0.h"
#include <stddef.h>
#include <avr/interrupt.h>     // For global interrupts.

// We are not (yet?) able to pass information to the callback,
// so the LED must be file-scope.
static Led led;

void timer_callback(void)
{
    static uint8_t count;
    LED_STATE led_state;
    LED_RETURN_CODE ret;

    if (count < 10)
    {
        ++count;
        return;
    }
    count = 0;

    ret = Led_GetState(led, &led_state);
    if (ret != LED_SUCCESS)
    {
        return;
    }

    if (led_state == LED_OFF)
    {
        Led_TurnOn(led);
    }
    else
    {
        Led_TurnOff(led);
    }
}

int main(void)
{
    ATtiny861Timer0_Params params = {
        .clock_source = ATTN861_TIMER0_INTERNAL_CLOCK_PRESCALER_1024,
        // Set timer to 10 Hz:
        //  match_value = 1MHZ / (1024*10Hz)
        .match_value_A = 97
    };

    ATtiny861Timer0_Create(&params);
    ATtiny861Timer0_RegisterCallback_MatchA(timer_callback);
    led = Led_Create(LED_1);

    ATtiny861Timer0_Start();
    sei();      // Enable global interrupts

    while (1)
    {
        ;
    }

    return 0;
}