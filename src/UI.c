/*
 * UI.c
 *
 *  Created on: 6 Nov 2021
 *      Author: Tim
 */

#include "main.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Animation.h"
#include "UserInput.h"
#include "RTC_DS3231.h"
#include "UI.h"

#define SCREEN_UPDATE_TIME_MS (30)
#define RTC_READ_TIME (1000)
#define OBJECT_PER_SCREEN_MAX (10)
#define OBJECT_NO_ID (0xFF)

typedef enum {
    m_eState0,
    m_eState1,
    m_eState2,
    m_eStateMax
} m_teState;

typedef enum {
    m_eProgState_None,
    m_eProgState_ChangeSeconds,
    m_eProgState_ChangeMinutes,
    m_eProgState_ChangeHours,
    m_eProgState_ChangeDay,
    m_eProgState_ChangeMonth,
    m_eProgState_ChangeYear,
    m_eProgState_Apply,
    m_eProgState_Max,
} m_teProgState;

static bool m_bButtonState = false;
static bool m_bProgButtonState = false;
static bool m_bProgValueIncreaseNeeded = false;
static m_teState m_eState = m_eState0;
static m_teProgState m_eProgState = m_eProgState_None;
static RTC_DS3231_tsTime m_sTime;

static uint32_t m_u32NextDisplayUpdate_ms = 0;
static uint32_t m_u32NextRTCRead_ms = 0;

static uint8_t u8DisplayNumber;
static uint8_t u8RedId;
static uint8_t u8PikachuId;
static uint8_t u8CircleId1;
static uint8_t u8CircleId2;
static uint8_t u8CircleId3;
static uint8_t u8CircleId4;
static uint8_t u8AgumonId;
static uint8_t u8Digimon1Id;
static uint8_t u8EggId;
static uint8_t u8DigimonBackgroundGlyphs0;
static uint8_t u8DigimonBackgroundGlyphs1;
static uint8_t u8DigimonBackgroundGlyphs2;
static uint8_t u8DigimonBackgroundGlyphs3;

