#ifndef _RTC_DS3231_H
#define _RTC_DS3231_H

typedef struct _RTC_DS3231_tsTime
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hour;
    uint8_t dayofweek;
    uint8_t dayofmonth;
    uint8_t month;
    uint8_t year;
} RTC_DS3231_tsTime;

void RTC_DS3231_vSetTime(RTC_DS3231_tsTime* sTime);
void RTC_DS3231_vGetTime(RTC_DS3231_tsTime* sTime);
int8_t RTC_DS3231_i8GetTemperature(void);

#endif // _RTC_DS3231_H
