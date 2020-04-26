#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#define COLOUR_DEPTH (4) /* To get colours somewhere between on and off, we need to be able to turn the colour on and off to dim it.
                          * The maximum "colour depth" we support is 4, so 5 unique levels: 0%, 25%, 50%, 75%, and 100%.
                          * Note increasing the colour depth means another buffer; 1024 bytes per buffer, with a front and back buffer.
                          * So at COLOUR_DEPTH 4, we're at 1024 bytes * 2 * 4 ~= 8KB.
                          *
                          * We could also pack the colours into a buffer, and convert to an 0b**BGRBGR value when we clock the data out to the display.
                          * Our 8K of buffers contains the data pre-calculated to be in this format. The current method is code speed > code space.
                          */

/* graphics shouldn't know about the display size, but it will for now... */
#define DISPLAY_COLUMNS (64)
#define DISPLAY_ROWS (32)
#define DISPLAY_PIXELS (DISPLAY_COLUMNS * DISPLAY_ROWS)
#define DISPLAY_INDICES (DISPLAY_PIXELS / 2)

#define BUFFER_DEPTH (2)

typedef struct {
	uint16_t red:3;
	uint16_t green:3;
	uint16_t unused_1:2; /* This unused lines the data up at the 8-bit boundaries. This probably isn't necessary, it may at least be more portable */
	uint16_t blue:3;
	uint16_t unused_2:5;
} GRAPHICS_tsColour;

GRAPHICS_tsColour GRAPHICS_tsRed;
GRAPHICS_tsColour GRAPHICS_tsGreen;
GRAPHICS_tsColour GRAPHICS_tsBlue;
GRAPHICS_tsColour GRAPHICS_tsPurple;
GRAPHICS_tsColour GRAPHICS_tsWhite;
GRAPHICS_tsColour GRAPHICS_tsPink;
GRAPHICS_tsColour GRAPHICS_tsBlack;

/*
 * A set of short hands for drawing sprites
 */
#define RED (GRAPHICS_tsRed)
#define GRN (GRAPHICS_tsGreen)
#define BLU (GRAPHICS_tsBlue)
#define PUR (GRAPHICS_tsPurple)
#define WHT (GRAPHICS_tsWhite)
#define PNK (GRAPHICS_tsPink)
#define BLK (GRAPHICS_tsBlack)

uint8_t* GRAPHICS_pau8Buffer[COLOUR_DEPTH]; /* The buffers in Graphics.c are 2D arrays */

void GRAPHICS_vInit(void);
void GRAPHICS_vUpdate(void);

uint8_t GRAPHICS_vDrawByColourArray(GRAPHICS_tsColour* sprite,
		                            int8_t x,
			                        int8_t y,
			                        uint8_t width,
			                        uint8_t height);

uint8_t GRAPHICS_vDrawBox(GRAPHICS_tsColour colour,
		                  int8_t x,
						  int8_t y,
						  uint8_t width,
						  uint8_t height);
#endif
