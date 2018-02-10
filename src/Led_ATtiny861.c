#include "Led.h"
#include "ATtiny861Gpio.h"
#include <stddef.h>

Led Led_Create(LED_NUMBER led_num)
{
    //TODO map Led number to gpio number
    ATtiny861_GpioSetAsOutput(ATTN861_PA0, GPIO_LOW);
    return NULL;
}

LED_STATE Led_GetState(Led self)
{
    // GPIO_STATE is mapped directly to LED_STATE at the moment.
    return ATtiny861_GpioGetState(ATTN861_PA0);
}

LED_RETURN_CODE Led_TurnOn(Led self)
{
    ATtiny861_GpioSetState(ATTN861_PA0, GPIO_HIGH);
    return LED_SUCCESS;
}

LED_RETURN_CODE Led_TurnOff(Led self)
{
    ATtiny861_GpioSetState(ATTN861_PA0, GPIO_LOW);
    return LED_SUCCESS;
}
