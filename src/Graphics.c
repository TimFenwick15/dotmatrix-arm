/*
 * Graphics.h
 *
 *  Created on: 19 Apr 2020
 *      Author: Tim
 */
#include "main.h"
#include "LedMatrix.h"
#include "Graphics.h"
#include <string.h> /* memset */

#define BUFFER_1 (0)
#define BUFFER_2 (1)

#define RGB2_SHIFT (3)

#define RED_1   (0x01)
#define GREEN_1 (0x02)
#define BLUE_1  (0x04)
#define RED_2   (0x08)
#define GREEN_2 (0x10)
#define BLUE_2  (0x20)

static uint8_t u8AddToBuffer(MAIN_tsColour* sprite,
		                     int8_t x,
							 int8_t y,
							 uint8_t width,
							 uint8_t height);

static uint8_t m_buffer1[COLOUR_DEPTH][DISPLAY_PIXELS / 2];
static uint8_t m_buffer2[COLOUR_DEPTH][DISPLAY_PIXELS / 2];
static uint8_t m_u8CurrentBuffer = BUFFER_1;
static uint8_t m_u8SpriteId;

/*
 * Call before accessing other members/data in the module.
 * Prepare the draw buffers.
 */
void GRAPHICS_vInit(void) {
	uint8_t u8Depth = 0;
	for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
		memset(m_buffer1[u8Depth], 0, sizeof m_buffer1[u8Depth]);
		memset(m_buffer2[u8Depth], 0, sizeof m_buffer2[u8Depth]);
		GRAPHICS_pau8Buffer[u8Depth] = m_buffer1[u8Depth];
	}

	LEDMATRIX_vInit();
}

/*
 * Swap the front and back buffers, so the previous back buffer becomes the active drawing buffer,
 * and the current front buffer is cleared and used to store incoming draw requests.
 */
void GRAPHICS_vUpdate(void) {
	uint8_t u8Depth;
	if (BUFFER_1 == m_u8CurrentBuffer) { /* This will lead to tearing */
		for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
			GRAPHICS_pau8Buffer[u8Depth] = m_buffer2[u8Depth];
		}
		m_u8CurrentBuffer = BUFFER_2;
		for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
			memset(m_buffer1[u8Depth], 0, sizeof m_buffer1[u8Depth]);
		}
	}
	else
	{
		for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
			GRAPHICS_pau8Buffer[u8Depth] = m_buffer1[u8Depth];
		}
		m_u8CurrentBuffer = BUFFER_1;
		for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
			memset(m_buffer2[u8Depth], 0, sizeof m_buffer2[u8Depth]);
		}
	}
}

/*
 * Draw a rectangular sprite with a variety of colours
 */
uint8_t GRAPHICS_vDrawByColourArray(MAIN_tsColour* sprite, /* Where each element is 3 bits: RGB */
		               int8_t x,
			           int8_t y,
			           uint8_t width,
			           uint8_t height) {
	return u8AddToBuffer(sprite, x, y, width, height);
}

/*
 * Draw a rectangular sprite of uniform colour
 */
uint8_t GRAPHICS_vDrawBox(MAIN_tsColour colour,
		                  int8_t x,
						  int8_t y,
						  uint8_t width,
						  uint8_t height) {
	uint16_t u16Loop;
	MAIN_tsColour sprite[width * height];
	for (u16Loop = 0; u16Loop < width * height; u16Loop++) {
		sprite[u16Loop] = colour;
	}
	return u8AddToBuffer(sprite, x, y, width, height);
}

/*
 * Add a generic sprite to the back buffer.
 * This sprite will not appear on screen until GRAPHICS_vUpdate is called.
 */
static uint8_t u8AddToBuffer(MAIN_tsColour* sprite,
		                     int8_t x,
							 int8_t y,
							 uint8_t width,
							 uint8_t height) {
	/* Block impossible shapes */
	if ((0 != width) && (0 != height)) {
		uint8_t* pu8InactiveBuffer[COLOUR_DEPTH]; /* A pointer to a 2D array */
		uint16_t u16Index,
				 u16SpriteY;
		int16_t  i16TargetPixel,
		         i16ActualYCoordinate,
		         i16TargetYCoordinate;
		uint8_t  u8Depth;
		if (BUFFER_1 == m_u8CurrentBuffer) {
			uint8_t u8Depth = 0;
			for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
				pu8InactiveBuffer[u8Depth] = m_buffer2[u8Depth];
			}
		}
		else {
			uint8_t u8Depth = 0;
			for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
				pu8InactiveBuffer[u8Depth] = m_buffer1[u8Depth];
			}
		}

		for (u16Index = 0; u16Index < (uint16_t)width * (uint16_t)height; u16Index++) {
			u16SpriteY = u16Index / (uint16_t)width;
			i16TargetPixel = (int16_t)y * DISPLAY_COLUMNS +
			        (int16_t)x + u16Index +
			        (DISPLAY_COLUMNS - (int16_t)width) * (int16_t)u16SpriteY;

			/* If we are attempting to draw to an x coordinate beyond the right hand edge of the display, ignore.
			 * To detect this, calculate the target y coordinate from the sprite array,
			 * and the y coordinate on the display. If we have wrapped around to a new row, these will be different.
			 */
			i16TargetYCoordinate = y + (int16_t)u16SpriteY;
			i16ActualYCoordinate = i16TargetPixel / DISPLAY_COLUMNS;

			/* Check we have not gone off the bottom of the display */
			if (i16TargetPixel < DISPLAY_PIXELS) {

				/* Check we have not gone off the left, right, or top of the display */
				if ((i16TargetYCoordinate == i16ActualYCoordinate) && (i16TargetPixel >= 0)) {

					/* Draw to either the top half of the display, or bottom half */
					if (i16TargetPixel < DISPLAY_INDICES) {
						for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
							/* Read the buffer with top half pixels masked out, OR in the new data, and write back */
							pu8InactiveBuffer[u8Depth][i16TargetPixel] =
								(pu8InactiveBuffer[u8Depth][i16TargetPixel] & 0x38) |
									(((sprite[u16Index].red > u8Depth ? RED_1 : 0) |
									(  sprite[u16Index].green > u8Depth ? GREEN_1 : 0) |
									(  sprite[u16Index].blue > u8Depth ? BLUE_1 : 0)) & 0x07);
						}
					} else {
						for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
							/* Read the buffer with bottom half pixels masked out, OR in the new data, and write back */
							pu8InactiveBuffer[u8Depth][i16TargetPixel - DISPLAY_INDICES] =
								(pu8InactiveBuffer[u8Depth][i16TargetPixel - DISPLAY_INDICES] & 0x07) |
									(((sprite[u16Index].red > u8Depth ? RED_2 : 0) |
									(  sprite[u16Index].green > u8Depth ? GREEN_2 : 0) |
									(  sprite[u16Index].blue > u8Depth ? BLUE_2 : 0)) & 0x38);
						}
					}
				}
			} else {
				break;
			}
		}

	}
	return m_u8SpriteId++;
}
