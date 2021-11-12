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
#include <Math.h>   /* pow */

#define BUFFER_1 (0)
#define BUFFER_2 (1)

#define RGB2_SHIFT (3)

#define RED_1   (0x01)
#define GREEN_1 (0x02)
#define BLUE_1  (0x04)
#define RED_2   (0x08)
#define GREEN_2 (0x10)
#define BLUE_2  (0x20)

#define ASCII_DEFINED_OFFSET (32U) /* Special characters at the start of the ASCII table I'm going to skip */
#define ASCII_NUMBER_OFFSET (48U)

static void vAddToBuffer(MAIN_tsColour* sprite,
                         MAIN_tsPosition position,
						 uint16_t width,
						 uint16_t height);

static void m_vDrawCharacter(MAIN_tsColour* sprite,
                             MAIN_tsColour colour,
                             uint64_t characterSprite,
                             GRAPHICS_teFontSize fontSize);

static uint8_t m_buffer1[COLOUR_DEPTH][DISPLAY_PIXELS / 2];
static uint8_t m_buffer2[COLOUR_DEPTH][DISPLAY_PIXELS / 2];
static uint8_t m_u8CurrentBuffer = BUFFER_1;

/* Zeros are unspecified tiles. I'm going to add them as I need them. If drawn, they will appear blank */
static uint64_t m_au64ASCII[] = {
        0b0,                                                                /* SPACE */
        0b0,                                                                /* ! */
        0b0,                                                                /* " */
        0b0,                                                                /* # */
        0b0,                                                                /* $ */
        0b0,                                                                /* % */
        0b0,                                                                /* & */
        0b0,                                                                /* ' */
        0b0,                                                                /* ( */
        0b0,                                                                /* ) */
        0b0,                                                                /* * */
        0b0,                                                                /* + */
        0b0,                                                                /* , */
        0b0,                                                                /* - */
        0b0,                                                                /* . */
        0b0000000000000010000001100000110000011000001100000110000001000000, /* / */
        0b0000000000011100001000100010011000101010001100100010001000011100, /* 0 */
        0b0000000000001000000110000000100000001000000010000000100000011100, /* 1 */
        0b0000000000011100001000100000010000001000000100000010000000111110, /* 2 */
        0b0000000000011100001000100000001000000100000000100010001000011100, /* 3 */
        0b0000000000000100000011000001010000100100001111100000010000000100, /* 4 */
        0b0000000000111110001000000010000000011100000000100000001000111100, /* 5 */
        0b0000000000001100000100000010000000111100001000100010001000011100, /* 6 */
        0b0000000000111110000000100000010000001000000010000000100000001000, /* 7 */
        0b0000000000011000001001000010010000011000001001000010010000011000, /* 8 */
        0b0000000000011000001001000010010000011100000001000010010000011000, /* 9 */
        0b0000000000011000000110000000000000000000000110000001100000000000, /* : */
        0b0,                                                                /* ; */
        0b0,                                                                /* < */
        0b0,                                                                /* = */
        0b0,                                                                /* > */
        0b0,                                                                /* ? */
        0b0,                                                                /* @ */
        0b0,                                                                /* A */
        0b0,                                                                /* B */
        0b0000000000011100001000100010000000100000001000000010001000011100, /* C */
        0b0,                                                                /* D */
        0b0,                                                                /* E */
        0b0,                                                                /* F */
        0b0,                                                                /* G */
        0b0,                                                                /* H */
        0b0,                                                                /* I */
        0b0,                                                                /* J */
        0b0,                                                                /* K */
        0b0,                                                                /* L */
        0b0,                                                                /* M */
        0b0,                                                                /* N */
        0b0,                                                                /* O */
        0b0000000000111100001000100010001000111100001000000010000000100000, /* P */
        0b0,                                                                /* Q */
        0b0,                                                                /* R */
};

static uint64_t m_au64NonASCII[GRAPHICS_eNonAsciiCharacters_Max] = {
        0b1110000010100000111000000000000000000000000000000000000000000000, /* GRAPHICS_eNonAsciiCharacters_Degrees */
};


/**
 * Call before accessing other members/data in the module.
 * Prepare the draw buffers.
 */
void GRAPHICS_vInit(void) {
	uint8_t u8Depth = 0;
	for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
		memset(m_buffer1[u8Depth], 0, sizeof m_buffer1[u8Depth]);
		memset(m_buffer2[u8Depth], 0, sizeof m_buffer2[u8Depth]);
		MAIN_pau8Buffer[u8Depth] = m_buffer1[u8Depth];
	}

	LEDMATRIX_vInit();
}

