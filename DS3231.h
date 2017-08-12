/*
 * DS3231.h - Library to set & get time from RTC DS3231
 * Created by Manjunath CV. July 08, 2017, 02:18 AM
 * Released into the public domain.
 */

#ifndef DS3231_H
#define DS3231_H

#include <Time.h>
#include <Arduino.h>
#include <Wire.h>

#define DS3231_ADDR 0x68

#define TIME_H12 0 
#define TIME_H24 1

class DS3231 {

    public:
      bool begin();

      void setSecond(uint8_t second);
      void setMinute(uint8_t minute);
      void setHour(uint8_t hour);
      void setDay(uint8_t day);
      void setMonth(uint8_t month);
      void setYear(uint16_t year);

      void setWeek(uint8_t week);

      void setDate(uint8_t day, uint8_t month, uint16_t year);
      void setTime(uint8_t hour, uint8_t minute, uint8_t second);

      void setEpoch(time_t epoch, time_t e_year, int16_t offset);

      void setDateTime(char* date, char* time);

      uint16_t getYear();
      uint8_t getMonth();
      uint8_t getDay();
      uint8_t getSecond();
      uint8_t getMinute();
      uint8_t getHour();

      uint8_t getWeek();

      time_t getEpoch();

      void enableAlarmPin();
      void enableAlarm1();
      void enableAlarm2();

      void setAlarm1();
      void setAlarm1(uint8_t second);
      void setAlarm1(uint8_t second, uint8_t minute);
      void setAlarm1(uint8_t second, uint8_t minute, uint8_t hour);
      void setAlarm1(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day);

      void setAlarm2();
      void setAlarm2(uint8_t minute);
      void setAlarm2(uint8_t minute, uint8_t hour);
      void setAlarm2(uint8_t minute, uint8_t hour, uint8_t day);

      bool isAlarm1Tiggered();
      bool isAlarm2Tiggered();

      bool lostPower();
      void StartClock();
      float getTemp();

      //uint8_t getRegister(uint8_t reg);
      //void setRegister(uint8_t reg, uint8_t data);

    private:
      uint8_t bin2bcd (uint8_t val);
      uint8_t bcd2bin (uint8_t val);

};

//Instantiate class DS3231 as RTC

static DS3231 RTC;

#endif /* DS3231_H */
