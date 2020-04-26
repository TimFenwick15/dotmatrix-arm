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
#include "BlinkLed.h"
#include "diag/Trace.h"
#include "LedMatrix.h"
#include "Graphics.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
#define INTERRUPT_PERIOD_300us (3 - 1) /* With a colour depth of 4, any slower than this and flicker becomes visible */
#define INTERRUPT_PERIOD_500us (5 - 1)
#define INTERRUPT_PERIOD_10ms (100 - 1)
#define INTERRUPT_PERIOD_500ms (5000 - 1)

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef    TimHandle;
TIM_OC_InitTypeDef   sConfig;

uint32_t uwPrescalerValue = 0;
uint32_t uwCapturedValue = 0;

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

  LEDMATRIX_vInit();

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
  uwPrescalerValue = (uint32_t) ((SystemCoreClock / 10000) - 1);
  
  /* Set TIMx instance */
  TimHandle.Instance = TIMx;
   
  /* Initialize TIM3 peripheral as follow:
       + Period = 10000 - 1
       + Prescaler = ((SystemCoreClock/2)/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period = INTERRUPT_PERIOD_300us;
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

  /*uint8_t sprite[2];
  sprite[0] = 0x1;
  sprite[1] = 0x1;
  GRAPHICS_vDraw(sprite, 0, 0, 9, 1);*/
  uint16_t x = 0;

  /* Infinite loop */
  while (1) {
	  if (LEDMATRIX_u32Counter_ms == 50) {
		  x++;
		  GRAPHICS_vDrawBox(GRAPHICS_tsRed   , 64 - (int16_t)((x +  0) % 128), 32 - (int16_t)((x +  0) % 64), 20, 20);
		  GRAPHICS_vDrawBox(GRAPHICS_tsGreen , 64 - (int16_t)((x +  8) % 128), (int16_t)((x +  8) % 64) - 32, 15, 30);
		  GRAPHICS_vDrawBox(GRAPHICS_tsBlue  , (int16_t)((34 - x) % 128) - 64, 32 - (int16_t)((x +  3) % 64), 12, 20);
		  GRAPHICS_vDrawBox(GRAPHICS_tsPurple, (int16_t)((54 - x) % 128) - 64, (int16_t)((x + 20) % 64) - 32, 32, 16);
		  GRAPHICS_vDrawBox(GRAPHICS_tsPink, 64 - (int16_t)((x + 68) % 128), 32 - (int16_t)((x + 40) % 64), 20, 20);
		  GRAPHICS_vDrawBox(GRAPHICS_tsWhite , 64 - (int16_t)((x + 40) % 128), (int16_t)((x + 50) % 64) - 32, 24, 24);
		  GRAPHICS_vDrawBox(GRAPHICS_tsRed   , (int16_t)((x +110) % 128) - 64, 32 - (int16_t)((x + 55) % 64), 32, 32);
		  GRAPHICS_vDrawBox(GRAPHICS_tsGreen , (int16_t)((x +120) % 128) - 64, (int16_t)((x + 60) % 64) - 32, 24, 24);
		  GRAPHICS_vDrawBox(GRAPHICS_tsBlue  , 64 - (int16_t)((x + 34) % 128), 32 - (int16_t)((x +  3) % 64), 12, 12);
		  GRAPHICS_vDrawBox(GRAPHICS_tsPurple, 64 - (int16_t)((x + 54) % 128), (int16_t)((x + 20) % 64) - 32, 16, 16);
		  GRAPHICS_vUpdate();
		  LEDMATRIX_u32Counter_ms = 0;
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