/**
 * Swap the front and back buffers, so the previous back buffer becomes the active drawing buffer,
 * and the current front buffer is cleared and used to store incoming draw requests.
 */
void GRAPHICS_vUpdate(void) {
	uint8_t u8Depth;
	if (BUFFER_1 == m_u8CurrentBuffer) { /* This will lead to tearing */
		for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
			MAIN_pau8Buffer[u8Depth] = m_buffer2[u8Depth];
		}
		m_u8CurrentBuffer = BUFFER_2;
		for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
			memset(m_buffer1[u8Depth], 0, sizeof m_buffer1[u8Depth]);
		}
	}
	else
	{
		for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
			MAIN_pau8Buffer[u8Depth] = m_buffer1[u8Depth];
		}
		m_u8CurrentBuffer = BUFFER_1;
		for (u8Depth = 0; u8Depth < COLOUR_DEPTH; u8Depth++) {
			memset(m_buffer2[u8Depth], 0, sizeof m_buffer2[u8Depth]);
		}
	}
}

/**
 * Draw a rectangular sprite with a variety of colours.
 * This sprite will not appear on screen until GRAPHICS_vUpdate is called.
 * @param sprite - the array of colours that form this sprite
 * @param position - the x y coordinates of the top left of the sprite relative to the top left corner of the screen
 * @param width - the width of the sprite, > 0, in pixels
 * @param height - the heigh of the sprite, > 0, in pixels
 */
void GRAPHICS_vDrawByColourArray(MAIN_tsColour* sprite, /* Where each element is 3 bits: RGB */
                                 MAIN_tsPosition position,
                                 uint8_t width,
                                 uint8_t height) {
	vAddToBuffer(sprite, position, width, height);
}

/**
 * Draw a rectangular sprite of uniform colour.
 * This sprite will not appear on screen until GRAPHICS_vUpdate is called.
 * @param colour - the colour the box should be
 * @param position - the x y coordinates of the top left of the box, relative to the top left corner of the screen
 * @param width - the width of the box, > 0, in pixels
 * @param height - the heigh of the box, > 0, in pixels
 */
void GRAPHICS_vDrawBox(MAIN_tsColour colour,
                          MAIN_tsPosition position,
						  uint8_t width,
						  uint8_t height) {
	uint16_t u16Loop;
	MAIN_tsColour sprite[width * height];
	for (u16Loop = 0; u16Loop < width * height; u16Loop++) {
		sprite[u16Loop] = colour;
	}
	vAddToBuffer(sprite, position, width, height);
}

/**
 * Draw a circular sprite of uniform colour
 * This sprite will not appear on screen until GRAPHICS_vUpdate is called.
 * @param colour - the colour the circle should be
 * @param position - the x y coordinates of the top left of the circles bounding box, relative to the top left corner of the screen
 * @param radius - the circle radius in pixels. The diameter will be double this
 */
void GRAPHICS_vDrawCircle(MAIN_tsColour colour,
                          MAIN_tsPosition position,
						  uint16_t radius) {
	/* Prevent an impossible shape */
	if (radius == 0) {
		radius = 1;
	}

	uint16_t u16Loop, u16DistanceToOriginSquared;
	uint16_t u16SideLength = radius + radius;
	uint16_t u16RadiusSquared = radius * radius;
	uint16_t u16SpriteIndices = 4 * u16RadiusSquared;
	MAIN_tsPosition i16ApproximateOrigin = {position.x + radius, position.y + radius};
	MAIN_tsPosition sCurrentCoordinates, sOriginCoordinates, sDeltaCoordinates;
	MAIN_tsColour sprite[u16SpriteIndices];

	/* We will draw any pixel within the circle radius.
	 * If we use distanceFromCircleCentre^2 and radius squared, we avoid the expensive square root
	 */

	for (u16Loop = 0; u16Loop < u16SpriteIndices; u16Loop++) {
		sCurrentCoordinates = (MAIN_tsPosition) {
			(u16Loop % u16SideLength),
			(u16Loop / u16SideLength)
		};

		/* Because we double the radius, we always have an even diameter circle.
		 * This means there is not a well defined centre, so the circle appears uneven.
		 * So solve this, move the origin into the same quadrant as the current pixel.
		 */
		sOriginCoordinates = (MAIN_tsPosition) {
			((sCurrentCoordinates.x < i16ApproximateOrigin.x) ?
				(i16ApproximateOrigin.x - 1) : i16ApproximateOrigin.x),
			((sCurrentCoordinates.y < i16ApproximateOrigin.y) ?
				(i16ApproximateOrigin.y - 1) : i16ApproximateOrigin.y)
		};

		/* Now calculate the distance (squared) of this pixel to the circle centre */
		sDeltaCoordinates = (MAIN_tsPosition) {
			(sOriginCoordinates.x - position.x - sCurrentCoordinates.x),
			(sOriginCoordinates.y - position.y - sCurrentCoordinates.y)
		};
		u16DistanceToOriginSquared = sDeltaCoordinates.x * sDeltaCoordinates.x +
			sDeltaCoordinates.y * sDeltaCoordinates.y;

		/* If the pixel is in the circle, draw it */
		if (u16DistanceToOriginSquared <= u16RadiusSquared) {
			sprite[u16Loop] = colour;
		} else {
			sprite[u16Loop] = MAIN_sTransparent;
		}
	}
	vAddToBuffer(sprite, position, u16SideLength, u16SideLength);
}

