/*
 * AnimationTest.c
 *
 *  Created on: 1 May 2020
 *      Author: Tim
 */

#include "unity.h"

extern void AnimationTestRunner(void);
extern void GPIOTestRunner(void);

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

int main(void) {
    UNITY_BEGIN();
    AnimationTestRunner();
    GPIOTestRunner();
    return UNITY_END();
}
