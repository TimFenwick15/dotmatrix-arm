/*
 * LedMatrix.c
 *
 *  Created on: 19 Apr 2020
 *      Author: Tim
 */
//#include "main.h"
#include "stm32f4xx_hal.h"
#include "LedMatrix.h"
#include "BlinkLed.h"
#include "Graphics.h"

#define PIN_NUMBER_R1  (0)
#define PIN_NUMBER_G1  (1)
#define PIN_NUMBER_B1  (2)
#define PIN_NUMBER_R2  (3)
#define PIN_NUMBER_G2  (4)
#define PIN_NUMBER_B2  (5)
#define PIN_NUMBER_A   (6)
#define PIN_NUMBER_B   (7)
#define PIN_NUMBER_C   (8)
#define PIN_NUMBER_D   (9)
#define PIN_NUMBER_CLK (10)
#define PIN_NUMBER_LAT (11)
#define PIN_NUMBER_OE  (15) /* 12 is an LED on the discovery board */

#define PIN_R1  (1 << PIN_NUMBER_R1)  /* These being consecutive allows us to write them all in one action, directly from the buffer */
#define PIN_G1  (1 << PIN_NUMBER_G1)
#define PIN_B1  (1 << PIN_NUMBER_B1)
#define PIN_R2  (1 << PIN_NUMBER_R2)
#define PIN_G2  (1 << PIN_NUMBER_G2)
#define PIN_B2  (1 << PIN_NUMBER_B2)
#define PIN_A   (1 << PIN_NUMBER_A)   /* These being consecutive allows us to write them all in one action, directly from the row address */
#define PIN_B   (1 << PIN_NUMBER_B)
#define PIN_C   (1 << PIN_NUMBER_C)
#define PIN_D   (1 << PIN_NUMBER_D)
#define PIN_CLK (1 << PIN_NUMBER_CLK) /* The order of these doesn't matter :) */
#define PIN_LAT (1 << PIN_NUMBER_LAT)
#define PIN_OE  (1 << PIN_NUMBER_OE)

#define ADDRESS_PORT_ON(x) ((x << PIN_NUMBER_A) & 0x3C0)
#define ADDRESS_PORT_OFF(x) (~(x << PIN_NUMBER_A) & 0x3C0)

#define ADRESS_PORT_SHIFT (PIN_NUMBER_A)
#define ADDRESS_MAX (DISPLAY_ROWS >> 1) /* Each address covers two rows. Address 0 is row 0 and row 16, address 1 is row 1 and row 17 etc */
static uint16_t m_u16Address = 0;

void LEDMATRIX_vInit(void) {
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_R1, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_G1, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_B1, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_R2, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_G2, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_B2, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_A, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_B, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_C, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_D, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_CLK, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_LAT, GPIO_OUTPUT);
	GPIO_vInit(GPIO_PORT_NUMBER, PIN_NUMBER_OE, GPIO_OPEN_DRAIN);

	GPIO_off(PIN_R1  | PIN_G1  | PIN_B1  | PIN_R2  | PIN_G2  | PIN_B2  |
			 PIN_A   | PIN_B   | PIN_C   | PIN_D   |
			 PIN_CLK | PIN_OE  | PIN_LAT);

	LEDMATRIX_u32Counter_ms = 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	GPIO_on(PIN_OE | PIN_LAT | ADDRESS_PORT_ON(m_u16Address));
	GPIO_off(ADDRESS_PORT_OFF(m_u16Address));
	GPIO_off(PIN_OE | PIN_LAT);

	m_u16Address++;
	if (m_u16Address >= ADDRESS_MAX) {
		m_u16Address = 0;
	}
	if (m_u16Address & 1) {
		LEDMATRIX_u32Counter_ms++;
	}

    uint16_t x = 0;
    uint16_t colourPort, notColourPort;
    const uint16_t indexOffset = m_u16Address * DISPLAY_COLUMNS;

    /* Loop unrolling, idea taken from Adafruits Arduino code for a dot matrix display.
     * When testing on a TI Picolo, this offered significant speed improvement.
     * The disadvantage is it knows the display is 64 pixels wide. So we will need to drop this to get more display sizes
     * (or #if some standard display sizes)
     * https://github.com/adafruit/RGB-matrix-Panel/blob/5d14b09d67e87f733db91ae7a4f667e517393d17/RGBmatrixPanel.cpp#L842
     */

#define pew \
		colourPort = GRAPHICS_pu8Buffer[x++ + indexOffset] & 0x3F;\
    	notColourPort = (~colourPort) & 0x3F;\
        GPIO_on(colourPort & 0x3F);\
        GPIO_off((~colourPort) & 0x3F);\
        GPIO_on(PIN_CLK);\
        GPIO_off(PIN_CLK);

	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
}