void GRAPHICS_vDrawCharacter(MAIN_tsColour colour,
                             MAIN_tsPosition position,
                             unsigned char character,
                             GRAPHICS_teFontSize fontSize)
{
    if ((character - ASCII_DEFINED_OFFSET) < (sizeof(m_au64ASCII) / sizeof(m_au64ASCII[0])))
    {
        uint16_t sideLength = (uint16_t)fontSize;
        MAIN_tsColour sprite[sideLength * sideLength];
        m_vDrawCharacter(sprite, colour, m_au64ASCII[character - ASCII_DEFINED_OFFSET], fontSize);
        vAddToBuffer(sprite, position, sideLength, sideLength);
    }
}

void GRAPHICS_vDrawNonAsciiCharacter(MAIN_tsColour colour,
                                     MAIN_tsPosition position,
                                     GRAPHICS_teNonAsciiCharacters character,
                                     GRAPHICS_teFontSize fontSize)
{
    if (character < GRAPHICS_eNonAsciiCharacters_Max)
    {
        uint16_t sideLength = (uint16_t)fontSize;
        MAIN_tsColour sprite[sideLength * sideLength];
        m_vDrawCharacter(sprite, colour, m_au64NonASCII[character], fontSize);
        vAddToBuffer(sprite, position, sideLength, sideLength);
    }
}

void GRAPHICS_vDrawNumber(MAIN_tsColour colour,
                          MAIN_tsPosition position,
                          uint32_t number,
                          uint8_t maximumDisplayDigits,
                          bool fillBlankDigitsWithZeros,
                          GRAPHICS_teFontSize fontSize)
{
    uint16_t sideLength = (uint16_t)fontSize;
    MAIN_tsColour sprite[sideLength * sideLength];
    uint8_t maxDigits = DISPLAY_COLUMNS / sideLength;
    if (maxDigits > maximumDisplayDigits)
    {
        maxDigits = maximumDisplayDigits;
    }
    uint32_t maxNumber = pow(10, maxDigits) - 1;
    uint8_t i;
    uint8_t character;
    uint16_t denominator;
    bool firstCharFound = false;
    if (number <= maxNumber)
    {
        for (i = 0; i < maxDigits; i++)
        {
            denominator = (uint16_t)pow(10, maxDigits - i - 1);
            if ((number > (uint16_t)(denominator - 1)) || firstCharFound)
            {
                firstCharFound = true;                                  /* Once we've started drawing the number, zeros are always included */
                character = (uint8_t)(number / denominator);
                number -= (character * denominator);                    /* Once we've drawn a digit, remove it so we can check the next column in isolation */
                m_vDrawCharacter(sprite, colour, m_au64ASCII[ASCII_NUMBER_OFFSET - ASCII_DEFINED_OFFSET + character], fontSize);
                vAddToBuffer(sprite, position, sideLength, sideLength);
            }
            else
            {
                if (fillBlankDigitsWithZeros)
                {
                    m_vDrawCharacter(sprite, colour, m_au64ASCII[ASCII_NUMBER_OFFSET - ASCII_DEFINED_OFFSET], fontSize);
                    vAddToBuffer(sprite, position, sideLength, sideLength);
                }
            }
            memset(sprite, 0, sizeof(sprite));
            position.x += sideLength;
        }
    }
}

