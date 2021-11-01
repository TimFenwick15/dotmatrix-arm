/*
 * UserInput.c
 *
 *  Created on: 8 May 2020
 *      Author: Tim
 */

#include "GPIO.h"
#include "main.h"
#include "UserInput.h"

#define BUTTON_PORT (0)
#define BUTTON_PIN (0)
#define DEBOUNCE_TIME_MS (10)

typedef struct
{
    bool bButtonPushed;
    bool bDebounce;
    uint32_t u32DebounceTime_ms;
    uint8_t u8Port;
    uint8_t u8Pin;
} m_tsButtonData;

static m_tsButtonData m_sButtonData[USERINPUT_eButtonCount] = {
        (m_tsButtonData){false, false, 0, 0, 0},
        (m_tsButtonData){false, false, 0, 0, 1},
        (m_tsButtonData){false, false, 0, 0, 2},
};

void USERINPUT_vInit(void) {
    uint8_t i;
    for (i = 0; i < USERINPUT_eButtonCount; i++)
    {
        GPIO_vInit(m_sButtonData[i].u8Port, m_sButtonData[i].u8Pin, GPIO_eInput);
    }
}

bool USERINPUT_bPollButton(USERINPUT_teButton eButton) {
    if ((true == m_sButtonData[eButton].bDebounce) &&
            ((MAIN_u32MainCounter_ms - m_sButtonData[eButton].u32DebounceTime_ms) > DEBOUNCE_TIME_MS)) {
        m_sButtonData[eButton].bDebounce = false;
    }

    if (false == m_sButtonData[eButton].bDebounce) {
        bool bOriginalButtonState = m_sButtonData[eButton].bButtonPushed;
        if (GPIO_read(m_sButtonData[eButton].u8Port, m_sButtonData[eButton].u8Pin)) {
            m_sButtonData[eButton].bButtonPushed = true;
        }
        else
        {
            m_sButtonData[eButton].bButtonPushed = false;
        }
        if (bOriginalButtonState != m_sButtonData[eButton].bButtonPushed) {
            m_sButtonData[eButton].bDebounce = true;
            m_sButtonData[eButton].u32DebounceTime_ms = MAIN_u32MainCounter_ms;
        }
    }
    return (m_sButtonData[eButton].bButtonPushed);
}
