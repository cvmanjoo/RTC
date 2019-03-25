#ifndef DS1307_H
#define DS1307_H

#include <time.h>
#include <Arduino.h>
#include <Wire.h>

#define DS1307_ADDR 0x68

#define CLOCK_H24 0
#define CLOCK_H12 1

#define HOUR_AM 0
#define HOUR_PM 1
#define HOUR_24 2

#define SQW001Hz 10
#define SQW04kHz 14
#define SQW08kHz 18
#define SQW32kHz 32

class DS1307
{
    public:
        bool begin();

        bool isRunning(void);
        void startClock();
        void stopClock();

        void setHourMode(uint8_t h_mode);
        uint8_t getHourMode();

        void setMeridiem(uint8_t meridiem);
        uint8_t getMeridiem();

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

        void setOutPin(uint8_t mode);
        bool isOutPinEnabled();
        bool isSqweEnabled();

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