void UI_vInit(void)
{
    /*
     * Draw things
     */
    MAIN_tsPosition sRedInitialPosition = {24, -16};
    MAIN_tsPosition sRedFinalPosition = {24, 48};
    CALL(ANIMATION_bRegisterAnimation(&u8RedId));
    CALL(ANIMATION_bAddFrame(u8RedId, SPRITE_sRed_0, 180)); /* Because units, ~300ms */
    CALL(ANIMATION_bAddFrame(u8RedId, SPRITE_sRed_1, 180));
    CALL(ANIMATION_bAddFrame(u8RedId, SPRITE_sRed_0, 180));
    CALL(ANIMATION_bAddFrame(u8RedId, SPRITE_sRed_2, 180));
    CALL(ANIMATION_bAddMotion(u8RedId, sRedInitialPosition,
            sRedFinalPosition, 4000));
    MAIN_tsPosition sPikachuInitialPosition = {24, -32};
    MAIN_tsPosition sPikachuFinalPosition = {24, 32};
    CALL(ANIMATION_bRegisterAnimation(&u8PikachuId));
    CALL(ANIMATION_bAddFrame(u8PikachuId, SPRITE_sPikachu_0, 180));
    CALL(ANIMATION_bAddFrame(u8PikachuId, SPRITE_sPikachu_1, 180));
    CALL(ANIMATION_bAddFrame(u8PikachuId, SPRITE_sPikachu_0, 180));
    CALL(ANIMATION_bAddFrame(u8PikachuId, SPRITE_sPikachu_2, 180));
    CALL(ANIMATION_bAddMotion(u8PikachuId, sPikachuInitialPosition,
            sPikachuFinalPosition, 4000));

    MAIN_tsPosition sCircle1InitialPosition = {0, 32};
    MAIN_tsPosition sCircle1FinalPosition = {0, -16};
    CALL(ANIMATION_bRegisterAnimation(&u8CircleId1));
    CALL(ANIMATION_bAddMotion(u8CircleId1, sCircle1InitialPosition,
            sCircle1FinalPosition, 3000));
    CALL(ANIMATION_bAddColourTransition(u8CircleId1, MAIN_sBlue,
            MAIN_sRed, 6000));

    MAIN_tsPosition sCircle2InitialPosition = {16, 32};
    MAIN_tsPosition sCircle2FinalPosition = {16, -16};
    CALL(ANIMATION_bRegisterAnimation(&u8CircleId2));
    CALL(ANIMATION_bAddMotion(u8CircleId2, sCircle2InitialPosition,
            sCircle2FinalPosition, 1500));
    CALL(ANIMATION_bAddColourTransition(u8CircleId2, MAIN_sPurple,
            MAIN_sGreen, 12000));

    MAIN_tsPosition sCircle3InitialPosition = {32, 32};
    MAIN_tsPosition sCircle3FinalPosition = {32, -16};
    CALL(ANIMATION_bRegisterAnimation(&u8CircleId3));
    CALL(ANIMATION_bAddMotion(u8CircleId3, sCircle3InitialPosition, sCircle3FinalPosition, 10000));
    CALL(ANIMATION_bAddColourTransition(u8CircleId3, MAIN_sGreen,
            MAIN_sRed, 5000));

    MAIN_tsPosition sCircle4InitialPosition = {48, 32};
    MAIN_tsPosition sCircle4FinalPosition = {48, -16};
    CALL(ANIMATION_bRegisterAnimation(&u8CircleId4));
    CALL(ANIMATION_bAddMotion(u8CircleId4, sCircle4InitialPosition,
            sCircle4FinalPosition, 2000));
    CALL(ANIMATION_bAddColourTransition(u8CircleId4, MAIN_sBlue,
            MAIN_sYellow, 2000));

    CALL(ANIMATION_bRegisterAnimation(&u8AgumonId));
    CALL(ANIMATION_bAddFrame(u8AgumonId, SPRITE_sAgumon_0, 500));
    CALL(ANIMATION_bAddFrame(u8AgumonId, SPRITE_sAgumon_1, 500));
    CALL(ANIMATION_bAddFrame(u8AgumonId, SPRITE_sAgumon_0, 500));
    CALL(ANIMATION_bAddFrame(u8AgumonId, SPRITE_sAgumon_2, 500));

    CALL(ANIMATION_bRegisterAnimation(&u8Digimon1Id));
    CALL(ANIMATION_bAddFrame(u8Digimon1Id, SPRITE_sDigimon1_0, 500));
    CALL(ANIMATION_bAddFrame(u8Digimon1Id, SPRITE_sDigimon1_1, 500));


    MAIN_tsPosition sEggInitialPosition = {48, 8};
    MAIN_tsPosition sEggFinalPosition = {50, 8};
    CALL(ANIMATION_bRegisterAnimation(&u8EggId));
    CALL(ANIMATION_bAddMotion(u8EggId, sEggInitialPosition,
            sEggFinalPosition, 1000));

    MAIN_tsPosition sDigimonBackgroundGlyphs0InitialPosition = {0, -50};
    MAIN_tsPosition sDigimonBackgroundGlyphs0FinalPosition = {0, 44};
    CALL(ANIMATION_bRegisterAnimation(&u8DigimonBackgroundGlyphs0));
    CALL(ANIMATION_bAddMotion(u8DigimonBackgroundGlyphs0, sDigimonBackgroundGlyphs0InitialPosition,
            sDigimonBackgroundGlyphs0FinalPosition, 3000));

    MAIN_tsPosition sDigimonBackgroundGlyphs1InitialPosition = {15, -30};
    MAIN_tsPosition sDigimonBackgroundGlyphs1FinalPosition = {15, 64};
    CALL(ANIMATION_bRegisterAnimation(&u8DigimonBackgroundGlyphs1));
    CALL(ANIMATION_bAddMotion(u8DigimonBackgroundGlyphs1, sDigimonBackgroundGlyphs1InitialPosition,
            sDigimonBackgroundGlyphs1FinalPosition, 3300));

    MAIN_tsPosition sDigimonBackgroundGlyphs2InitialPosition = {35, -62};
    MAIN_tsPosition sDigimonBackgroundGlyphs2FinalPosition = {35, 32};
    CALL(ANIMATION_bRegisterAnimation(&u8DigimonBackgroundGlyphs2));
    CALL(ANIMATION_bAddMotion(u8DigimonBackgroundGlyphs2, sDigimonBackgroundGlyphs2InitialPosition,
            sDigimonBackgroundGlyphs2FinalPosition, 3100));

    MAIN_tsPosition sDigimonBackgroundGlyphs3InitialPosition = {50, -45};
    MAIN_tsPosition sDigimonBackgroundGlyphs3FinalPosition = {50, 49};
    CALL(ANIMATION_bRegisterAnimation(&u8DigimonBackgroundGlyphs3));
    CALL(ANIMATION_bAddMotion(u8DigimonBackgroundGlyphs3, sDigimonBackgroundGlyphs3InitialPosition,
            sDigimonBackgroundGlyphs3FinalPosition, 2800));

    CALL(ANIMATION_bRegisterAnimation(&u8DisplayNumber));
    CALL(ANIMATION_bAddColourTransition(u8DisplayNumber, MAIN_sBlue,
            MAIN_sYellow, 20000));
}

