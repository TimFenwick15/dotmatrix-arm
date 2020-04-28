#ifndef GRAPHICS_H_
#define GRAPHICS_H_

void GRAPHICS_vInit(void);
void GRAPHICS_vUpdate(void);

void GRAPHICS_vDrawByColourArray(MAIN_tsColour* sprite,
		                         int8_t x,
		                         int8_t y,
		                         uint8_t width,
		                         uint8_t height);

void GRAPHICS_vDrawBox(MAIN_tsColour colour,
		               int8_t x,
				 	   int8_t y,
				  	   uint8_t width,
				 	   uint8_t height);

void GRAPHICS_vDrawCircle(MAIN_tsColour colour,
		                  int16_t x,
						  int16_t y,
						  uint16_t radius);
#endif
