extern "C"
{
#include "Led.h"
#include "ATtiny861_Gpio.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(Led)
{
    Led led1;
    LedParams params1;
    LED_STATE led_state1;
    ATTINY861_PIN pin1;
    GPIO_STATE gpio_state1;

    Led led2;
    LedParams params2;
    LED_STATE led_state2;
    ATTINY861_PIN pin2;
    GPIO_STATE gpio_state2;

    LED_RETURN_CODE ret;

    void setup()
    {
        pin1 = ATTN861_PA0;
        params1.pin = pin1;
        params1.active_state = LED_ACTIVE_HIGH;
        params1.initial_state = LED_OFF;
        led1 = Led_Create(&params1);
        led_state1 = LED_OFF;

        pin2 = ATTN861_PA1;
        params2.pin = pin2;
        params2.active_state = LED_ACTIVE_HIGH;
        params2.initial_state = LED_OFF;
        led2 = Led_Create(&params2);
        led_state2 = LED_OFF;
    }

    void teardown()
    {
        Led_Destroy(&led1);
        Led_Destroy(&led2);
    }
};

TEST_GROUP(Led_ActiveLow)
{
    Led led;
    LedParams params;
    LED_STATE led_state;
    LED_RETURN_CODE ret;
    ATTINY861_PIN pin;
    GPIO_STATE gpio_state;

    void setup()
    {
        pin = ATTN861_PB6;
        params.pin = pin;
        params.initial_state = LED_OFF;
        params.active_state = LED_ACTIVE_LOW;

        led = Led_Create(&params);
    }

    void teardown()
    {
        Led_Destroy(&led);
    }
};

TEST(Led, can_create_led1)
{
    CHECK_TRUE(led1 != NULL);
}

TEST(Led, can_create_led2)
{
    CHECK_TRUE(led2 != NULL);
}

TEST(Led, can_destroy_an_led)
{
    Led_Destroy(&led1);
    POINTERS_EQUAL(NULL, led1);
}

TEST(Led, can_destroy_a_null_led)
{
    Led_Destroy(NULL);
}

TEST(Led, led_is_off_after_create)
{
    ret = Led_GetState(led1, &led_state1);

    ATtiny861_Gpio_GetState(pin1, &gpio_state1);
    LONGS_EQUAL(LED_SUCCESS, ret);
    LONGS_EQUAL(LED_OFF, led_state1);
    LONGS_EQUAL(GPIO_LOW, gpio_state1);
}

TEST(Led, can_turn_led1_on)
{
    ret = Led_TurnOn(led1);

    Led_GetState(led1, &led_state1);
    ATtiny861_Gpio_GetState(pin1, &gpio_state1);
    LONGS_EQUAL(LED_SUCCESS, ret);
    LONGS_EQUAL(LED_ON, led_state1);
    LONGS_EQUAL(GPIO_HIGH, gpio_state1);
}

TEST(Led, can_turn_led1_off)
{
    Led_TurnOn(led1);

    ret = Led_TurnOff(led1);

    ATtiny861_Gpio_GetState(pin1, &gpio_state1);
    Led_GetState(led1, &led_state1);

    LONGS_EQUAL(LED_SUCCESS, ret);
    LONGS_EQUAL(LED_OFF, led_state1);
    LONGS_EQUAL(GPIO_LOW, gpio_state1);
}

TEST(Led, can_turn_led2_on)
{
    Led_TurnOn(led2);

    ret = Led_TurnOn(led2);

    ATtiny861_Gpio_GetState(pin2, &gpio_state2);
    Led_GetState(led2, &led_state2);

    LONGS_EQUAL(LED_SUCCESS, ret);
    LONGS_EQUAL(LED_ON, led_state2);
    LONGS_EQUAL(GPIO_HIGH, gpio_state2);
}

TEST(Led, can_turn_led2_off)
{
    Led_TurnOn(led2);

    ret = Led_TurnOff(led2);

    ATtiny861_Gpio_GetState(pin2, &gpio_state2);
    Led_GetState(led2, &led_state2);

    LONGS_EQUAL(LED_SUCCESS, ret);
    LONGS_EQUAL(LED_OFF, led_state2);
    LONGS_EQUAL(GPIO_LOW, gpio_state1);
}

TEST(Led, can_turn_led1_on_and_led2_off)
{
    Led_TurnOn(led1);
    Led_TurnOff(led2);

    ATtiny861_Gpio_GetState(pin1, &gpio_state1);
    ATtiny861_Gpio_GetState(pin2, &gpio_state2);
    Led_GetState(led1, &led_state1);
    Led_GetState(led2, &led_state2);

    LONGS_EQUAL(LED_ON, led_state1);
    LONGS_EQUAL(LED_OFF, led_state2);
    LONGS_EQUAL(GPIO_HIGH, gpio_state1);
    LONGS_EQUAL(GPIO_LOW, gpio_state2);
}

TEST(Led, can_toggle_an_led_on)
{
    ret = Led_Toggle(led1);

    ATtiny861_Gpio_GetState(pin1, &gpio_state1);
    Led_GetState(led1, &led_state1);

    LONGS_EQUAL(GPIO_HIGH, gpio_state1);
    LONGS_EQUAL(LED_SUCCESS, ret);
    LONGS_EQUAL(LED_ON, led_state1);
}

TEST(Led, can_toggle_an_led_on_and_off)
{
    Led_Toggle(led1);

    ret = Led_Toggle(led1);

    ATtiny861_Gpio_GetState(pin1, &gpio_state1);
    Led_GetState(led1, &led_state1);

    LONGS_EQUAL(GPIO_LOW, gpio_state1);
    LONGS_EQUAL(LED_SUCCESS, ret);
    LONGS_EQUAL(LED_OFF, led_state1);
}

TEST(Led, can_toggle_two_leds_in_same_state)
{
    Led_Toggle(led1);
    Led_Toggle(led2);

    Led_GetState(led1, &led_state1);
    Led_GetState(led2, &led_state2);
    ATtiny861_Gpio_GetState(pin1, &gpio_state1);
    ATtiny861_Gpio_GetState(pin2, &gpio_state2);

    LONGS_EQUAL(LED_ON, led_state1);
    LONGS_EQUAL(LED_ON, led_state2);
    LONGS_EQUAL(GPIO_HIGH, gpio_state1);
    LONGS_EQUAL(GPIO_HIGH, gpio_state2);
}

TEST(Led, can_toggle_two_leds_in_opposing_states)
{
    Led_TurnOn(led1);

    Led_Toggle(led1);
    Led_Toggle(led2);

    ATtiny861_Gpio_GetState(pin1, &gpio_state1);
    ATtiny861_Gpio_GetState(pin2, &gpio_state2);
    Led_GetState(led1, &led_state1);
    Led_GetState(led2, &led_state2);

    LONGS_EQUAL(LED_OFF, led_state1);
    LONGS_EQUAL(LED_ON, led_state2);
    LONGS_EQUAL(GPIO_LOW, gpio_state1);
    LONGS_EQUAL(GPIO_HIGH, gpio_state2);
}


/*
 * Validity checks
 */
TEST(Led, will_not_get_state_if_state_pointer_is_null)
{
    ret = Led_GetState(led1, NULL);
    LONGS_EQUAL(LED_NULL_POINTER, ret);
}

TEST(Led, will_not_create_an_invalid_led)
{
    params1.pin = 42;
    Led led = Led_Create(&params1);
    CHECK_TRUE(led == NULL);
}

TEST(Led, will_not_create_with_null_params)
{
    Led led = Led_Create(NULL);
    CHECK_TRUE(led == NULL);
}

TEST(Led, do_not_turn_on_a_null_led)
{
    ret = Led_TurnOn(NULL);
    LONGS_EQUAL(LED_NULL_POINTER, ret);
}

TEST(Led, do_not_turn_off_a_null_led)
{
    ret = Led_TurnOff(NULL);
    LONGS_EQUAL(LED_NULL_POINTER, ret);
}

TEST(Led, do_not_toggle_a_null_led)
{
    ret = Led_Toggle(NULL);
    LONGS_EQUAL(LED_NULL_POINTER, ret);
}




/*
 * Active low Led
 */
TEST(Led_ActiveLow, can_create_led)
{
    CHECK_TRUE( led != NULL );
}

TEST(Led_ActiveLow, led_is_off_after_create)
{
    ret = Led_GetState(led, &led_state);

    ATtiny861_Gpio_GetState(pin, &gpio_state);
    LONGS_EQUAL( GPIO_HIGH, gpio_state );
    LONGS_EQUAL( LED_OFF, led_state );
}

TEST(Led_ActiveLow, can_turn_led_on)
{
    Led_TurnOn(led);

    ATtiny861_Gpio_GetState(pin, &gpio_state);
    Led_GetState(led, &led_state);
    LONGS_EQUAL( GPIO_LOW, gpio_state );
    LONGS_EQUAL( LED_ON, led_state );
}

TEST(Led_ActiveLow, can_turn_led_off)
{
    Led_TurnOn(led);
    Led_TurnOff(led);

    ATtiny861_Gpio_GetState(pin, &gpio_state);
    Led_GetState(led, &led_state);
    LONGS_EQUAL( GPIO_HIGH, gpio_state );
    LONGS_EQUAL( LED_OFF, led_state );
}

TEST(Led, active_high_can_be_turned_on_after_create)
{
    ATTINY861_PIN pin = ATTN861_PA4;
    LedParams params = {
        .pin = pin,
        .initial_state = LED_ON,
        .active_state = LED_ACTIVE_HIGH,
    };
    LED_STATE led_state;
    GPIO_STATE gpio_state;
    Led led = Led_Create(&params);

    ATtiny861_Gpio_GetState(pin, &gpio_state);
    Led_GetState(led, &led_state);
    LONGS_EQUAL( GPIO_HIGH, gpio_state );
    LONGS_EQUAL( LED_ON, led_state );

    Led_Destroy(&led);
}

TEST(Led, active_low_can_be_turned_on_after_create)
{
    ATTINY861_PIN pin = ATTN861_PA4;
    LedParams params = {
        .pin = pin,
        .initial_state = LED_ON,
        .active_state = LED_ACTIVE_LOW,
    };
    LED_STATE led_state;
    GPIO_STATE gpio_state;
    Led led = Led_Create(&params);

    ATtiny861_Gpio_GetState(pin, &gpio_state);
    Led_GetState(led, &led_state);
    LONGS_EQUAL( GPIO_LOW, gpio_state );
    LONGS_EQUAL( LED_ON, led_state );

    Led_Destroy(&led);
}
