extern "C"
{
#include "ATtiny861.h"
}

#include "CppUTestExt/MockSupport.h"

void ATtiny861_GpioSetAsOutput(ATTN861_GPIO gpio, GPIO_STATE initial_state)
{
    mock().actualCall("ATtiny861_GpioSetAsOutput")
        .withParameter("gpio", gpio)
        .withParameter("initial_state", initial_state);
}

void ATtiny861_GpioSetState(ATTN861_GPIO gpio, GPIO_STATE state)
{
    mock().actualCall("ATtiny861_GpioSetState")
        .withParameter("gpio", gpio)
        .withParameter("state", state);
}