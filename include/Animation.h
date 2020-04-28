#ifndef ANIMATION_H_
#define ANIMATION_H_

typedef struct {
	int8_t x;
	int8_t y;
} ANIMATION_tsPostion;

typedef enum {
	ANIMATION_eMotionLinear,
	ANIMATION_eMotionSmooth, // there's a name for this
	ANIMATION_eMotionArc,
	ANIMATION_eMotionRandom,
	ANIMATION_eMotionStop // do we need this?
} ANIMATION_teMotion;

void ANIMATION_vInit(void);

bool ANIMATION_bRegisterAnimation(uint8_t* id);

bool ANIMATION_bAddFrame(uint8_t id,
                         MAIN_tsColour* sprite,
						 uint16_t timeToNextFrame_ms);

MAIN_tsColour* ANIMATION_psGetFrame(uint8_t id);

bool ANIMATION_bAddMotion(uint8_t id,
		                  int8_t x1,
						  int8_t y1,
						  int8_t x2,
						  int8_t y2,
						  uint16_t time_ms,
						  bool repeat,
						  ANIMATION_teMotion motionType);

ANIMATION_tsPostion ANIMATION_sGetPosition(uint8_t id);

bool ANIMATION_bAddColourTransition(uint8_t id,
		                            MAIN_tsColour initialColour,
		                            MAIN_tsColour finalColour,
		                            uint16_t time_ms);

MAIN_tsColour ANIMATION_sGetColour(uint8_t id);

#endif
