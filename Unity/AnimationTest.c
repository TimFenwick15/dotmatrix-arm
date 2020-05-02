#include "unity.h"
#include "main.h"
#include "Animation.h"

/*
void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}*/

void test_function_should_doBlahAndBlah(void) {
    /*MAIN_sTransparent.red = 0;
    MAIN_sTransparent.green = 0;
    MAIN_sTransparent.blue = 0;
    MAIN_sTransparent.visible = 0;*/
    ANIMATION_vInit();
    uint8_t a;
    TEST_ASSERT_TRUE(ANIMATION_bRegisterAnimation(&a));
    TEST_ASSERT_TRUE(ANIMATION_bRegisterAnimation(&a));
    TEST_ASSERT_TRUE(ANIMATION_bRegisterAnimation(&a));
    TEST_ASSERT_TRUE(ANIMATION_bRegisterAnimation(&a));
    TEST_ASSERT_TRUE(ANIMATION_bRegisterAnimation(&a));
    TEST_ASSERT_TRUE(ANIMATION_bRegisterAnimation(&a));
    TEST_ASSERT_TRUE(ANIMATION_bRegisterAnimation(&a));
    TEST_ASSERT_TRUE(ANIMATION_bRegisterAnimation(&a));
    TEST_ASSERT_EQUAL_UINT8(7, a);
    TEST_ASSERT_FALSE(ANIMATION_bRegisterAnimation(&a));
    TEST_ASSERT_EQUAL_UINT8(7, a);
}

void test_function_should_doAlsoDoBlah(void) {
    //more test stuff
}
