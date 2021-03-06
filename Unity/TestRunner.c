/*
 * AnimationTest.c
 *
 *  Created on: 1 May 2020
 *      Author: Tim
 */

#include "unity.h"

extern void AnimationTestRunner(void);
extern void GraphicsTestRunner(void);
extern void SpriteTestRunner(void);
extern void UserInputTestRunner(void);

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

int main(void) {
    UNITY_BEGIN();
    AnimationTestRunner();
    GraphicsTestRunner();
    SpriteTestRunner();
    UserInputTestRunner();
    return UNITY_END();
}
