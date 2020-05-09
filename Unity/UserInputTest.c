/*
 * UserInputTest.c
 *
 *  Created on: 9 May 2020
 *      Author: Tim
 */

#include "unity.h"
#include "../include/main.h"
#include "../include/UserInput.h"

static uint8_t m_u8ButtonState = 0;

/*
 * Stub functions
 */
void GPIO_vInit(uint8_t u8Port, uint8_t u8Pin, uint8_t eGpioType) {

}
uint8_t GPIO_read(uint8_t port, uint8_t pin) {
    return (m_u8ButtonState);
}

static void USERINPUT_USERINPUT_bPollButton(void) {
    /* Test the button state is updated and not reset by debounce */
    USERINPUT_vInit();
    MAIN_u32MainCounter_ms = 0;
    TEST_ASSERT_FALSE(USERINPUT_bPollButton());
    MAIN_u32MainCounter_ms = 10;
    TEST_ASSERT_FALSE(USERINPUT_bPollButton());
    USERINPUT_vInit();
    m_u8ButtonState = 1;
    MAIN_u32MainCounter_ms = 0;
    TEST_ASSERT_TRUE(USERINPUT_bPollButton());
    MAIN_u32MainCounter_ms = 10;
    TEST_ASSERT_TRUE(USERINPUT_bPollButton());

    /* Test the button state cannot change within the debounce time */
    USERINPUT_vInit();
    MAIN_u32MainCounter_ms = 0;
    m_u8ButtonState = 1;
    TEST_ASSERT_TRUE(USERINPUT_bPollButton());
    m_u8ButtonState = 0;
    TEST_ASSERT_TRUE(USERINPUT_bPollButton());
    MAIN_u32MainCounter_ms = 10;
    TEST_ASSERT_TRUE(USERINPUT_bPollButton());
    MAIN_u32MainCounter_ms = 11;
    TEST_ASSERT_FALSE(USERINPUT_bPollButton());

    m_u8ButtonState = 1;
    TEST_ASSERT_FALSE(USERINPUT_bPollButton());
    MAIN_u32MainCounter_ms = 21;
    TEST_ASSERT_FALSE(USERINPUT_bPollButton());
    MAIN_u32MainCounter_ms = 22;
    TEST_ASSERT_TRUE(USERINPUT_bPollButton());
}

void UserInputTestRunner() {
    RUN_TEST(USERINPUT_USERINPUT_bPollButton);
}
