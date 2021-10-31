/**
  ******************************************************************************
  * @file    TIM/TIM_TimeBase/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/
typedef struct {
	uint16_t red:3;
	uint16_t green:3;
	uint16_t blue:3;
	uint16_t visible:1;
} MAIN_tsColour;

typedef struct {
	int16_t x;
	int16_t y;
} MAIN_tsPosition;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* User can use this section to tailor TIMx instance used and associated 
   resources */
/* Definition for TIMx clock resources */
#define TIMx                           TIM3
#define TIMx_CLK_ENABLE                __HAL_RCC_TIM3_CLK_ENABLE

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM3_IRQn
#define TIMx_IRQHandler                TIM3_IRQHandler

#define BUFFER_DEPTH (2)
#define COLOUR_DEPTH (4) /* To get colours somewhere between on and off, we need to be able to turn the colour on and off to dim it.
                          * The maximum "colour depth" we support is 4, so 5 unique levels: 0%, 25%, 50%, 75%, and 100%.
                          * Note increasing the colour depth means another buffer; 1024 bytes per buffer, with a front and back buffer.
                          * So at COLOUR_DEPTH 4, we're at 1024 bytes * 2 * 4 ~= 8KB.
                          *
                          * We could also pack the colours into a buffer, and convert to an 0b**BGRBGR value when we clock the data out to the display.
                          * Our 8K of buffers contains the data pre-calculated to be in this format. The current method is code speed > code space.
                          */

#define COLOUR_BRIGHTNESS_100_PERCENT (4) /* This magically knows about the colour depth */
#define COLOUR_BRIGHTNESS_75_PERCENT  (3)
#define COLOUR_BRIGHTNESS_50_PERCENT  (2)
#define COLOUR_BRIGHTNESS_25_PERCENT  (1)
#define COLOUR_BRIGHTNESS_0_PERCENT   (0)
#define COLOUR_VISIBLE (1)
#define COLOUR_INVISIBLE (0)

/* Exported functions ------------------------------------------------------- */

MAIN_tsColour MAIN_sRed;
MAIN_tsColour MAIN_sGreen;
MAIN_tsColour MAIN_sBlue;
MAIN_tsColour MAIN_sPurple;
MAIN_tsColour MAIN_sYellow;
MAIN_tsColour MAIN_sWhite;
MAIN_tsColour MAIN_sPink;
MAIN_tsColour MAIN_sBlack;
MAIN_tsColour MAIN_sTransparent;

uint32_t MAIN_u32MainCounter;
uint32_t MAIN_u32MainCounter_ms;
uint8_t* MAIN_pau8Buffer[COLOUR_DEPTH]; /* The buffers in Graphics.c are 2D arrays */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
