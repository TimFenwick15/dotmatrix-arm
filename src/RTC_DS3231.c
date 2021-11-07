/*
 * RTC_DS3231.c
 *
 *  Created on: 7 Nov 2021
 *      Author: Tim
 */

#include "main.h"
#include "i2c.h"
#include "RTC_DS3231.h"

#define DS3231_TIME_MEMORY_ADDRESS (0)

static uint8_t decToBcd(int val);
static int bcdToDec(uint8_t val);

void RTC_DS3231_vSetTime(RTC_DS3231_tsTime* sTime)
{
    uint8_t pu8Buffer[8];
    pu8Buffer[0] = decToBcd(sTime->seconds);
    pu8Buffer[1] = decToBcd(sTime->minutes);
    pu8Buffer[2] = decToBcd(sTime->hour);
    pu8Buffer[3] = decToBcd(sTime->dayofweek);
    pu8Buffer[4] = decToBcd(sTime->dayofmonth);
    pu8Buffer[5] = decToBcd(sTime->month);
    pu8Buffer[6] = decToBcd(sTime->year);
    I2C_vWriteMemory(DS3231_TIME_MEMORY_ADDRESS, pu8Buffer, 7);
}

void RTC_DS3231_vGetTime(RTC_DS3231_tsTime* sTime)
{
    uint8_t pu8Buffer[8];
    I2C_vReadMemory(DS3231_TIME_MEMORY_ADDRESS, pu8Buffer, 8);
    sTime->seconds    = bcdToDec(pu8Buffer[0]);
    sTime->minutes    = bcdToDec(pu8Buffer[1]);
    sTime->hour       = bcdToDec(pu8Buffer[2]);
    sTime->dayofweek  = bcdToDec(pu8Buffer[3]);
    sTime->dayofmonth = bcdToDec(pu8Buffer[4]);
    sTime->month      = bcdToDec(pu8Buffer[5]);
    sTime->year       = bcdToDec(pu8Buffer[6]);
}

// Convert normal decimal numbers to binary coded decimal
static uint8_t decToBcd(int val)
{
  return (uint8_t)( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
static int bcdToDec(uint8_t val)
{
  return (int)( (val/16*10) + (val%16) );
}
