/*
 * AnimationTest.c
 *
 *  Created on: 1 May 2020
 *      Author: Tim
 */

#include "unity.h"
#include "../include/main.h"
//#include "../include/Animation.h"
#include "../src/Animation.c" /* This seems bad, but allows us to test static functions */


static void Animation_ANIMATION_vInit(void) {
    ANIMATION_vInit();
    TEST_ASSERT_EQUAL_PTR(m_psPlaceholder, &MAIN_sTransparent);
    TEST_ASSERT_EACH_EQUAL_INT8(0, m_u8FrameCount, ANIMATION_MAX);
}

static void Animation_i32Blend(void) {
    TEST_ASSERT(1);
    TEST_ASSERT_EQUAL_INT32(0, i32Blend(0, 0, 0));
    TEST_ASSERT_EQUAL_INT32(0, i32Blend(0, 1, 0));
    TEST_ASSERT_EQUAL_INT32(0, i32Blend(0, 1, 1));
    TEST_ASSERT_EQUAL_INT32(1, i32Blend(0, 1, 100));
    TEST_ASSERT_EQUAL_INT32(1, i32Blend(1, 1, 100));
    TEST_ASSERT_EQUAL_INT32(50, i32Blend(0, 100, 50));
}

void AnimationTestRunner() {
    RUN_TEST(Animation_ANIMATION_vInit);
    RUN_TEST(Animation_i32Blend);
}
