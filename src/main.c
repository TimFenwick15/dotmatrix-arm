/**
  ******************************************************************************
  * @file    TIM/TIM_TimeBase/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F4xx TIM HAL API to generate
  *          4 signals in PWM.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "diag/Trace.h"

#include "Graphics.h"
#include "Sprite.h"
#include "Animation.h"
#include "UserInput.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
#define TIMx_IRQHandler                TIM3_IRQHandler

#define INTERRUPT_PERIOD_300us (3 - 1) /* With a colour depth of 4, any slower than this and flicker becomes visible */
#define INTERRUPT_PERIOD_500us (5 - 1)
#define INTERRUPT_PERIOD_10ms (100 - 1)
#define INTERRUPT_PERIOD_500ms (5000 - 1)
#define INTERRUPT_PERIOD (INTERRUPT_PERIOD_300us)
#define CLOCK_FREQUENCY (10000) /* 10KHz, or 100us */
#define SECONDS_TO_MS (1000)
#define SCREEN_UPDATE_TIME_MS (30)

#define CALL(x) if(!x)Error_Handler();

typedef enum {
    m_eState0,
    m_eState1,
    m_eState2,
    m_eStateMax
} m_teState;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef    TimHandle;
TIM_OC_InitTypeDef   sConfig;

uint32_t uwPrescalerValue = 0;
uint32_t uwCapturedValue = 0;

