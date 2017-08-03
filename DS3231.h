/*
 * DS3231.h - Library to set & get time from RTC DS3231
 * Created by Manjunath CV. July 08, 2017, 02:18 AM
 * Released into the public domain.
 */

#ifndef DS3231_H
#define DS3231_H

#include <Arduino.h>
#include <Wire.h>

#define DS3231_ADDR 0x68

#define TIME_H12 0 
#define TIME_H24 1

#define Y1900 1900 
#define Y1970 1970
#define Y2000 2000

class DS3231 {

    public:
      void begin();

      void setYear(uint16_t year);
      void setMonth(uint8_t);
      void setDay(uint8_t);
      void setSecond(uint8_t data);
      void setMinute(uint8_t);
      void setHour(uint8_t);

      void setDate(uint8_t,uint8_t,uint16_t);
      void setTime(uint8_t,uint8_t,uint8_t);

      uint16_t getYear();
      uint8_t getMonth();
      uint8_t getDay();
      uint8_t getSecond();
      uint8_t getMinute();
      uint8_t getHour();

      uint8_t getWeek();

      uint8_t enableAlaram();

      bool lostPower();
      void StartClock();
      float getTemp();

    private:
      static uint8_t bin2bcd (uint8_t val);
      static uint8_t bcd2bin (uint8_t val);

};


#endif /* DS3231_H */
