/**************************************************************************
    Library to set & get time from I2C RTCs for Arduino

    Copyright 2024 Manjunath <cvmanjoo@gmail.com>
    Website     : http://github.com/cvmanjoo/rtc
    Author      : Manjunath CV
    Create Time : July 08, 2017, 02:18 AM

    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or
    distribute this software, either in source code form or as a compiled
    binary, for any purpose, commercial or non-commercial, and by any
    means.

    In jurisdictions that recognize copyright laws, the author or authors
    of this software dedicate any and all copyright interest in the
    software to the public domain. We make this dedication for the benefit
    of the public at large and to the detriment of our heirs and
    successors. We intend this dedication to be an overt act of
    relinquishment in perpetuity of all present and future rights to this
    software under copyright law.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

    For more information, please refer to <http://unlicense.org>
    
 *************************************************************************/

#ifndef RTC_H

#ifndef UNIX_OFFSET
#define UNIX_OFFSET 946684800
#endif

//define all the rtc clocks.
//#define DS1307_H
//#define DS3231_H
//#define PCF8563_H

#define INVALID -1

#define CLOCK_H24 0
#define CLOCK_H12 1

#define HOUR_AM 0
#define HOUR_PM 1
#define HOUR_24 2

#define SQW001Hz 10
#define SQW01kHz 11
#define SQW04kHz 14
#define SQW08kHz 18
#define SQW32kHz 32

//const char* week_days[7]
//    = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };

// enum _WEEK_DAYS_

#include <time.h>
#include <Arduino.h>
#include <Wire.h>

#define DS1307_ADDR 0x68

class DS1307
{
    public:
        //DS1307();

        uint8_t begin();

        bool isConnected();
        bool isRunning();
        
        void startClock();
        void stopClock();

        void setHourMode(uint8_t h_mode);
        uint8_t getHourMode();

        void setMeridiem(uint8_t meridiem);
        uint8_t getMeridiem();

        void setSeconds(uint8_t seconds);
        uint8_t getSeconds();
        
        void setMinutes(uint8_t minutes);
        uint8_t getMinutes();       

        void setHours(uint8_t hours);
        uint8_t getHours();       

        void setDay(uint8_t day);
        uint8_t getDay();

        uint8_t getWeek();
        void setWeek(uint8_t week);
        void updateWeek();

        void setMonth(uint8_t month);
        uint8_t getMonth();       

        uint16_t getYear();        
        void setYear(uint16_t year);
		
        void setTime(uint8_t hour, uint8_t minute, uint8_t second);
        void setDate(uint8_t day, uint8_t month, uint16_t year);
        
        void setDateTime(String date, String time);
        void setDateTime(String timestamp);

        tm getDateTime();
        String getDateTimeString();
        String getTimeString();
        String getDateString();
       

        void setEpoch(time_t epoch);
        time_t getEpoch();

        void setOutPin(uint8_t mode);
        bool isOutPinEnabled();
        bool isSqweEnabled();

	private:
		uint8_t _i2c_address = DS1307_ADDR;

        uint8_t calculateDayOfWeek(uint8_t d, uint8_t m, uint16_t y);

        uint8_t _read_one_register(uint8_t reg_address);
        void _write_one_register(uint8_t reg_address, uint8_t reg_data);

		uint8_t bin2bcd(uint8_t val);
		uint8_t bcd2bin(uint8_t val);
};

class NVRAM
{
	private:
		uint8_t _length = 56;
    public:
        bool begin();
        
		uint8_t read(uint8_t address);
        void write(uint8_t address, uint8_t value);
		
		uint8_t operator[](uint8_t address) const;
		//uint8_t& operator[](uint8_t address); 
        
		void read(uint8_t address, uint8_t *buf, uint8_t size);
        void write(uint8_t address, uint8_t *buf, uint8_t size);
        
		uint8_t length();
};


/*

*/

#define DS3231_ADDR 0x68

class DS3231 {

    public:
        uint8_t begin();
        
        bool isConnected();
        bool isRunning();
        
        void startClock();
        void stopClock();

        void setHourMode(uint8_t h_mode);
        uint8_t getHourMode();

        void setMeridiem(uint8_t meridiem);
        uint8_t getMeridiem();

