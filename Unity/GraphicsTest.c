/*
 * GraphicsTest.c
 *
 *  Created on: 2 May 2020
 *      Author: Tim
 */

#include "unity.h"
#include "../include/main.h"
#include "../src/Graphics.c" /* Including the source to get access to static data */

void LEDMATRIX_vInit(void) {} /* Stub a dependency */

static void Graphics_Graphics_vInit(void) {
    GRAPHICS_vInit();
    TEST_ASSERT_EACH_EQUAL_UINT8(0, m_buffer1[0], DISPLAY_PIXELS / 2);
    TEST_ASSERT(1);
}

void GraphicsTestRunner() {
    RUN_TEST(Graphics_Graphics_vInit);
}
