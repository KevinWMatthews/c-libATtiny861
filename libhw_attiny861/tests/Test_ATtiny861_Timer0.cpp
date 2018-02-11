extern "C"
{
#include "ATtiny861_Timer0.h"
#include "Mock_ATtiny861_Timer0.h"
#include <avr/io.h>
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(ATtiny861_Timer0)
{
    ATTN861_TIMER0_RETURN_CODE ret;

    void setup()
    {
        mock().strictOrder();
    }

    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
    }
};


TEST(ATtiny861_Timer0, can_create_timer)
{
    ATtiny861_Timer0_Params params = {
        .clock_source = ATTN861_TIMER0_STOPPED,
        .match_value_A = 123
    };
    ret = ATtiny861_Timer0_Create(&params);

    LONGS_EQUAL( ATTN861_TIMER0_SUCCESS, ret );
    BYTES_EQUAL( _BV(CTC0), TCCR0A );
    BYTES_EQUAL( _BV(OCIE0A), TIMSK );
    BYTES_EQUAL( params.match_value_A, OCR0A );
}

TEST(ATtiny861_Timer0, can_not_pass_null_params_to_create)
{
    ret = ATtiny861_Timer0_Create(NULL);
    LONGS_EQUAL( ATTN861_TIMER0_NULL_POINTER, ret );
}

TEST(ATtiny861_Timer0, can_start_timer_using_system_clock)
{
    ATtiny861_Timer0_Params params = {
        .clock_source = ATTN861_TIMER0_INTERNAL_CLOCK,
        .match_value_A = 123
    };
    uint8_t expected_clock = 0;

    //TODO give this a descriptive function name.
    // Set clock source to system clock
    expected_clock &= ~_BV(CS02);
    expected_clock &= ~_BV(CS01);
    expected_clock |= _BV(CS00);

    ATtiny861_Timer0_Create(&params);

    ret = ATtiny861_Timer0_Start();

    LONGS_EQUAL( ATTN861_TIMER0_SUCCESS, ret );
    BYTES_EQUAL( expected_clock, TCCR0B );
    // BYTES_EQUAL( 0x00, TCNT0L );        // Never fails... nothing increments the count.
}

TEST(ATtiny861_Timer0, can_start_timer_using_internal_clock_with_prescaler_1024)
{
    ATtiny861_Timer0_Params params = {
        .clock_source = ATTN861_TIMER0_INTERNAL_CLOCK_PRESCALER_1024,
        .match_value_A = 123
    };
    uint8_t expected_clock = 0;
    //TODO give this a descriptive function name.
    // Set clock source to system clock
    expected_clock |= _BV(CS02);
    expected_clock &= ~_BV(CS01);
    expected_clock |= _BV(CS00);

    ATtiny861_Timer0_Create(&params);

    ret = ATtiny861_Timer0_Start();

    LONGS_EQUAL( ATTN861_TIMER0_SUCCESS, ret );
    BYTES_EQUAL( expected_clock, TCCR0B );
}

void match_a_callback(void)
{
    mock().actualCall("match_a_callback");
}

void match_a_callback2(void)
{
    mock().actualCall("match_a_callback2");
}

TEST(ATtiny861_Timer0, can_register_callback_for_timer0_compare_A_interrupt)
{
    ATtiny861_Timer0_Params params = {
        .clock_source = ATTN861_TIMER0_INTERNAL_CLOCK_PRESCALER_1024,
        .match_value_A = 123
    };

    ATtiny861_Timer0_Create(&params);

    ret = ATtiny861_Timer0_RegisterCallback_MatchA(match_a_callback);

    LONGS_EQUAL( ATTN861_TIMER0_SUCCESS, ret );
}

TEST(ATtiny861_Timer0, can_clear_callback_for_timer0_compare_A_interrupt)
{
    ATtiny861_Timer0_Params params = {
        .clock_source = ATTN861_TIMER0_INTERNAL_CLOCK_PRESCALER_1024,
        .match_value_A = 123
    };

    ATtiny861_Timer0_Create(&params);

    ret = ATtiny861_Timer0_RegisterCallback_MatchA(NULL);

    LONGS_EQUAL( ATTN861_TIMER0_SUCCESS, ret );
}

TEST(ATtiny861_Timer0, can_execute_callback_for_timer0_compare_A_interrupt)
{
    ATtiny861_Timer0_Params params = {
        .clock_source = ATTN861_TIMER0_INTERNAL_CLOCK_PRESCALER_1024,
        .match_value_A = 123
    };

    ATtiny861_Timer0_Create(&params);
    ATtiny861_Timer0_RegisterCallback_MatchA(match_a_callback);
    ATtiny861_Timer0_Start();

    mock().expectOneCall("match_a_callback");
    Mock_ATtiny861_Timer0_CompareMatchA();
}

TEST(ATtiny861_Timer0, second_compare_A_callback_replaces_original)
{
    ATtiny861_Timer0_Params params = {
        .clock_source = ATTN861_TIMER0_INTERNAL_CLOCK_PRESCALER_1024,
        .match_value_A = 123
    };

    ATtiny861_Timer0_Create(&params);
    ATtiny861_Timer0_RegisterCallback_MatchA(match_a_callback);
    ATtiny861_Timer0_Start();

    mock().expectOneCall("match_a_callback");
    Mock_ATtiny861_Timer0_CompareMatchA();
}

TEST(ATtiny861_Timer0, will_not_execute_null_callback_for_timer0_compare_A_interrupt)
{
    ATtiny861_Timer0_Params params = {
        .clock_source = ATTN861_TIMER0_INTERNAL_CLOCK_PRESCALER_1024,
        .match_value_A = 123
    };

    ATtiny861_Timer0_Create(&params);
    ATtiny861_Timer0_RegisterCallback_MatchA(match_a_callback);
    ATtiny861_Timer0_RegisterCallback_MatchA(match_a_callback2);
    ATtiny861_Timer0_Start();

    mock().expectOneCall("match_a_callback2");
    Mock_ATtiny861_Timer0_CompareMatchA();
}