        void setSeconds(uint8_t second);
        uint8_t getSeconds();

        void setMinutes(uint8_t minute);
        uint8_t getMinutes();

        void setHours(uint8_t hour);
        uint8_t getHours();

        void setDay(uint8_t day);
        uint8_t getDay();

        void setMonth(uint8_t month);
        uint8_t getMonth();

        void setYear(uint16_t year);
        uint16_t getYear();

        void setWeek(uint8_t week);
        uint8_t getWeek();
        void updateWeek();

        void setDate(uint8_t day, uint8_t month, uint16_t year);
        void setTime(uint8_t hour, uint8_t minute, uint8_t second);
        void setDateTime(char* date, char* time);
        void setDateTime(String timestamp);


        void setEpoch(time_t epoch, bool is_unix_epoch=true);
        time_t getEpoch(bool as_unix_epoch=true);

        void enableAlarmPin();

        void enableAlarm1();
        void enableAlarm2();

        void disableAlarm1();
        void disableAlarm2();

        bool isAlarm1Enabled();
        bool isAlarm2Enabled();

        void setAlarm1();
        void setAlarm1(uint8_t second);
        void setAlarm1(uint8_t minute, uint8_t second);
        void setAlarm1(uint8_t hour, uint8_t minute, uint8_t second);
        void setAlarm1(uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

        //DateTime getAlarm1();

        void setAlarm2();
        void setAlarm2(uint8_t minute);
        void setAlarm2(uint8_t hour, uint8_t minute);
        void setAlarm2(uint8_t day, uint8_t hour, uint8_t minute);

        void setAlarm2Minutes(uint8_t minute);
        void setAlarm2Hours(uint8_t hour);
        void setAlarm2Week(uint8_t week);
        void setAlarm2Day(uint8_t day);

        uint8_t getAlarm2Minutes();
        uint8_t getAlarm2Hours();
        uint8_t getAlarm2Week();
        uint8_t getAlarm2Day();

        //DateTime getAlarm2();

        bool isAlarm1Tiggered();
        bool isAlarm2Tiggered();

        void clearAlarm1();
        void clearAlarm2();

        void setOutPin(uint8_t mode);
        bool getINTPinMode();
        void enableSqwePin();

        int8_t getAgingOffset();
        void setAgingOffset(int8_t);

        float getTemp();

    private:
        uint8_t _i2c_address = DS3231_ADDR;

        uint8_t bin2bcd(uint8_t val);
        uint8_t bcd2bin(uint8_t val);
        
        uint8_t _read_one_register(uint8_t reg_address);
        void _write_one_register(uint8_t reg_address, uint8_t reg_data);
};

/*

*/


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

        uint8_t getHourMode();
        uint8_t getMeridiem();

        void setSeconds(uint8_t second);
        uint8_t getSeconds();
       
        void setMinutes(uint8_t minute);
        uint8_t getMinutes();

        void setHours(uint8_t hour);
        uint8_t getHours();      

        void setDay(uint8_t day);
        uint8_t getDay();

        void setWeek(uint8_t week);
        uint8_t getWeek();
        void updateWeek();

        void setMonth(uint8_t month);
        uint8_t getMonth();

        void setYear(uint16_t year);
        uint16_t getYear();

        void setDate(uint8_t day, uint8_t month, uint16_t year);
        void setTime(uint8_t hour, uint8_t minute, uint8_t second);

        void setDateTime(char* date, char* time);
        void setDateTime(String timestamp);

        void setEpoch(time_t epoch);
        time_t getEpoch();

        //Alarm Functions

        //void enableAlarm();
        void disableAlarm();
        void clearAlarm();

        bool isAlarmEnabled();
        bool isAlarmTriggered();

        void setAlarmMinutes(uint8_t minutes);        
        void setAlarmHours(uint8_t hours);
        void setAlarmDay(uint8_t day);
        void setAlarmWeek(uint8_t week);

        uint8_t getAlarmMinutes();        
        uint8_t getAlarmHours();
        uint8_t getAlarmDay();
        uint8_t getAlarmWeek();

        void enableAlarmInterrupt(void);
        void disableAlarmInterrupt(void);



        //void setOutPin(uint8_t mode);
        //bool isOutPinEnabled();
        //bool isSqweEnabled();

