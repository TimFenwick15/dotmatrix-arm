#ifndef ANIMATION_H_
#define ANIMATION_H_

void ANIMATION_vInit(void);

bool ANIMATION_bRegisterAnimation(uint8_t* id);

bool ANIMATION_bAddFrame(uint8_t id,
                         MAIN_tsColour* sprite,
						 uint16_t timeToNextFrame_ms);

MAIN_tsColour* ANIMATION_psGetFrame(uint8_t id);

bool ANIMATION_bAddMotion(uint8_t id,
		                  MAIN_tsPostion initialPosition,
						  MAIN_tsPostion finalPosition,
						  uint16_t time_ms);

MAIN_tsPostion ANIMATION_sGetPosition(uint8_t id);

bool ANIMATION_bAddColourTransition(uint8_t id,
		                            MAIN_tsColour initialColour,
		                            MAIN_tsColour finalColour,
		                            uint16_t time_ms);

MAIN_tsColour ANIMATION_sGetColour(uint8_t id);

#endif