void UI_vTask(uint32_t u32Counter_ms)
{
    bool bPreviousButtonState = m_bButtonState;
    m_bButtonState = USERINPUT_bPollButton(USERINPUT_eButton3);

    /* We only want to know when the button was first pressed. Don't change screen while changing time */
    if ((true == m_bButtonState) && (false == bPreviousButtonState))
    {
        if (!((m_eProgState > m_eProgState_None) && (m_eState == m_eState0)))
        {
            m_eState++;
            if (m_eState >= m_eStateMax)
            {
                m_eState = m_eState0;
            }
        }
        else
        {
            m_bProgValueIncreaseNeeded = true;
        }
    }

    bool bPreviousProgState = m_bProgButtonState;
    m_bProgButtonState = USERINPUT_bPollButton(USERINPUT_eButton2);
    if ((true == m_bProgButtonState) && (false == bPreviousProgState))
    {
        m_eProgState++;
        if (m_eProgState >= m_eProgState_Max)
        {
            m_eProgState = m_eProgState_None;
        }
        m_bProgValueIncreaseNeeded = false;
    }

    if (u32Counter_ms >= m_u32NextDisplayUpdate_ms) /* 100 * 300us ~ 30ms. Screen redraws in about 5ms */
    {
        m_u32NextDisplayUpdate_ms = u32Counter_ms + SCREEN_UPDATE_TIME_MS;
        switch (m_eState) {
        case m_eState0:
            if (m_eProgState == m_eProgState_None)
            {
                if (u32Counter_ms >= m_u32NextRTCRead_ms)
                {
                    m_u32NextRTCRead_ms = u32Counter_ms + RTC_READ_TIME;
                    RTC_DS3231_vGetTime(&m_sTime);
                }
            }
            else if (m_eProgState == m_eProgState_Apply)
            {
                RTC_DS3231_vSetTime(&m_sTime);
                m_eProgState = m_eProgState_None;
            }
            else
            {
                if (m_bProgValueIncreaseNeeded)
                {
                    switch(m_eProgState)
                    {
                    case m_eProgState_ChangeSeconds:
                        m_sTime.seconds++;
                        if (m_sTime.seconds >= 60)
                        {
                            m_sTime.seconds = 0;
                        }
                        break;
                    case m_eProgState_ChangeMinutes:
                        m_sTime.minutes++;
                        if (m_sTime.minutes >= 60)
                        {
                            m_sTime.minutes = 0;
                        }
                        break;
                    case m_eProgState_ChangeHours:
                        m_sTime.hour++;
                        if (m_sTime.hour >= 24)
                        {
                            m_sTime.hour = 0;
                        }
                        break;
                    case m_eProgState_ChangeDay:
                        m_sTime.dayofmonth++;
                        if (m_sTime.dayofmonth >= 32) // this needs leap year handling :(
                        {
                            m_sTime.dayofmonth = 0;
                        }
                        break;
                    case m_eProgState_ChangeMonth:
                        m_sTime.month++;
                        if (m_sTime.month >= 13)
                        {
                            m_sTime.month = 0;
                        }
                        break;

                    // We've skipped day of week and year because they're not on screen yet

                    default:
                        m_eProgState++;
                        if (m_eProgState == m_eProgState_Max)
                        {
                            m_eProgState = m_eProgState_None;
                        }
                        break;
                    }
                    m_bProgValueIncreaseNeeded = false;
                }
                GRAPHICS_vDrawCharacter(MAIN_sWhite, (MAIN_tsPosition){40,24}, 'P', GRAPHICS_eFontSize8x8);
            }

            GRAPHICS_vDrawNumber(ANIMATION_sGetColour(u8DisplayNumber), (MAIN_tsPosition){0,0}, m_sTime.hour, 2, true, GRAPHICS_eFontSize16x16);
            GRAPHICS_vDrawCharacter(MAIN_sWhite, (MAIN_tsPosition){29,5}, ':', GRAPHICS_eFontSize8x8);
            GRAPHICS_vDrawNumber(ANIMATION_sGetColour(u8DisplayNumber), (MAIN_tsPosition){32,0}, m_sTime.minutes, 2, true, GRAPHICS_eFontSize16x16);
            GRAPHICS_vDrawNumber(ANIMATION_sGetColour(u8DisplayNumber), (MAIN_tsPosition){48,20}, m_sTime.seconds, 2, true, GRAPHICS_eFontSize8x8);

            GRAPHICS_vDrawNumber(MAIN_sPurple, (MAIN_tsPosition){0,20}, m_sTime.dayofmonth, 2, true, GRAPHICS_eFontSize8x8);
            GRAPHICS_vDrawCharacter(MAIN_sWhite, (MAIN_tsPosition){16,20}, '/', GRAPHICS_eFontSize8x8);
            GRAPHICS_vDrawNumber(MAIN_sPurple, (MAIN_tsPosition){23,20}, m_sTime.month, 2, true, GRAPHICS_eFontSize8x8);
            break;

        case m_eState1:
            GRAPHICS_vDrawCircle(ANIMATION_sGetColour(u8CircleId1),
                    ANIMATION_sGetPosition(u8CircleId1), 8);
            GRAPHICS_vDrawCircle(ANIMATION_sGetColour(u8CircleId2),
                    ANIMATION_sGetPosition(u8CircleId2), 8);
            GRAPHICS_vDrawCircle(ANIMATION_sGetColour(u8CircleId3),
                    ANIMATION_sGetPosition(u8CircleId3), 8);
            GRAPHICS_vDrawCircle(ANIMATION_sGetColour(u8CircleId4),
                    ANIMATION_sGetPosition(u8CircleId4), 8);
            GRAPHICS_vDrawByColourArray(ANIMATION_psGetFrame(u8RedId),
                    ANIMATION_sGetPosition(u8RedId), RED_SIZE, RED_SIZE);
            GRAPHICS_vDrawByColourArray(ANIMATION_psGetFrame(u8PikachuId),
                    ANIMATION_sGetPosition(u8PikachuId), PIKACHU_SIZE, PIKACHU_SIZE);
            break;
        case m_eState2:
            GRAPHICS_vDrawByColourArray(SPRITE_sBackground,
                    ANIMATION_sGetPosition(u8DigimonBackgroundGlyphs0),
                    BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
            GRAPHICS_vDrawByColourArray(SPRITE_sBackground,
                    ANIMATION_sGetPosition(u8DigimonBackgroundGlyphs1),
                    BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
            GRAPHICS_vDrawByColourArray(SPRITE_sBackground,
                    ANIMATION_sGetPosition(u8DigimonBackgroundGlyphs2),
                    BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
            GRAPHICS_vDrawByColourArray(SPRITE_sBackground,
                    ANIMATION_sGetPosition(u8DigimonBackgroundGlyphs3),
                    BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
            GRAPHICS_vDrawByColourArray(ANIMATION_psGetFrame(u8AgumonId),
                    (MAIN_tsPosition){24, 8}, AGUMON_SIZE, AGUMON_SIZE);
            GRAPHICS_vDrawByColourArray(ANIMATION_psGetFrame(u8Digimon1Id),
                    (MAIN_tsPosition) {0, 8}, DIGIMON_1_WIDTH, DIGIMON_1_HEIGHT);
            GRAPHICS_vDrawByColourArray(SPRITE_sEgg,
                    ANIMATION_sGetPosition(u8EggId), EGG_SIZE, EGG_SIZE);
            break;
        default:
            break;
        }
        GRAPHICS_vUpdate();
    }
}
