#ifndef SPRITE_H_
#define SPRITE_H_

MAIN_tsColour GRAPHICS_tsRed;
MAIN_tsColour GRAPHICS_tsGreen;
MAIN_tsColour GRAPHICS_tsBlue;
MAIN_tsColour GRAPHICS_tsPurple;
MAIN_tsColour GRAPHICS_tsWhite;
MAIN_tsColour GRAPHICS_tsPink;
MAIN_tsColour GRAPHICS_tsBlack;

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

#define RED_SIZE (16)
MAIN_tsColour SPRITE_sRed_0[RED_SIZE * RED_SIZE];
MAIN_tsColour SPRITE_sRed_1[RED_SIZE * RED_SIZE];
MAIN_tsColour SPRITE_sRed_2[RED_SIZE * RED_SIZE];

void SPRITE_vInit(void);

#endif
