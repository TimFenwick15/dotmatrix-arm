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
static uint8_t m_u8FrameCount[ANIMATION_MAX];
static MAIN_tsColour* m_psFrameData[ANIMATION_MAX][FRAME_MAX];
static uint16_t m_u16FrameTime[ANIMATION_MAX][FRAME_MAX];

/*
 * Move animation data
 */
static ANIMATION_tsPostion m_sInitialPosition[ANIMATION_MAX];
static ANIMATION_tsPostion m_sFinalPosition[ANIMATION_MAX];
static uint16_t m_u16FinalPositionTime[ANIMATION_MAX];

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
		                  int8_t x1,
						  int8_t y1,
						  int8_t x2,
						  int8_t y2,
						  uint16_t time_ms,
						  bool repeat,
						  ANIMATION_teMotion motionType) {
	bool bResult = false;
	if (id < m_u8AnimationID) {
		m_sInitialPosition[id].x = x1;
		m_sInitialPosition[id].y = y1;
		m_sFinalPosition[id].x = x2;
		m_sFinalPosition[id].y = y2;
		m_u16FinalPositionTime[id] = time_ms;
		bResult = true;
	}
	return (bResult);
}

ANIMATION_tsPostion ANIMATION_sGetPosition(uint8_t id) {
	ANIMATION_tsPostion sResult;
	uint32_t u32TimeIntoInterval = MAIN_u32MainCounter % ((uint32_t)m_u16FinalPositionTime[id]);
	uint32_t u32PercentageComplete = (u32TimeIntoInterval * FRACTION_TO_PERCENTAGE) / m_u16FinalPositionTime[id];
	sResult.x = m_sInitialPosition[id].x + (int8_t)(((int32_t)m_sFinalPosition[id].x - (int32_t)m_sInitialPosition[id].x) * (int32_t)(u32PercentageComplete) / FRACTION_TO_PERCENTAGE);
	sResult.y = m_sInitialPosition[id].y + (int8_t)(((int32_t)m_sFinalPosition[id].y - (int32_t)m_sInitialPosition[id].y) * (int32_t)(u32PercentageComplete) / FRACTION_TO_PERCENTAGE);
	return (sResult);
}
