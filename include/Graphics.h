#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef enum
{
    GRAPHICS_eFontSize8x8 = 8,
    GRAPHICS_eFontSize16x16 = 16,
    GRAPHICS_eFontSize32x32 = 32,
} GRAPHICS_teFontSize;

void GRAPHICS_vInit(void);
void GRAPHICS_vUpdate(void);

void GRAPHICS_vDrawByColourArray(MAIN_tsColour* sprite,
		                         MAIN_tsPosition position,
		                         uint8_t width,
		                         uint8_t height);

void GRAPHICS_vDrawBox(MAIN_tsColour colour,
                       MAIN_tsPosition position,
				  	   uint8_t width,
				 	   uint8_t height);

void GRAPHICS_vDrawCircle(MAIN_tsColour colour,
                          MAIN_tsPosition position,
						  uint16_t radius);

void GRAPHICS_vDrawCharacter(MAIN_tsColour colour,
                             MAIN_tsPosition position,
                             unsigned char character,
                             GRAPHICS_teFontSize fontSize);

void GRAPHICS_vDrawNumber(MAIN_tsColour colour,
                          MAIN_tsPosition position,
                          uint32_t number,
                          uint8_t maximumDisplayDigits,
                          bool fillBlankDigitsWithZeros,
                          GRAPHICS_teFontSize fontSize);

#endif
