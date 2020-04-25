#ifndef GRAPHICS_H_
#define GRAPHICS_H_

/* graphics shouldn't know about the display size, but it will for now... */
#define DISPLAY_COLUMNS (64)
#define DISPLAY_ROWS (32)
#define DISPLAY_PIXELS (DISPLAY_COLUMNS * DISPLAY_ROWS)
#define DISPLAY_INDICES (DISPLAY_PIXELS / 2)

#define BUFFER_DEPTH (2)

uint8_t* GRAPHICS_pu8Buffer;

void GRAPHICS_vInit(void);
void GRAPHICS_vUpdate(void);

uint8_t GRAPHICS_vDraw(uint8_t* sprite,
		              int8_t x,
			          int8_t y,
			          uint8_t width,
			          uint8_t height);

uint8_t GRAPHICS_vDrawBox(uint8_t colour,
		                  int8_t x,
						  int8_t y,
						  uint8_t width,
						  uint8_t height);
#endif
