/*
 * i2c.c
 *
 *  Created on: 3 Nov 2021
 *      Author: Tim
 */
//#include "stm32f4xx_hal_i2c.h"
#include "main.h"
#include "i2c.h"
#include <string.h> /* memset */

#define TXBUFFERSIZE (1)
#define RXBUFFERSIZE (8)
#define I2C_ADDRESS_SELF (0x30F)
#define I2C_ADDRESS_SECONDS (0x0)

I2C_HandleTypeDef I2cHandle;
uint8_t aTxBuffer[TXBUFFERSIZE] = { 0 };
uint8_t aRxBuffer[RXBUFFERSIZE];

static void I2Cx_MspInit(I2C_HandleTypeDef *hi2c)
{

#define DISCOVERY_I2Cx_SDA_PIN GPIO_PIN_9
#define DISCOVERY_I2Cx_SCL_PIN GPIO_PIN_6
#define DISCOVERY_I2Cx_AF GPIO_AF4_I2C1
#define DISCOVERY_I2Cx_GPIO_PORT GPIOB
#define DISCOVERY_I2Cx_EV_IRQn I2C1_EV_IRQn
#define DISCOVERY_I2Cx_ER_IRQn I2C1_ER_IRQn
#define DISCOVERY_I2Cx_CLOCK_ENABLE()           __HAL_RCC_I2C1_CLK_ENABLE()
#define DISCOVERY_I2Cx_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define DISCOVERY_I2Cx_FORCE_RESET()            __HAL_RCC_I2C1_FORCE_RESET()
#define DISCOVERY_I2Cx_RELEASE_RESET()          __HAL_RCC_I2C1_RELEASE_RESET()
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the I2C peripheral */
  DISCOVERY_I2Cx_CLOCK_ENABLE();

  /* Enable SCK and SDA GPIO clocks */
  DISCOVERY_I2Cx_GPIO_CLK_ENABLE();

  /* I2Cx SD1 & SCK pin configuration */
  GPIO_InitStructure.Pin = DISCOVERY_I2Cx_SDA_PIN | DISCOVERY_I2Cx_SCL_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  //GPIO_InitStructure.Pull = GPIO_PULLUP;
  //GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStructure.Alternate = DISCOVERY_I2Cx_AF;

  HAL_GPIO_Init(DISCOVERY_I2Cx_GPIO_PORT, &GPIO_InitStructure);

  /* Force the I2C peripheral clock reset */
  DISCOVERY_I2Cx_FORCE_RESET();

  /* Release the I2C peripheral clock reset */
  DISCOVERY_I2Cx_RELEASE_RESET();

  /* Enable and set I2Cx Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(DISCOVERY_I2Cx_EV_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(DISCOVERY_I2Cx_EV_IRQn);

  /* Enable and set I2Cx Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(DISCOVERY_I2Cx_ER_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(DISCOVERY_I2Cx_ER_IRQn);
}

bool I2C_vInit(void)
{
    bool bResult = false;
    I2cHandle.Instance             = I2C1;
    I2cHandle.Init.ClockSpeed      = 400000;
    I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_2;
    I2cHandle.Init.OwnAddress1     = 0x43;
    I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2cHandle.Init.OwnAddress2     = 0;
    I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

    I2Cx_MspInit(&I2cHandle);
    volatile HAL_StatusTypeDef initresult = HAL_I2C_Init(&I2cHandle);
    if (initresult == HAL_OK)
    {
        bResult = true;
    }

    return (bResult);
}

void I2C_vWriteMemory(uint8_t u8MemoryAddress, uint8_t* pu8Buffer, uint8_t u8Length)
{
    HAL_I2C_Mem_Write(&I2cHandle, 0xD0, u8MemoryAddress, 1, pu8Buffer, u8Length, HAL_MAX_DELAY);
}

void I2C_vReadMemory(uint8_t u8MemoryAddress, uint8_t* pu8Buffer, uint8_t u8Length)
{
    HAL_I2C_Mem_Read(&I2cHandle, 0xD0, u8MemoryAddress, 1, pu8Buffer, u8Length, HAL_MAX_DELAY);
}
