#ifndef DS1307_H
#define DS1307_H

#include <time.h>
#include <Arduino.h>
#include <Wire.h>

#define DS1307_ADDR 0x68

#define TIME_H12 0
#define TIME_H24 1

#define SQW001Hz 10
#define SQW04kHz 14
#define SQW08kHz 18
#define SQW32kHz 32

class DS1307
{
    public:
        bool begin();

        void setSecond(uint8_t second);
        void setMinute(uint8_t minute);
        void setHour(uint8_t hour);
        void setDay(uint8_t day);
        void setMonth(uint8_t month);
        void setYear(uint16_t year);

        void setWeek(uint8_t week);

        void setEpoch(time_t epoch, time_t e_year, int16_t offset);

        void setDate(uint8_t day, uint8_t month, uint16_t year);
        void setTime(uint8_t hour, uint8_t minute, uint8_t second);


        void setDateTime(char* date, char* time);


        uint16_t getYear();
        uint8_t getMonth();
        uint8_t getDay();
        uint8_t getSecond();
        uint8_t getMinute();
        uint8_t getHour();

        uint8_t getWeek();

        time_t getEpoch();

        bool isRunning(void);
        void startClock();
        void stopClock();

        void outPin(uint8_t mode);

    private:
        uint8_t bin2bcd (uint8_t val);
        uint8_t bcd2bin (uint8_t val);
};

class NVRAM
{
    public:
        bool begin();
        uint8_t read(uint8_t address);
        void write(uint8_t address, uint8_t value);
        uint8_t length = 56;
    private:
};


//Instantiate class DS1307 as RTC

static DS1307 RTC;
static NVRAM NVRAM;

#endif /* DS1307_H */