static bool m_bButtonState = false;
static m_teState m_eState = m_eState0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  if(HAL_Init()!= HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler(); 
  }

  /* Configure the system clock to 100 MHz */
  SystemClock_Config();

  GRAPHICS_vInit();

  SPRITE_vInit();

  ANIMATION_vInit();

  USERINPUT_vInit();

 /*##-1- Configure the TIM peripheral #######################################*/ 
  /* -----------------------------------------------------------------------
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 2 
      => TIM3CLK = HCLK = SystemCoreClock
    To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
    Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    Prescaler = (SystemCoreClock /10 KHz) - 1
       
    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency  
  ----------------------------------------------------------------------- */  
  
  /* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz */ // 100us
  uwPrescalerValue = (uint32_t) ((SystemCoreClock / CLOCK_FREQUENCY) - 1);
  
  /* Set TIMx instance */
  TimHandle.Instance = TIMx;
  TimHandle.Init.Period = INTERRUPT_PERIOD;
  TimHandle.Init.Prescaler = uwPrescalerValue;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */
  if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }

  MAIN_u32MainCounter = 0;
  MAIN_u32MainCounter_ms = 0;

  /*
   * Draw things
   */
  uint8_t u8RedId;
  MAIN_tsPosition sRedInitialPosition = {24, -16};
  MAIN_tsPosition sRedFinalPosition = {24, 48};
  CALL(ANIMATION_bRegisterAnimation(&u8RedId));
  CALL(ANIMATION_bAddFrame(u8RedId, SPRITE_sRed_0, 180)); /* Because units, ~300ms */
  CALL(ANIMATION_bAddFrame(u8RedId, SPRITE_sRed_1, 180));
  CALL(ANIMATION_bAddFrame(u8RedId, SPRITE_sRed_0, 180));
  CALL(ANIMATION_bAddFrame(u8RedId, SPRITE_sRed_2, 180));
  CALL(ANIMATION_bAddMotion(u8RedId, sRedInitialPosition,
		  sRedFinalPosition, 4000));

  uint8_t u8PikachuId;
  MAIN_tsPosition sPikachuInitialPosition = {24, -32};
  MAIN_tsPosition sPikachuFinalPosition = {24, 32};
  CALL(ANIMATION_bRegisterAnimation(&u8PikachuId));
  CALL(ANIMATION_bAddFrame(u8PikachuId, SPRITE_sPikachu_0, 180));
  CALL(ANIMATION_bAddFrame(u8PikachuId, SPRITE_sPikachu_1, 180));
  CALL(ANIMATION_bAddFrame(u8PikachuId, SPRITE_sPikachu_0, 180));
  CALL(ANIMATION_bAddFrame(u8PikachuId, SPRITE_sPikachu_2, 180));
  CALL(ANIMATION_bAddMotion(u8PikachuId, sPikachuInitialPosition,
		  sPikachuFinalPosition, 4000));

  uint8_t u8CircleId1;
  MAIN_tsPosition sCircle1InitialPosition = {0, 32};
  MAIN_tsPosition sCircle1FinalPosition = {0, -16};
  CALL(ANIMATION_bRegisterAnimation(&u8CircleId1));
  CALL(ANIMATION_bAddMotion(u8CircleId1, sCircle1InitialPosition,
		  sCircle1FinalPosition, 3000));
  CALL(ANIMATION_bAddColourTransition(u8CircleId1, MAIN_sBlue,
		  MAIN_sRed, 6000));

  uint8_t u8CircleId2;
  MAIN_tsPosition sCircle2InitialPosition = {16, 32};
  MAIN_tsPosition sCircle2FinalPosition = {16, -16};
  CALL(ANIMATION_bRegisterAnimation(&u8CircleId2));
  CALL(ANIMATION_bAddMotion(u8CircleId2, sCircle2InitialPosition,
		  sCircle2FinalPosition, 1500));
  CALL(ANIMATION_bAddColourTransition(u8CircleId2, MAIN_sPurple,
		  MAIN_sGreen, 12000));

  uint8_t u8CircleId3;
  MAIN_tsPosition sCircle3InitialPosition = {32, 32};
  MAIN_tsPosition sCircle3FinalPosition = {32, -16};
  CALL(ANIMATION_bRegisterAnimation(&u8CircleId3));
  CALL(ANIMATION_bAddMotion(u8CircleId3, sCircle3InitialPosition, sCircle3FinalPosition, 10000));
  CALL(ANIMATION_bAddColourTransition(u8CircleId3, MAIN_sGreen,
		  MAIN_sRed, 5000));

  uint8_t u8CircleId4;
  MAIN_tsPosition sCircle4InitialPosition = {48, 32};
  MAIN_tsPosition sCircle4FinalPosition = {48, -16};
  CALL(ANIMATION_bRegisterAnimation(&u8CircleId4));
  CALL(ANIMATION_bAddMotion(u8CircleId4, sCircle4InitialPosition,
		  sCircle4FinalPosition, 2000));
  CALL(ANIMATION_bAddColourTransition(u8CircleId4, MAIN_sBlue,
		  MAIN_sYellow, 2000));

  uint8_t u8AgumonId;
  MAIN_tsPosition sAgumonPosition = {24, 8};
  CALL(ANIMATION_bRegisterAnimation(&u8AgumonId));
  CALL(ANIMATION_bAddFrame(u8AgumonId, SPRITE_sAgumon_0, 500));
  CALL(ANIMATION_bAddFrame(u8AgumonId, SPRITE_sAgumon_1, 500));
  CALL(ANIMATION_bAddFrame(u8AgumonId, SPRITE_sAgumon_0, 500));
  CALL(ANIMATION_bAddFrame(u8AgumonId, SPRITE_sAgumon_2, 500));

  uint8_t u8Digimon1Id;
  MAIN_tsPosition sDigimon1Position = {0, 8};
  CALL(ANIMATION_bRegisterAnimation(&u8Digimon1Id));
  CALL(ANIMATION_bAddFrame(u8Digimon1Id, SPRITE_sDigimon1_0, 500));
  CALL(ANIMATION_bAddFrame(u8Digimon1Id, SPRITE_sDigimon1_1, 500));


  uint8_t u8EggId;
  MAIN_tsPosition sEggInitialPosition = {48, 8};
  MAIN_tsPosition sEggFinalPosition = {50, 8};
  CALL(ANIMATION_bRegisterAnimation(&u8EggId));
  CALL(ANIMATION_bAddMotion(u8EggId, sEggInitialPosition,
          sEggFinalPosition, 1000));

  uint8_t u8DigimonBackgroundGlyphs0;
  MAIN_tsPosition sDigimonBackgroundGlyphs0InitialPosition = {0, -50};
  MAIN_tsPosition sDigimonBackgroundGlyphs0FinalPosition = {0, 44};
  CALL(ANIMATION_bRegisterAnimation(&u8DigimonBackgroundGlyphs0));
  CALL(ANIMATION_bAddMotion(u8DigimonBackgroundGlyphs0, sDigimonBackgroundGlyphs0InitialPosition,
          sDigimonBackgroundGlyphs0FinalPosition, 3000));

  uint8_t u8DigimonBackgroundGlyphs1;
  MAIN_tsPosition sDigimonBackgroundGlyphs1InitialPosition = {15, -30};
  MAIN_tsPosition sDigimonBackgroundGlyphs1FinalPosition = {15, 64};
  CALL(ANIMATION_bRegisterAnimation(&u8DigimonBackgroundGlyphs1));
  CALL(ANIMATION_bAddMotion(u8DigimonBackgroundGlyphs1, sDigimonBackgroundGlyphs1InitialPosition,
          sDigimonBackgroundGlyphs1FinalPosition, 3300));

  uint8_t u8DigimonBackgroundGlyphs2;
  MAIN_tsPosition sDigimonBackgroundGlyphs2InitialPosition = {35, -62};
  MAIN_tsPosition sDigimonBackgroundGlyphs2FinalPosition = {35, 32};
  CALL(ANIMATION_bRegisterAnimation(&u8DigimonBackgroundGlyphs2));
  CALL(ANIMATION_bAddMotion(u8DigimonBackgroundGlyphs2, sDigimonBackgroundGlyphs2InitialPosition,
          sDigimonBackgroundGlyphs2FinalPosition, 3100));

  uint8_t u8DigimonBackgroundGlyphs3;
  MAIN_tsPosition sDigimonBackgroundGlyphs3InitialPosition = {50, -45};
  MAIN_tsPosition sDigimonBackgroundGlyphs3FinalPosition = {50, 49};
  CALL(ANIMATION_bRegisterAnimation(&u8DigimonBackgroundGlyphs3));
  CALL(ANIMATION_bAddMotion(u8DigimonBackgroundGlyphs3, sDigimonBackgroundGlyphs3InitialPosition,
          sDigimonBackgroundGlyphs3FinalPosition, 2800));

  uint32_t lastIncrease = 0;
  uint32_t displayNumber = 990;
  uint8_t u8DisplayNumber;
  CALL(ANIMATION_bRegisterAnimation(&u8DisplayNumber));
  CALL(ANIMATION_bAddColourTransition(u8DisplayNumber, MAIN_sBlue,
          MAIN_sRed, 10000));

  /* Infinite loop */
  while (1) {
      MAIN_u32MainCounter_ms = MAIN_u32MainCounter * (INTERRUPT_PERIOD + 1) *
              SECONDS_TO_MS / CLOCK_FREQUENCY;
      bool bPreviousButtonState = m_bButtonState;
      m_bButtonState = USERINPUT_bPollButton();

      /* We only want to know when the button was first pressed */
      if ((true == m_bButtonState) && (false == bPreviousButtonState)) {
          m_eState++;
          if (m_eState >= m_eStateMax) {
              m_eState = m_eState0;
          }
      }

	  if ((MAIN_u32MainCounter_ms % SCREEN_UPDATE_TIME_MS) == 0) { /* 100 * 300us ~ 30ms. Screen redraws in about 5ms */
	      switch (m_eState) {
          case m_eState0:
              //if (MAIN_u32MainCounter_ms - lastIncrease >= 1000)
              if (MAIN_u32MainCounter_ms - lastIncrease >= 200)
              {
                  lastIncrease = MAIN_u32MainCounter_ms;
                  displayNumber++;
              }
              GRAPHICS_vDrawNumber(ANIMATION_sGetColour(u8DisplayNumber), (MAIN_tsPosition){0,8}, displayNumber, 4, true, GRAPHICS_eFontSize16x16);
              //GRAPHICS_vDrawNumber(ANIMATION_sGetColour(u8DisplayNumber), (MAIN_tsPosition){0,0}, displayNumber, 2, true, GRAPHICS_eFontSize32x32);
              //GRAPHICS_vDrawNumber(ANIMATION_sGetColour(u8DisplayNumber), (MAIN_tsPosition){0,0}, displayNumber, 7, true, GRAPHICS_eFontSize8x8);
              GRAPHICS_vDrawCharacter(ANIMATION_sGetColour(u8DisplayNumber), (MAIN_tsPosition){29,13}, ':', GRAPHICS_eFontSize8x8);
              break;
          case m_eState1:
              GRAPHICS_vDrawCircle(ANIMATION_sGetColour(u8CircleId1),
                      ANIMATION_sGetPosition(u8CircleId1), 8);
              GRAPHICS_vDrawCircle(ANIMATION_sGetColour(u8CircleId2),
                      ANIMATION_sGetPosition(u8CircleId2), 8);
              GRAPHICS_vDrawCircle(ANIMATION_sGetColour(u8CircleId3),
                      ANIMATION_sGetPosition(u8CircleId3), 8);
              GRAPHICS_vDrawCircle(ANIMATION_sGetColour(u8CircleId4),
                      ANIMATION_sGetPosition(u8CircleId4), 8);
              GRAPHICS_vDrawByColourArray(ANIMATION_psGetFrame(u8RedId),
                      ANIMATION_sGetPosition(u8RedId), RED_SIZE, RED_SIZE);
              GRAPHICS_vDrawByColourArray(ANIMATION_psGetFrame(u8PikachuId),
                      ANIMATION_sGetPosition(u8PikachuId), PIKACHU_SIZE, PIKACHU_SIZE);
              break;
          case m_eState2:
              GRAPHICS_vDrawByColourArray(SPRITE_sBackground,
                      ANIMATION_sGetPosition(u8DigimonBackgroundGlyphs0),
                      BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
              GRAPHICS_vDrawByColourArray(SPRITE_sBackground,
                      ANIMATION_sGetPosition(u8DigimonBackgroundGlyphs1),
                      BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
              GRAPHICS_vDrawByColourArray(SPRITE_sBackground,
                      ANIMATION_sGetPosition(u8DigimonBackgroundGlyphs2),
                      BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
              GRAPHICS_vDrawByColourArray(SPRITE_sBackground,
                      ANIMATION_sGetPosition(u8DigimonBackgroundGlyphs3),
                      BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
              GRAPHICS_vDrawByColourArray(ANIMATION_psGetFrame(u8AgumonId),
                      sAgumonPosition, AGUMON_SIZE, AGUMON_SIZE);
              GRAPHICS_vDrawByColourArray(ANIMATION_psGetFrame(u8Digimon1Id),
                      sDigimon1Position, DIGIMON_1_WIDTH, DIGIMON_1_HEIGHT);
              GRAPHICS_vDrawByColourArray(SPRITE_sEgg,
                      ANIMATION_sGetPosition(u8EggId), EGG_SIZE, EGG_SIZE);
              break;
          default:
              break;
          }
		  GRAPHICS_vUpdate();
	  }
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  trace_printf("Error handler called");
  while(1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 400
  *            PLL_P                          = 4
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale2 mode
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  
  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
