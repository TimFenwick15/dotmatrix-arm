#ifndef SPRITE_H_
#define SPRITE_H_

/*
 * A set of short hands for drawing sprites
 */
#define RED (SPRITE_sRed)
#define GRN (SPRITE_sGreen)
#define BLU (SPRITE_sBlue)
#define PUR (SPRITE_sPurple)
#define YEL (SPRITE_sYellow)
#define WHT (SPRITE_sWhite)
#define PNK (SPRITE_sPink)
#define BLK (SPRITE_sBlack)
#define OFF (SPRITE_sTransparent)

#define RED_SIZE (16)
MAIN_tsColour SPRITE_sRed_0[RED_SIZE * RED_SIZE];
MAIN_tsColour SPRITE_sRed_1[RED_SIZE * RED_SIZE];
MAIN_tsColour SPRITE_sRed_2[RED_SIZE * RED_SIZE];

#define PIKACHU_SIZE (16)
MAIN_tsColour SPRITE_sPikachu_0[PIKACHU_SIZE * PIKACHU_SIZE];
MAIN_tsColour SPRITE_sPikachu_1[PIKACHU_SIZE * PIKACHU_SIZE];
MAIN_tsColour SPRITE_sPikachu_2[PIKACHU_SIZE * PIKACHU_SIZE];

void SPRITE_vInit(void);

#endif
