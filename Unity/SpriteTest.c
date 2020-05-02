/*
 * SpriteTest.c
 *
 *  Created on: 2 May 2020
 *      Author: Tim
 */

#include "unity.h"
#include "../include/main.h"
#include "../include/Sprite.h"

static void Sprite_Sprite_vInit(void) {
    SPRITE_vInit();
    TEST_ASSERT_EQUAL_MEMORY(&MAIN_sTransparent, &SPRITE_sRed_0[0], sizeof MAIN_sTransparent);
    TEST_ASSERT_EQUAL_MEMORY(&MAIN_sTransparent, &SPRITE_sRed_1[0], sizeof MAIN_sTransparent);
    TEST_ASSERT_EQUAL_MEMORY(&MAIN_sTransparent, &SPRITE_sRed_2[0], sizeof MAIN_sTransparent);
    TEST_ASSERT_EQUAL_MEMORY(&MAIN_sTransparent, &SPRITE_sPikachu_0[0], sizeof MAIN_sTransparent);
    TEST_ASSERT_EQUAL_MEMORY(&MAIN_sWhite, &SPRITE_sPikachu_1[0], sizeof MAIN_sWhite);
    TEST_ASSERT_EQUAL_MEMORY(&MAIN_sWhite, &SPRITE_sPikachu_2[0], sizeof MAIN_sWhite);
}

void SpriteTestRunner() {
    RUN_TEST(Sprite_Sprite_vInit);
}
