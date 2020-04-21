/*
 * Graphics.h
 *
 *  Created on: 19 Apr 2020
 *      Author: Tim
 */
#include "stm32f4xx_hal.h"
#include "Graphics.h"
#include <string.h> /* memset */

#define BUFFER_1 (0)
#define BUFFER_2 (0)

#define RGB2_SHIFT (3)

static uint8_t m_buffer1[DISPLAY_PIXELS / 2];
static uint8_t m_buffer2[DISPLAY_PIXELS / 2];
static uint8_t m_u8CurrentBuffer = 0;
static uint8_t m_u8SpriteId;

void GRAPHICS_vInit(void) {
	memset(m_buffer1, 0, sizeof m_buffer1);
	memset(m_buffer2, 0, sizeof m_buffer2);
	GRAPHICS_pu8Buffer = m_buffer1;
}

void GRAPHICS_update(void) {
	if (BUFFER_1 == m_u8CurrentBuffer) { // This isn't "thread safe"
		GRAPHICS_pu8Buffer = m_buffer2;
		m_u8CurrentBuffer = BUFFER_2;
	}
	else
	{
		GRAPHICS_pu8Buffer = m_buffer1;
		m_u8CurrentBuffer = BUFFER_1;
	}
}

__uint8_t GRAPHICS_vDraw(uint8_t* sprite, // where each element is 3 bits: RGB
		                uint8_t x,
			            uint8_t y,
			            uint8_t width,
			            uint8_t height) {
	uint8_t* pu8InactiveBuffer;
	uint16_t u16Index, u16TargetPixel, u16TargetBufferIndex;
	if (BUFFER_1 == m_u8CurrentBuffer) {
		pu8InactiveBuffer = m_buffer2;
	}
	else
	{
		pu8InactiveBuffer = m_buffer1;
	}

	for (u16Index = 0; u16Index < width * height; u16Index++) {
		u16TargetPixel = DISPLAY_COLUMNS * (__uint16_t)y + (__uint16_t)x + u16Index;
		if (u16TargetPixel < DISPLAY_PIXELS) {
			if (u16TargetPixel < DISPLAY_INDICES) {
				u16TargetBufferIndex = u16TargetPixel;
	            pu8InactiveBuffer[u16TargetBufferIndex] = (*sprite)++;
			} else {
				u16TargetBufferIndex = u16TargetPixel - DISPLAY_INDICES;
				pu8InactiveBuffer[u16TargetBufferIndex] = ((*sprite)++) << RGB2_SHIFT;
			}
		} else {
			break;
		}
	}

	return m_u8SpriteId++;
}
