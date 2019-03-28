#ifndef PCF8563_H
#define PCF8563_H

#include <time.h>
#include <Arduino.h>
#include <Wire.h>

#define PCF8563_ADDR 0x51

#define SQW32kHz 00
#define SQW10kHz 01
#define SQW032Hz 10
#define SQW001Hz 11

class PCF8563
{
    public:
        bool begin();

        bool isRunning(void);
        void startClock();
        void stopClock();

        void setSeconds(uint8_t second);
        void setMinutes(uint8_t minute);
        void setHours(uint8_t hour);
        void setDay(uint8_t day);
        void setWeek(uint8_t week);
        void setMonth(uint8_t month);
        void setYear(uint16_t year);

        void setDate(uint8_t day, uint8_t month, uint16_t year);
        void setTime(uint8_t hour, uint8_t minute, uint8_t second);


        void setDateTime(char* date, char* time);

        uint8_t getSeconds();
        uint8_t getMinutes();
        uint8_t getHours();
        uint8_t getDay();
        uint8_t getWeek();
        uint8_t getMonth();
        uint16_t getYear();


        void setEpoch(time_t epoch);
        time_t getEpoch();
        
        //Alarm Functions

        //void enableAlarm();
        void disableAlarm();
        void clearAlarm();

        bool isAlarmEnabled();
        bool isAlarmTriggered();

        void setAlarmHours(uint8_t hours);
        void setAlarmMinutes(uint8_t minutes);
        void setAlarmDay(uint8_t day);
        void setAlarmWeekDay(uint8_t weekday);
        //void setAlarmTime();

        uint8_t getAlarmHours();
        uint8_t getAlarmMinutes();
        uint8_t getAlarmDay();
        uint8_t getAlarmWeekDay();

        //void setOutPin(uint8_t mode);
        //bool isOutPinEnabled();
        //bool isSqweEnabled();

        //Timer Functions
        bool isTimerEnabled();
        void enableTimer();
        void disableTimer();
        void setTimer(uint8_t t_seconds);
        uint8_t getTimer();


    private:
        uint8_t bin2bcd (uint8_t val);
        uint8_t bcd2bin (uint8_t val);
};



//Instantiate class PCF8563 as RTC

static PCF8563 RTC;

#endif /* PCF8563_H */
