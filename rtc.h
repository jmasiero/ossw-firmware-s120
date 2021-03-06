#ifndef RTC_H
#define RTC_H

#include <stdbool.h>
#include <stdint.h>
#include "app_timer.h"

#define RTC_INTERVAL       APP_TIMER_TICKS(1000, 0)

void rtc_timer_init(void);

uint32_t rtc_get_current_time(void);
void rtc_set_current_time(uint32_t);
bool rtc_should_store_current_time(void);
void rtc_store_current_time(void);

uint32_t rtc_get_current_hour(void);
uint32_t rtc_get_current_minutes(void);
uint32_t rtc_get_current_seconds(void);
uint32_t rtc_get_current_day_of_month(void);
uint32_t rtc_get_current_month(void);
uint32_t rtc_get_current_year(void);

#endif /* RTC_H */