        //Timer Functions
        bool isTimerEnabled();
        void enableTimer();
        void disableTimer();
        void setTimer(uint8_t t_seconds);
        uint8_t getTimer();

        void enableTimerInterrupt(void);
        void disableTimerInterrupt(void);


        bool getTimerFlag();
        void clearTimerFlag();

    private:
		uint8_t _i2c_address = PCF8563_ADDR;

		uint8_t bin2bcd(uint8_t val);
		uint8_t bcd2bin(uint8_t val);
		
		uint8_t _read_one_register(uint8_t reg_address);
        void _write_one_register(uint8_t reg_address, uint8_t reg_data);
};



#define PCF8523_ADDR 0x68

class PCF8523
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
        uint8_t getSeconds();

        void setMinutes(uint8_t minute);
        uint8_t getMinutes();

        void setHours(uint8_t hour);
        uint8_t getHours();

        void setDay(uint8_t day);
        uint8_t getDay();

        void setWeek(uint8_t week);
        uint8_t getWeek();
        void updateWeek();
        
        void setMonth(uint8_t month);
        uint8_t getMonth();

        void setYear(uint16_t year);
		uint16_t getYear();

        void setDate(uint8_t day, uint8_t month, uint16_t year);
        void setTime(uint8_t hour, uint8_t minute, uint8_t second);
        
        void setDateTime(char* date, char* time);
        void setDateTime(String timestamp);

        void setEpoch(time_t epoch);
        time_t getEpoch();

        //Alarm Functions

        //void enableAlarm();
        void disableAlarm();
        void clearAlarm();

        bool isAlarmEnabled();
        bool isAlarmTriggered();
        
        void setAlarm(uint8_t hours, uint8_t minutes);
        void setAlarm(uint8_t week,uint8_t day, uint8_t hours, uint8_t minutes);
        //DateTime getAlarm();

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
		uint8_t _i2c_address = PCF8523_ADDR;

		uint8_t bin2bcd(uint8_t val);
		uint8_t bcd2bin(uint8_t val);
		
		uint8_t _read_one_register(uint8_t reg_address);
        void _write_one_register(uint8_t reg_address, uint8_t reg_data);
};

#define MCP7940_ADDR 0x6F

class MCP7940
{
	public:

		bool isConnected();

		bool isRunning(void);
		void startClock();
		void stopClock();
		
		void setHourMode(uint8_t h_mode);
		uint8_t getHourMode();

		void setMeridiem(uint8_t meridiem);
		uint8_t getMeridiem();

		void setSeconds(uint8_t second);
		uint8_t getSeconds();

		void setMinutes(uint8_t minute);
		uint8_t getMinutes();

		void setHours(uint8_t hour);
		uint8_t getHours();

		void setDay(uint8_t day);
		uint8_t getDay();

		void setWeek(uint8_t week);
		uint8_t getWeek();
		// void updateWeek();
		
		void setMonth(uint8_t month);
		uint8_t getMonth();

		void setYear(uint16_t year);
		uint16_t getYear();

        tm getDateTime();
        tm getPowerDownDateTime();
        tm getPowerUpDateTime();

		// void setDate(uint8_t day, uint8_t month, uint16_t year);
		// void setTime(uint8_t hour, uint8_t minute, uint8_t second);
		
		// void setDateTime(char* date, char* time);
		void setDateTime(String timestamp);

		// void setEpoch(time_t epoch);
		// time_t getEpoch();

        bool doesPowerFailed();
        void clearPowerFail();
        
        bool isExternalBatteryEnabled();
        void enableExternalBattery();
        void disableExternalBattery();



        uint8_t showRegister(uint8_t reg_address);


	private:
		uint8_t _i2c_address = MCP7940_ADDR;

		uint8_t bin2bcd(uint8_t val);
		uint8_t bcd2bin(uint8_t val);
		
		uint8_t _read_one_register(uint8_t reg_address);
        void _write_one_register(uint8_t reg_address, uint8_t reg_data);
};




/* Instantiate class

static DS3231 RTC;
static DS1307 RTC;
static NVRAM NVRAM;
static PCF8563 RTC;

*/


#endif   /* RTC_H */
