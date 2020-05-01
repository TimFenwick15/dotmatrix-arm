/*
 * AnimationTest.c
 *
 *  Created on: 1 May 2020
 *      Author: Tim
 */

#include "unity.h"
#include "../include/main.h"
#include "../include/GPIO.h"

static void test_function_should_doBlahAndBlah(void) {
    TEST_ASSERT(1);
}

static void test_function_should_doAlsoDoBlah(void) {
    //more test stuff
}

void GPIOTestRunner() {
    RUN_TEST(test_function_should_doBlahAndBlah);
    RUN_TEST(test_function_should_doAlsoDoBlah);
}
