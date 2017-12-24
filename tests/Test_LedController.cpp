extern "C"
{
#include "LedController.h"
#include "Mock_ButtonHw.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(LedController)
{
    Led led;
    Button button;
    ButtonObserver observer;
    LEDCONTROLLER_STATUS ret;

    void setup()
    {
        MockButtonHw_Init();

        led = Led_Create(LEDHW_LED_1);
        button = Button_Create(BUTTONHW_BUTTON_1);
        observer = ButtonObserver_Create(button, BUTTON_RELEASED);
        ret = LedController_Create(observer, led);
    }

    void teardown()
    {
        LedController_Destroy();
        ButtonObserver_Destroy(&observer);
        Button_Destroy(&button);
        Led_Destroy(&led);
    }
};

/*
 * Create and destroy
 */
TEST(LedController, can_be_created_and_destroyed)
{
    LONGS_EQUAL( LEDCONTROLLER_SUCCESS, ret );
}

TEST(LedController, can_not_be_created_twice)
{
    ret = LedController_Create(observer, led);

    LONGS_EQUAL( LEDCONTROLLER_ALREADY_CREATED, ret );
}

TEST(LedController, can_be_destroyed_twice)
{
    LedController_Destroy();
}

/*
 * Button/LED behavior
 */
TEST(LedController, do_nothing_when_button_is_not_pressed)
{
    MockButtonHw_SetState(BUTTONHW_BUTTON_1, BUTTONHW_RELEASED);

    ret = LedController_Update();

    LONGS_EQUAL( LEDCONTROLLER_SUCCESS, ret );
    LONGS_EQUAL( LED_OFF, Led_GetState(led) );
}

TEST(LedController, turn_led_on_when_button_is_pressed)
{
    // ButtonHw initial state is released.

    MockButtonHw_SetState(BUTTONHW_BUTTON_1, BUTTONHW_PRESSED);
    LedController_Update();

    LONGS_EQUAL( LED_ON, Led_GetState(led) );
}

TEST(LedController, turn_led_off_when_button_is_released)
{
    MockButtonHw_SetState(BUTTONHW_BUTTON_1, BUTTONHW_PRESSED);
    LedController_Update();     // Turn LED on.

    MockButtonHw_SetState(BUTTONHW_BUTTON_1, BUTTONHW_RELEASED);
    LedController_Update();     // Turn LED off

    LONGS_EQUAL( LED_OFF, Led_GetState(led) );
}

TEST(LedController, keep_led_on_when_button_is_kept_pressed)
{
    MockButtonHw_SetState(BUTTONHW_BUTTON_1, BUTTONHW_PRESSED);
    LedController_Update();
    LedController_Update();

    LONGS_EQUAL( LED_ON, Led_GetState(led) );
}

TEST(LedController, keep_led_off_when_button_is_kept_released)
{
    MockButtonHw_SetState(BUTTONHW_BUTTON_1, BUTTONHW_PRESSED);
    LedController_Update();     // Turn LED on.

    MockButtonHw_SetState(BUTTONHW_BUTTON_1, BUTTONHW_RELEASED);
    LedController_Update();     // Turn LED off
    LedController_Update();

    LONGS_EQUAL( LED_OFF, Led_GetState(led) );
}

TEST(LedController, do_nothing_on_button_failure)
{
    // LED is initially off.
    MockButtonHw_SetState(BUTTONHW_BUTTON_1, BUTTONHW_INVALID);
    LedController_Update();

    LONGS_EQUAL( LED_OFF, Led_GetState(led) );
}