static void m_vDrawCharacter(MAIN_tsColour* sprite,
                             MAIN_tsColour colour,
                             uint64_t characterSprite,
                             GRAPHICS_teFontSize fontSize)
{
    uint16_t i;
    uint16_t baseIndex;
    MAIN_tsColour newColour;
    for (i = 0; i < 64; i++)
    {
        newColour = ((characterSprite >> (63 - i)) & 1) ? colour : MAIN_sTransparent;
        switch (fontSize)
        {
        case GRAPHICS_eFontSize8x8:
            sprite[i] = newColour;
            break;
        case GRAPHICS_eFontSize16x16:
            baseIndex = i * 2 + (i / 8) * 16;
            sprite[baseIndex     ] = newColour;
            sprite[baseIndex +  1] = newColour;
            sprite[baseIndex + 16] = newColour;
            sprite[baseIndex + 17] = newColour;
            break;
        case GRAPHICS_eFontSize32x32:
            baseIndex = i * 4 + (i / 8) * 96;
            sprite[baseIndex     ] = newColour;
            sprite[baseIndex +  1] = newColour;
            sprite[baseIndex +  2] = newColour;
            sprite[baseIndex +  3] = newColour;

            sprite[baseIndex + 32] = newColour;
            sprite[baseIndex + 33] = newColour;
            sprite[baseIndex + 34] = newColour;
            sprite[baseIndex + 35] = newColour;

            sprite[baseIndex + 64] = newColour;
            sprite[baseIndex + 65] = newColour;
            sprite[baseIndex + 66] = newColour;
            sprite[baseIndex + 67] = newColour;

            sprite[baseIndex + 96] = newColour;
            sprite[baseIndex + 97] = newColour;
            sprite[baseIndex + 98] = newColour;
            sprite[baseIndex + 99] = newColour;
            break;
        default:
            break;
        }
    }
}

/**
 * Add a generic sprite to the back buffer.
 * This sprite will not appear on screen until GRAPHICS_vUpdate is called.
 * @param sprite - the array of colours that form this sprite
 * @param position - the x y coordinates of the top left of the sprite relative to the top left corner of the screen
 * @param width - the width of the sprite, > 0, in pixels
 * @param height - the heigh of the sprite, > 0, in pixels
 */
static void vAddToBuffer(MAIN_tsColour* sprite,
                         MAIN_tsPosition position,
                         uint16_t width,
                         uint16_t height) {
    /* Block impossible shapes */
    if ((0 != width) && (0 != height)) {
        uint8_t* pu8InactiveBuffer[COLOUR_DEPTH]; /* A pointer to a 2D array */
        uint16_t u16Index,
				 u16SpriteY;
		int16_t  i16TargetPixel,
		         i16ActualYCoordinate,
		         i16TargetYCoordinate;
		uint8_t  u8Depth;

		/* Pick the inactive buffer - this is the buffer we draw to, while the front buffer is rendered to the screen */
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

		for (u16Index = 0; u16Index < width * (uint16_t)height; u16Index++) {

			u16SpriteY = u16Index / width;
			i16TargetPixel = position.y * DISPLAY_COLUMNS +
			        position.x + u16Index +
			        (DISPLAY_COLUMNS - (int16_t)width) * (int16_t)u16SpriteY;

			/* Check we have not gone off the bottom of the display. If we have, there is nothing left to draw */
			if (i16TargetPixel < DISPLAY_PIXELS) {

                /* If we are attempting to draw to an x coordinate beyond the right hand edge of the display, ignore.
                 * To detect this, calculate the target y coordinate from the sprite array, and the y coordinate on the display.
                 * If we have wrapped around to a new row, these will be different.
                 */
                i16TargetYCoordinate = position.y + (int16_t)u16SpriteY;
                i16ActualYCoordinate = i16TargetPixel / DISPLAY_COLUMNS;

				/* Check we have not gone off the left, right, or top of the display, and that this isn't transparent.
				 * If it is, there might still be more drawing to do, so we should continue the loop.
				 */
				if ((i16TargetYCoordinate == i16ActualYCoordinate) &&
						(i16TargetPixel >= 0) &&
						(COLOUR_VISIBLE == sprite[u16Index].visible)) {

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
}
