/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define BLINK_PORT_NUMBER (3)
#define BLINK_PIN_NUMBER  (12)
#define BLINK_ACTIVE_LOW  (0)

#define BLINK_GPIOx(_N)     ((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))
#define BLINK_PIN_MASK(_N)  (1 << (_N))
#define BLINK_RCC_MASKx(_N) (RCC_AHB1ENR_GPIOAEN << (_N))

#define GPIO_PORT_NUMBER (3)

typedef enum {
    GPIO_eInput = GPIO_MODE_INPUT,
    GPIO_eOutputPP = GPIO_MODE_OUTPUT_PP,
    GPIO_eOutputOD = GPIO_MODE_OUTPUT_OD
} GPIO_teGpioType;

inline void GPIO_on(uint16_t pinBitField);
inline void GPIO_off(uint16_t pinBitField);
uint8_t GPIO_read(uint8_t port, uint8_t pin);

void GPIO_vInit(uint8_t u8Port, uint8_t u8Pin, GPIO_teGpioType eGpioType);

/* Providing two functions should prioritise code speed over code space */
inline void __attribute__((always_inline)) GPIO_on(uint16_t pinBitField) {
	if (0 != pinBitField) {
		HAL_GPIO_WritePin(BLINK_GPIOx(GPIO_PORT_NUMBER), pinBitField, GPIO_PIN_SET);
	}
}
inline void __attribute__((always_inline)) GPIO_off(uint16_t pinBitField) {
    if (0 != pinBitField) {
        HAL_GPIO_WritePin(BLINK_GPIOx(GPIO_PORT_NUMBER), pinBitField, GPIO_PIN_RESET);
    }
}

// ----------------------------------------------------------------------------

#endif // BLINKLED_H_
