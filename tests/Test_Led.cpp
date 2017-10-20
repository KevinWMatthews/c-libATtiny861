extern "C"
{
#include "Led.h"
}

#include "CppUTest/TestHarness.h"
// #include "CppUTestExt/MockSupport.h"

TEST_GROUP(Led)
{
    Led led;

    void setup()
    {
        led = Led_Create();
    }

    void teardown()
    {
        Led_Destroy(led);
    }
};

TEST(Led, create_an_led)
{
}

TEST(Led, led_is_off_after_create)
{
    CHECK_EQUAL(LED_OFF, Led_GetState(led));
}