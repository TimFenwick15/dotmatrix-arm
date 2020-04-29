/*
 * Animation.c
 *
 *  Created on: 27 Apr 2020
 *      Author: Tim
 */

#include "main.h"
#include "Animation.h"
#include <string.h> /* memset */

#define ANIMATION_MAX (8) /* These are pretty arbitrary */
#define FRAME_MAX (4)

#define FRACTION_TO_PERCENTAGE (100)

static uint8_t m_u8AnimationID = 0;

/*
 * Frame animation data
 */
static uint8_t m_u8FrameCount[ANIMATION_MAX];
static MAIN_tsColour* m_psFrameData[ANIMATION_MAX][FRAME_MAX];
static uint16_t m_u16FrameTime[ANIMATION_MAX][FRAME_MAX];

/*
 * Move animation data
 */
static MAIN_tsPosition m_sInitialPosition[ANIMATION_MAX];
static MAIN_tsPosition m_sFinalPosition[ANIMATION_MAX];
static uint16_t m_u16FinalPositionTime[ANIMATION_MAX];


/*
 * Colour animation data
 */
static MAIN_tsColour m_sInitialColour[ANIMATION_MAX];
static MAIN_tsColour m_sFinalColour[ANIMATION_MAX];
static uint16_t m_u16FinalColourTime[ANIMATION_MAX];


void ANIMATION_vInit(void) {
	memset(m_u8FrameCount, 0, sizeof m_u8FrameCount);
}

bool ANIMATION_bRegisterAnimation(uint8_t* id) {
	bool bResult = false;
	if (m_u8AnimationID < ANIMATION_MAX) {
		*id = m_u8AnimationID;
		m_u8AnimationID++;
		bResult = true;
	}
	return (bResult);
}

bool ANIMATION_bAddFrame(uint8_t id,
                         MAIN_tsColour* sprite,
						 uint16_t timeToNextFrame_ms) {
	bool bResult = false;
	if (m_u8FrameCount[id] < FRAME_MAX) {
		m_psFrameData[id][ m_u8FrameCount[id] ] = sprite;
		m_u16FrameTime[id][ m_u8FrameCount[id] ] = timeToNextFrame_ms;
		if (m_u8FrameCount[id] > 0) {
			m_u16FrameTime[id][ m_u8FrameCount[id] ] += m_u16FrameTime[id][ m_u8FrameCount[id] - 1];
		}
		m_u8FrameCount[id]++;
		bResult = true;
	}
	return (bResult);
}

MAIN_tsColour* ANIMATION_psGetFrame(uint8_t id) {
	uint8_t u8Loop;
	uint16_t u16Time = MAIN_u32MainCounter % m_u16FrameTime[id][m_u8FrameCount[id] - 1];
	for (u8Loop = 0; u8Loop < m_u8FrameCount[id]; u8Loop++) {
		if (u16Time < m_u16FrameTime[id][u8Loop]) {
			break;
		}
	}
	return (m_psFrameData[id][u8Loop]);
}

bool ANIMATION_bAddMotion(uint8_t id,
		                  MAIN_tsPosition initialPosition,
                          MAIN_tsPosition finalPosition,
						  uint16_t time_ms) {
	bool bResult = false;
	if ((id < m_u8AnimationID) && (time_ms > 0)) {
		m_sInitialPosition[id] = initialPosition;
		m_sFinalPosition[id] = finalPosition;
		m_u16FinalPositionTime[id] = time_ms;
		bResult = true;
	}
	return (bResult);
}

MAIN_tsPosition ANIMATION_sGetPosition(uint8_t id) {
	MAIN_tsPosition sResult;
	uint32_t u32TimeIntoInterval = MAIN_u32MainCounter % ((uint32_t)m_u16FinalPositionTime[id]);
	uint32_t u32PercentageComplete = (u32TimeIntoInterval * FRACTION_TO_PERCENTAGE) / m_u16FinalPositionTime[id];
	sResult.x = m_sInitialPosition[id].x + (int8_t)(((int32_t)m_sFinalPosition[id].x - (int32_t)m_sInitialPosition[id].x) * (int32_t)(u32PercentageComplete) / FRACTION_TO_PERCENTAGE);
	sResult.y = m_sInitialPosition[id].y + (int8_t)(((int32_t)m_sFinalPosition[id].y - (int32_t)m_sInitialPosition[id].y) * (int32_t)(u32PercentageComplete) / FRACTION_TO_PERCENTAGE);
	return (sResult);
}

bool ANIMATION_bAddColourTransition(uint8_t id,
		                            MAIN_tsColour initialColour,
		                            MAIN_tsColour finalColour,
		                            uint16_t time_ms) {
	bool bResult = false;
	if (id < m_u8AnimationID) {
		m_sInitialColour[id] = initialColour;
		m_sFinalColour[id] = finalColour;
		m_u16FinalColourTime[id] = time_ms;
		bResult = true;
	}
	return (bResult);
}

MAIN_tsColour ANIMATION_sGetColour(uint8_t id) {
	MAIN_tsColour sResult;
	uint32_t u32TimeIntoInterval = MAIN_u32MainCounter % ((uint32_t)m_u16FinalColourTime[id]);
	uint32_t u32PercentageComplete = (u32TimeIntoInterval * FRACTION_TO_PERCENTAGE) / m_u16FinalColourTime[id];
	sResult.red = m_sInitialColour[id].red + ((int32_t)m_sFinalColour[id].red - (int32_t)m_sInitialColour[id].red) * (int32_t)(u32PercentageComplete) / FRACTION_TO_PERCENTAGE;
	sResult.green = m_sInitialColour[id].green + ((int32_t)m_sFinalColour[id].green - (int32_t)m_sInitialColour[id].green) * (int32_t)(u32PercentageComplete) / FRACTION_TO_PERCENTAGE;
	sResult.blue = m_sInitialColour[id].blue + ((int32_t)m_sFinalColour[id].blue - (int32_t)m_sInitialColour[id].blue) * (int32_t)(u32PercentageComplete) / FRACTION_TO_PERCENTAGE;
	sResult.visible = 1;
	return (sResult);
}
