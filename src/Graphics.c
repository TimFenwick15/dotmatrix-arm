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
#define BUFFER_2 (1)

#define RGB2_SHIFT (3)

static uint8_t u8AddToBuffer(uint8_t* sprite,
		                     uint8_t x,
							 uint8_t y,
							 uint8_t width,
							 uint8_t height);

static uint8_t m_buffer1[DISPLAY_PIXELS / 2];
static uint8_t m_buffer2[DISPLAY_PIXELS / 2];
static uint8_t m_u8CurrentBuffer = BUFFER_1;
static uint8_t m_u8SpriteId;

void GRAPHICS_vInit(void) {
	memset(m_buffer1, 0, sizeof m_buffer1);
	memset(m_buffer2, 0, sizeof m_buffer2);
	GRAPHICS_pu8Buffer = m_buffer1;
}

void GRAPHICS_vUpdate(void) {
	if (BUFFER_1 == m_u8CurrentBuffer) { /* This will lead to tearing */
		GRAPHICS_pu8Buffer = m_buffer2;
		m_u8CurrentBuffer = BUFFER_2;
		memset(m_buffer1, 0, sizeof m_buffer1);
	}
	else
	{
		GRAPHICS_pu8Buffer = m_buffer1;
		m_u8CurrentBuffer = BUFFER_1;
		memset(m_buffer2, 0, sizeof m_buffer2);
	}
}

uint8_t GRAPHICS_vDraw(uint8_t* sprite, /* Where each element is 3 bits: RGB */
		               uint8_t x,
			           uint8_t y,
			           uint8_t width,
			           uint8_t height) {
	return u8AddToBuffer(sprite, x, y, width, height);
}

uint8_t GRAPHICS_vDrawBox(uint8_t colour, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {

	uint8_t sprite[width * height];
	memset(sprite, colour, sizeof sprite);
	return u8AddToBuffer(sprite, x, y, width, height);
}


static uint8_t u8AddToBuffer(uint8_t* sprite,
		                     uint8_t x,
							 uint8_t y,
							 uint8_t width,
							 uint8_t height) {
	if ((0 != width) && (0 != height)) {
		uint8_t* pu8InactiveBuffer;
		uint16_t u16Index,
		         u16TargetPixel,
				 u16ActualYCoordinate,
				 u16TargetYCoordinate,
				 u16SpriteY;
		if (BUFFER_1 == m_u8CurrentBuffer) {
			pu8InactiveBuffer = m_buffer2;
		}
		else
		{
			pu8InactiveBuffer = m_buffer1;
		}

		for (u16Index = 0; u16Index < (uint16_t)width * (uint16_t)height; u16Index++) {
			u16SpriteY = u16Index / (uint16_t)width;
			u16TargetPixel = (uint16_t)y * DISPLAY_COLUMNS +
					(uint16_t)x + u16Index +
					(DISPLAY_COLUMNS - (uint16_t)width) * u16SpriteY;

			/* If we are attempting to draw to an x coordinate beyond the right hand edge of the display, ignore.
			 * To detect this, calculate the target y coordinate from the sprite array,
			 * and the y coordinate on the display. If we have wrapped around to a new row, these will be different.
			 */
			u16TargetYCoordinate = y + u16SpriteY;
			u16ActualYCoordinate = u16TargetPixel / DISPLAY_COLUMNS;

			/* Check we have not gone off the bottom of the display */
			if (u16TargetPixel < DISPLAY_PIXELS) {

				/* Check we have not gone off the right of the display */
				if (u16TargetYCoordinate == u16ActualYCoordinate) {

					/* Draw to either the top half of the display, or bottom half */
					if (u16TargetPixel < DISPLAY_INDICES) {
						/* Read the buffer with top half pixels masked out, OR in the new data, and write back */
						pu8InactiveBuffer[u16TargetPixel] =
							(pu8InactiveBuffer[u16TargetPixel] & 0x38) |
								((*(sprite + 1)) & 0x07);
					} else {
						/* Read the buffer with bottom half pixels masked out, OR in the new data, and write back */
						pu8InactiveBuffer[u16TargetPixel - DISPLAY_INDICES] =
							(pu8InactiveBuffer[u16TargetPixel - DISPLAY_INDICES] & 0x07) |
								(((*(sprite + 1)) << RGB2_SHIFT) & 0x38);
					}
				}
			} else {
				break;
			}
		}

	}
	return m_u8SpriteId++;
}
