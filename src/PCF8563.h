#ifndef PCF8563_H
#define PCF8563_H

#include <time.h>
#include <Arduino.h>
#include <Wire.h>

#define PCF8563_ADDR 0x51

#define SQW001Hz 10
#define SQW04kHz 14
#define SQW08kHz 18
#define SQW32kHz 32

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
        uint8_t getWeek();
        uint8_t getMonth();
        uint16_t getYear();
        uint8_t getDay();

        //void setEpoch(time_t epoch, time_t e_year, int16_t offset);
        //time_t getEpoch();
        //void outPin(uint8_t mode);

        //void enableAlarm();
        void disableAlarm();
        void clearAlarm();

        //bool isAlarmEnabled();
        bool isAlarmTriggered();

        void setAlarmHours(uint8_t hours);
        void setAlarmMinutes(uint8_t minutes);
        //setAlarmTime();
        //setAlarmDay();

        uint8_t getAlarmHours();
        uint8_t getAlarmMinutes();
        //getAlarmTime();
        //getAlarmDay();




    private:
        uint8_t bin2bcd (uint8_t val);
        uint8_t bcd2bin (uint8_t val);
};



//Instantiate class PCF8563 as RTC

static PCF8563 RTC;

#endif /* PCF8563_H */
