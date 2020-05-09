/*
 * Animation.c
 *
 *  Created on: 27 Apr 2020
 *      Author: Tim
 */

#include "main.h"
#include "Animation.h"
#include <string.h> /* memset */

#define ANIMATION_MAX (16) /* These are pretty arbitrary - this allows us to avoid heap allocation */
#define FRAME_MAX (4)

#define FRACTION_TO_PERCENTAGE (100)

#define COLOUR_CAST(x) ((uint16_t)x & 0x07)

static int32_t i32Blend(int32_t initial,
                        int32_t final,
                        int32_t percentageComplete);

static uint8_t m_u8AnimationID = 0;
static MAIN_tsColour* m_psPlaceholder;

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

/**
 * Prepare any data required before use of this module
 */
void ANIMATION_vInit(void) {
    m_psPlaceholder = &MAIN_sTransparent;
    memset(m_u8FrameCount, 0, sizeof m_u8FrameCount);
}

/**
 * Obtain an ID that can be used to specify an animation
 * @param id - a pointer to an ID variable that will be set if successfully allocated
 * @returns bool - true if successful, else false
 */
bool ANIMATION_bRegisterAnimation(uint8_t* id) {
	bool bResult = false;
	if (m_u8AnimationID < ANIMATION_MAX) {
		*id = m_u8AnimationID;
		m_u8AnimationID++; /* This order means m_u8AnimationID holds the ID count,
		                    * and can be given as an ID to the next registered animation.
		                    */
		bResult = true;
	}
	return (bResult);
}

/**
 * Add a frame to a registered animation
 * @param id - an ID alocated by ANIMATION_bRegisterAnimation
 * @param sprite - a pointer to the sprite to draw
 * @param timeToNextFrame_ms - the time before moving to the next frame, if there is one
 * @returns bool - true if successful, else false
 */
bool ANIMATION_bAddFrame(uint8_t id,
                         MAIN_tsColour* sprite,
						 uint16_t timeToNextFrame_ms) {
	bool bResult = false;
	if ((m_u8FrameCount[id] < FRAME_MAX) && (timeToNextFrame_ms > 0)) {
		m_psFrameData[id][ m_u8FrameCount[id] ] = sprite;
		m_u16FrameTime[id][ m_u8FrameCount[id] ] = timeToNextFrame_ms;

		/* The frame time wants to be a list of change times. EG, 100, 200, 300 etc.
		 * To get this counting up, we need to add the previous frame time to the one we just posted.
		 * Only if m_u8FrameCount > 0, if this is the first frame, we can post the time unmodified.
		 */
		if (m_u8FrameCount[id] > 0) {
			m_u16FrameTime[id][ m_u8FrameCount[id] ] +=
			        m_u16FrameTime[id][ m_u8FrameCount[id] - 1]; /* The previous frame time */
		}
		m_u8FrameCount[id]++;
		bResult = true;
	}
	return (bResult);
}

/**
 * Get a pointer to the frame that it is now time to display
 * @param id - the id of the animation we are asking for
 * @returns - a pointer to the sprite data
 */
MAIN_tsColour* ANIMATION_psGetFrame(uint8_t id) {
    MAIN_tsColour* sResult;
    if (m_u8FrameCount[id] > 0) {
        uint8_t u8Loop;
        uint16_t u16Time = MAIN_u32MainCounter_ms %
                m_u16FrameTime[id][m_u8FrameCount[id] - 1]; /* The -1 is to turn m_u8FrameCount into an array index.
                                                             * ie if we had 1 frame, that would be at index 0.
                                                             * We don't allow frames to be added with zero time, so the modulo should be safe.
                                                             */

        /* Loop over the frames until current time matches the frame time, then return that element */
        for (u8Loop = 0; u8Loop < m_u8FrameCount[id]; u8Loop++) {
            if (u16Time < m_u16FrameTime[id][u8Loop]) {
                break;
            }
        }
        sResult = m_psFrameData[id][u8Loop];
    } else {
        sResult = m_psPlaceholder;
    }
    return (sResult);
}

