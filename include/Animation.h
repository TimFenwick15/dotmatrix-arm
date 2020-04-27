#ifndef ANIMATION_H_
#define ANIMATION_H_

void ANIMATION_vInit(void);

bool ANIMATION_bRegisterAnimation(uint8_t* id);

bool ANIMATION_bAddFrame(uint8_t id,
                         MAIN_tsColour* sprite,
						 uint16_t timeToNextFrame_ms);

bool ANIMATION_bAddMotion(uint8_t id);

MAIN_tsColour* ANIMATION_psGetFrame(uint8_t id);

#endif
