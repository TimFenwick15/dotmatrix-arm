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

#include "main.h"
#include "GPIO.h"

/**
 * Prepare a pin for use - should be called on each pin required
 * @param u8Port - 0=A, 1=B etc, each port holds 16 pins (0 - 15)
 * @param u8Pin - the pin number from this port, 0 - 15
 * @param u8UseOpenDrain - one pin for my application wants to be open drain,
 *                         the rest want to be "push pull" mode
 */
void GPIO_vInit(uint8_t u8Port, uint8_t u8Pin, GPIO_teGpioType eGpioType) {
	/* Enable GPIO Peripheral clock */
	RCC->AHB1ENR |= BLINK_RCC_MASKx(u8Port);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = BLINK_PIN_MASK(u8Pin);
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = (uint32_t)eGpioType;
	if (GPIO_eOutputOD == eGpioType) {
		GPIO_InitStructure.Pull = GPIO_NOPULL;
	} else {
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	}
	HAL_GPIO_Init(BLINK_GPIOx(u8Port), &GPIO_InitStructure);
}

uint8_t GPIO_read(uint8_t port, uint8_t pin) {
    return HAL_GPIO_ReadPin(BLINK_GPIOx(port), 1 << pin);// ? true : false;
}