/**
 * Add a looping movement to this animation ID
 * @param id - the id of the animation we are asking for
 * @param initialPosition - where the sprite will start from
 * @param finalPosition - where the sprite will end up
 * @param time_ms - the time for the full movement
 * @returns bool - true if added successfully, else false
 */
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

/**
 * Get the position at this time of an element with a motion animation
 * @param id - the id of the animation we are asking for
 * @returns - the position of the element at this time
 */
MAIN_tsPosition ANIMATION_sGetPosition(uint8_t id) {
	MAIN_tsPosition sResult;
	uint32_t u32TimeIntoInterval;
	uint32_t u32PercentageComplete = 0;

    /* We don't allow zero times to be added, so this division should be safe */
	if (m_u16FinalPositionTime[id] > 0) {
	    u32TimeIntoInterval = MAIN_u32MainCounter_ms % ((uint32_t)m_u16FinalPositionTime[id]);
	    u32PercentageComplete = (u32TimeIntoInterval * FRACTION_TO_PERCENTAGE) / m_u16FinalPositionTime[id];
	}

	sResult = (MAIN_tsPosition) {
            (int16_t)(i32Blend((int32_t)m_sInitialPosition[id].x,
                               (int32_t)m_sFinalPosition[id].x,
                               (int32_t)u32PercentageComplete)),
            (int16_t)(i32Blend((int32_t)m_sInitialPosition[id].y,
                               (int32_t)m_sFinalPosition[id].y,
                               (int32_t)u32PercentageComplete))
	};

	return (sResult);
}

/**
 * Add a looping colour change to this animation ID
 * @param id - the id of the animation we are asking for
 * @param initialColour - the starting colour
 * @param finalColour - the ending colour
 * @param time_ms - the time for the full transition
 * @returns bool - true if added successfully, else false
 */
bool ANIMATION_bAddColourTransition(uint8_t id,
		                            MAIN_tsColour initialColour,
		                            MAIN_tsColour finalColour,
		                            uint16_t time_ms) {
	bool bResult = false;
	if ((id < m_u8AnimationID) && (time_ms > 0)) {
		m_sInitialColour[id] = initialColour;
		m_sFinalColour[id] = finalColour;
		m_u16FinalColourTime[id] = time_ms;
		bResult = true;
	}
	return (bResult);
}

/**
 * Get the colour at this time of an element with a colour animation
 * @param id - the id of the animation we are asking for
 * @returns - the colour of the element at this time
 */
MAIN_tsColour ANIMATION_sGetColour(uint8_t id) {
	MAIN_tsColour sResult;
	uint32_t u32TimeIntoInterval;
	uint32_t u32PercentageComplete = 0;

	/* It shouldn't be possible to pass negative numbers */
	if (m_u16FinalColourTime[id] > 0) {
        u32TimeIntoInterval = MAIN_u32MainCounter_ms %
                ((uint32_t)m_u16FinalColourTime[id]);
        u32PercentageComplete =
                (u32TimeIntoInterval * FRACTION_TO_PERCENTAGE) /
                        m_u16FinalColourTime[id];
	}

	sResult = (MAIN_tsColour) {
	    COLOUR_CAST(i32Blend((int32_t)m_sInitialColour[id].red,
                             (int32_t)m_sFinalColour[id].red,
                             (int32_t)u32PercentageComplete)),
        COLOUR_CAST(i32Blend((int32_t)m_sInitialColour[id].green,
                             (int32_t)m_sFinalColour[id].green,
                             (int32_t)u32PercentageComplete)),
        COLOUR_CAST(i32Blend((int32_t)m_sInitialColour[id].blue,
                             (int32_t)m_sFinalColour[id].blue,
                             (int32_t)u32PercentageComplete)),
        1
	};

	return (sResult);
}

/**
 * Find the point between a start and end point, a percentage of the way between
 * @param initial - the starting point
 * @param final - the ending point
 * @param percentageComplete - a percentage 0 - 100
 * @returns the numberical results in correct units
 */
static int32_t i32Blend(int32_t initial,
                        int32_t final,
                        int32_t percentageComplete) {
    return (initial + (final - initial) * percentageComplete / FRACTION_TO_PERCENTAGE);
}
