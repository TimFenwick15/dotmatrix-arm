#ifndef GRAPHICS_H_
#define GRAPHICS_H_

void GRAPHICS_vInit(void);
void GRAPHICS_vUpdate(void);

void GRAPHICS_vDrawByColourArray(MAIN_tsColour* sprite,
		                         MAIN_tsPostion position,
		                         uint8_t width,
		                         uint8_t height);

void GRAPHICS_vDrawBox(MAIN_tsColour colour,
                       MAIN_tsPostion position,
				  	   uint8_t width,
				 	   uint8_t height);

void GRAPHICS_vDrawCircle(MAIN_tsColour colour,
                          MAIN_tsPostion position,
						  uint16_t radius);
#endif
