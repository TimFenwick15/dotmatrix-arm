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

static uint8_t m_u8AnimationID = 0;
static uint8_t m_u8FrameCount[ANIMATION_MAX];
static MAIN_tsColour* m_psFrameData[ANIMATION_MAX][FRAME_MAX];
static uint16_t m_u16FrameTime[ANIMATION_MAX][FRAME_MAX];

void ANIMATION_vInit(void) {
	memset(m_u8FrameCount, 0, sizeof m_u8FrameCount);
}

bool ANIMATION_bRegisterAnimation(uint8_t* id) {
	uint8_t u8Result = false;
	if (m_u8AnimationID < ANIMATION_MAX) {
		*id = m_u8AnimationID;
		m_u8AnimationID++;
		u8Result = true;
	}
	return (u8Result);
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

bool ANIMATION_bAddMotion(uint8_t id) {
	return (false);
}
