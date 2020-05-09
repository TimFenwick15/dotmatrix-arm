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

static bool m_bButtonPushed = false;
static bool m_bDebounce = false;
static uint32_t m_u32DebounceTime_ms = 0;

void USERINPUT_vInit(void) {
    GPIO_vInit(BUTTON_PORT, BUTTON_PIN, GPIO_eInput);
}

bool USERINPUT_bPollButton(void) {
    if ((true == m_bDebounce) &&
            ((MAIN_u32MainCounter_ms - m_u32DebounceTime_ms) > DEBOUNCE_TIME_MS)) {
        m_bDebounce = false;
    }

    if (false == m_bDebounce) {
        bool bOriginalButtonState = m_bButtonPushed;
        if (GPIO_read(BUTTON_PORT, BUTTON_PIN)) {
            m_bButtonPushed = true;
        }
        else
        {
            m_bButtonPushed = false;
        }
        if (bOriginalButtonState != m_bButtonPushed) {
            m_bDebounce = true;
            m_u32DebounceTime_ms = MAIN_u32MainCounter_ms;
        }
    }
    return (m_bButtonPushed);
}
