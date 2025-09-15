/* ------------------------------------------------------------
 * "THE BEERWARE LICENSE" (Revision 42):
 * <cvmanjoo@gmail.com> wrote this code. As long as you retain this
 * notice, you can do whatever you want with this stuff. If we
 * meet someday, and you think this stuff is worth it, you can
 * buy me a beer in return.
 * ------------------------------------------------------------
 * ------------------------------------------------------------
 * MCP7940.h - MCP7940
 * Created by Manjunath CV. April 04 2019, 07:00 PM
 * Released into the public domain.
 * -----------------------------------------------------------*/

#include <Arduino.h>
#include <Wire.h>
#include <I2C_RTC.h>

#define	RTCSEC		0x00
#define	RTCMIN		0x01
#define	RTCHOUR		0x02
#define	RTCWKDAY	0x03
#define	RTCDATE		0x04
#define	RTCMTH		0x05
#define	RTCYEAR		0x06
#define	CONTROL		0x07
#define	OSCTRIM		0x08

#define	ALM0SEC		0x0A
#define	ALM0MIN		0x0B
#define	ALM0HOUR	0x0C
#define	ALM0WKDAY	0x0D
#define	ALM0DATE	0x0E
#define	ALM0MTH		0x0F

#define	ALM1SEC		0x11
#define	ALM1MIN		0x12
#define	ALM1HOUR	0x13
#define	ALM1WKDAY	0x14
#define	ALM1DATE	0x15
#define	ALM1MTH		0x16

#define	PWRDNMIN	0x18
#define	PWRDNHOUR	0x19
#define	PWRDNDATE	0x1A
#define	PWRDNMTH	0x1B

#define	PWRUPMIN	0x1C
#define	PWRUPHOUR	0x1D
#define	PWRUPDATE	0x1E
#define	PWRUPMTH	0x1F

#define	ST	    7
#define LYPR    5

#define OSCRUN  5
#define PWRFAIL 4
#define VBATEN  3

#define CLCK_12_24 6
#define MRDN_AM_PM 5

uint8_t MCP7940::begin()
{
    Wire.begin(); // join i2c bus
    return(MCP7940_ADDR);
}

bool MCP7940::isConnected()
{
    Wire.begin(); // join i2c bus
    Wire.beginTransmission(MCP7940_ADDR);
    return (Wire.endTransmission() == 0 ?  true : false);
}

bool MCP7940::isRunning(void)
{
   uint8_t reg_data;
   reg_data = _read_one_register(RTCSEC);
   return(bitRead(reg_data,ST));
}

void MCP7940::startClock()
{
    uint8_t reg_data;
    reg_data = _read_one_register(RTCSEC);
    bitSet(reg_data,ST);
    _write_one_register(RTCSEC,reg_data);
}

void MCP7940::stopClock()
{
    uint8_t reg_data;
    reg_data = _read_one_register(RTCSEC);
    bitClear(reg_data,ST);
    _write_one_register(RTCSEC,reg_data);
}

/*-----------------------------------------------------------
-----------------------------------------------------------*/

void MCP7940::setHourMode(uint8_t h_mode)
{
    uint8_t reg_data;
    reg_data = _read_one_register(RTCHOUR);
    bitWrite(reg_data, 6, h_mode);
    _write_one_register(RTCHOUR,reg_data);
}

uint8_t MCP7940::getHourMode()
{
    bool h_mode;
	uint8_t reg_data;
    reg_data = _read_one_register(RTCHOUR);
	h_mode = bitRead(reg_data, 6);
	return (h_mode);
}
/*-----------------------------------------------------------
-----------------------------------------------------------*/
void MCP7940::setMeridiem(uint8_t meridiem)
{
    uint8_t reg_data;
    reg_data = _read_one_register(RTCHOUR);
    bitSet(reg_data,5);
    _write_one_register(RTCHOUR,reg_data);
}

uint8_t MCP7940::getMeridiem()
{
    bool am_pm;
	uint8_t reg_data;
    if (getHourMode() == CLOCK_H12)
	{
        reg_data = _read_one_register(RTCHOUR);
        am_pm = bitRead(reg_data, 5);
	    return (am_pm);
    }
    else
        return (HOUR_24);
}

/*-----------------------------------------------------------
get & set Second
-----------------------------------------------------------*/

void MCP7940::setSeconds(uint8_t seconds)
{
    uint8_t reg_data, st_bit;
	if (seconds >= 00 && seconds <= 59)
	{
        reg_data = _read_one_register(RTCSEC);
        bitClear(reg_data,ST);
        st_bit = bitRead(reg_data, ST);
        seconds = bin2bcd(seconds);
        bitWrite(seconds,ST,st_bit);
        _write_one_register(RTCSEC,seconds);
	}
}

uint8_t MCP7940::getSeconds()
{
    uint8_t seconds;
    seconds = _read_one_register(RTCSEC);
    bitClear(seconds, ST);
    return(bcd2bin(seconds));
}

/*-----------------------------------------------------------
getMinute
-----------------------------------------------------------*/
uint8_t MCP7940::getMinutes()
{
	uint8_t minutes;
    minutes = _read_one_register(RTCMIN);
    return(bcd2bin(minutes));
}

void MCP7940::setMinutes(uint8_t minutes)
{
	if(minutes >= 00 && minutes <= 59)
        _write_one_register(RTCMIN,bin2bcd(minutes));
}

/*-----------------------------------------------------------
getHours
-----------------------------------------------------------*/
void  MCP7940::setHours(uint8_t hours)
{
	bool h_mode;
    uint8_t reg_hour;

    reg_hour = _read_one_register(RTCHOUR);
	h_mode = bitRead(reg_hour, 6);

	if (hours >= 00 && hours <= 23)
	{

		if (h_mode == CLOCK_H24)
		{
            _write_one_register(RTCHOUR,bin2bcd(hours));
		}
		else if (h_mode == CLOCK_H12)
		{
			if (hours > 12)
			{
				hours = hours % 12;
				hours = bin2bcd(hours);
				bitSet(hours, 6);
				bitSet(hours, 5);
				 _write_one_register(RTCHOUR,hours);
			}
			else
			{
				hours = bin2bcd(hours);
				bitSet(hours, 6);
				bitClear(hours, 5);
				 _write_one_register(RTCHOUR,hours);
			}
		}
	}
}


uint8_t MCP7940::getHours()
{
	bool h_mode; // = getHourMode();
    uint8_t reg_hour;
	
    reg_hour = _read_one_register(RTCHOUR);
	h_mode = bitRead(reg_hour, 6);

	if (h_mode == CLOCK_H24)
	{
		return (bcd2bin(reg_hour));
	}
	else if (h_mode == CLOCK_H12)
	{
		bitClear(reg_hour, 5);
		bitClear(reg_hour, 6);
		return (bcd2bin(reg_hour));
	}
	else
	{
		return 0;
	}
}

/*-----------------------------------------------------------
set and get day;
-----------------------------------------------------------*/

void MCP7940::setDay(uint8_t day)
{
    if(day >= 1 && day <= 31) 
    {
        _write_one_register(RTCDATE,bin2bcd(day));
    }
}

uint8_t MCP7940::getDay()
{
    uint8_t day;
    day = _read_one_register(RTCDATE);
    return(bcd2bin(day));
}

void MCP7940::setWeek(uint8_t week)
{
    uint8_t reg_week;
    if(week >= 1 && week <= 7)
    {
        reg_week = _read_one_register(RTCWKDAY);
        reg_week = reg_week & 0xF8; // Clear 7:3 Bits
        reg_week = reg_week | week; // Merge both data
        _write_one_register(RTCWKDAY,reg_week);
    }
}

uint8_t MCP7940::getWeek()
{
    uint8_t week;
    week = _read_one_register(RTCWKDAY);
    week = week & 0x07;
    return(bcd2bin(week));
}

/*-----------------------------------------------------------
set and get month;
-----------------------------------------------------------*/
void MCP7940::setMonth(uint8_t month)
{
    if(month >= 1 && month <= 12) 
    {
        _write_one_register(RTCMTH,bin2bcd(month));
    }
}

uint8_t MCP7940::getMonth()
{
   uint8_t reg_month;
   reg_month = _read_one_register(RTCMTH);
   bitClear(reg_month,LYPR);
   return(bcd2bin(reg_month));
}

/*-----------------------------------------------------------
set and get month;
-----------------------------------------------------------*/
void MCP7940::setYear(uint16_t year)
{
    if((year >= 0 && year <= 99) || (year >= 2000 && year <= 2099))
	{
        year = year % 100; //Converting to 2 Digit   
        _write_one_register(RTCYEAR,bin2bcd(year));
    }
}

uint16_t MCP7940::getYear()
{
    uint8_t reg_year;
    reg_year = _read_one_register(RTCYEAR);
    return(bcd2bin(reg_year)+2000);
}

/*-----------------------------------------------------------
    setDateTime(__TIMESTAMP__)
    Uses Built in variable to set time 

    //Timestamp format Fri Mar 08 13:01:53 2024
-----------------------------------------------------------*/

void MCP7940::setDateTime(String timestamp)
{
	uint8_t day, month, hour, minute, second, week;
	uint16_t year;
	String month_str, week_str;
	
	week_str = timestamp.substring(0,3);
	month_str = timestamp.substring(4,7);
	day = timestamp.substring(8,11).toInt();
	hour = timestamp.substring(11,13).toInt();
	minute = timestamp.substring(14,16).toInt();
	second = timestamp.substring(17,19).toInt();
	year = timestamp.substring(20,24).toInt();

	switch (week_str[0]) {
	case 'S': week = week_str[1] == 'u' ? 1 : 7; break;
	case 'M': week = 2; break;
	case 'T': week = week_str[1] == 'u' ? 3 : 5; break;
	case 'W': week = 4; break;
	case 'F': week = 6; break;
	}

	switch (month_str[0]) {
	case 'J': month = (month_str[1] == 'a') ? 1 : ((month_str[2] == 'n') ? 6 : 7); break;
	case 'F': month = 2; break;
	case 'A': month = month_str[2] == 'r' ? 4 : 8; break;
	case 'M': month = month_str[2] == 'r' ? 3 : 5; break;
	case 'S': month = 9; break;
	case 'O': month = 10; break;
	case 'N': month = 11; break;
	case 'D': month = 12; break;
	}

    // Wire.beginTransmission(_i2c_address);
    // Wire.write(0x00);
    // Wire.write(0);
    // Wire.endTransmission();

	setWeek(week);
	
	setDay(day);
	setMonth(month);
	setYear(year);

	setHours(hour);
	setMinutes(minute);
	setSeconds(second);
}

bool MCP7940::doesPowerFailed()
{
    uint8_t reg_week;
    bool pwr_flag;
    reg_week = _read_one_register(RTCWKDAY);
    pwr_flag = bitRead(reg_week,PWRFAIL);
    return(pwr_flag); 
}

void MCP7940::clearPowerFail()
{
    uint8_t reg_week;
    reg_week = _read_one_register(RTCWKDAY);
    bitClear(reg_week,PWRFAIL);
    _write_one_register(RTCWKDAY,reg_week);
}

bool MCP7940::isExternalBatteryEnabled()
{
    uint8_t reg_week;
    bool bat_flag;
    reg_week = _read_one_register(RTCWKDAY);
    bat_flag = bitRead(reg_week,VBATEN);
    return(bat_flag); 
}

void MCP7940::enableExternalBattery()
{
    uint8_t reg_week;
    reg_week = _read_one_register(RTCWKDAY);
    bitSet(reg_week,VBATEN);
    _write_one_register(RTCWKDAY,reg_week);
}

void MCP7940::disableExternalBattery()
{
    uint8_t reg_week;
    reg_week = _read_one_register(RTCWKDAY);
    bitClear(reg_week,VBATEN);
    _write_one_register(RTCWKDAY,reg_week);
}

tm MCP7940::getDateTime()
{
    struct tm rtc_time;
    rtc_time.tm_sec = getSeconds();
    rtc_time.tm_min = getMinutes();
    rtc_time.tm_hour = getHours();
    rtc_time.tm_wday = getWeek() - 1;
    rtc_time.tm_mday = getDay();
    rtc_time.tm_mon = getMonth() - 1;
    rtc_time.tm_year = getYear() - 1900;

    return rtc_time;
}

tm MCP7940::getPowerDownDateTime()
{
    struct tm power_down_time;
    uint8_t reg_data;
	
    Wire.beginTransmission(_i2c_address);
	Wire.write(PWRDNMIN);
	Wire.endTransmission();
	Wire.requestFrom((int)_i2c_address, 4);

    for(int i = 0;i< 4; i++)
    {
        switch (i) 
        {
            case 0: reg_data = Wire.read();
                    power_down_time.tm_min = bcd2bin(reg_data);   
                    break;
            case 1: reg_data = Wire.read();
                    if(bitRead(reg_data,CLCK_12_24) == 1) //12-Hour
                    {
                        reg_data = reg_data & 0x9F; //Clear 6:5 Bits
                        power_down_time.tm_hour = bcd2bin(reg_data);
                    }
                    else //24 Hours
                    {
                        power_down_time.tm_hour = bcd2bin(reg_data);
                    }
                    break;
            case 2: reg_data = Wire.read();
                    power_down_time.tm_mday = bcd2bin(reg_data);
                    break;
            case 4: reg_data = Wire.read();
                    power_down_time.tm_wday = (reg_data >> 5) - 1;


                    reg_data = reg_data & 0x1F; //Clear 7:5 Bits
                    
                    power_down_time.tm_mon = bcd2bin(reg_data);
                    break;
        }
    }
    return power_down_time;
}

tm MCP7940::getPowerUpDateTime()
{
    struct tm power_up_time;
    uint8_t reg_data;
	
    Wire.beginTransmission(_i2c_address);
	Wire.write(PWRUPMIN);
	Wire.endTransmission();
	Wire.requestFrom((int)_i2c_address, 4);

    for(int i = 0;i< 4; i++)
    {
        switch (i)
        {
            case 0: reg_data = Wire.read();
                    power_up_time.tm_min = bcd2bin(reg_data);   
                    break;
            case 1: reg_data = Wire.read();
                    if(bitRead(reg_data,CLCK_12_24) == 1) //12-Hour
                    {
                        reg_data = reg_data & 0x9F; //Clear 6:5 Bits
                        power_up_time.tm_hour = bcd2bin(reg_data);
                    }
                    else //24 Hours
                    {
                        power_up_time.tm_hour = bcd2bin(reg_data);
                    }
                    break;
            case 2: reg_data = Wire.read();
                    power_up_time.tm_mday = bcd2bin(reg_data);
                    break;
            case 4: reg_data = Wire.read();
                    power_up_time.tm_wday = (reg_data >> 5) - 1;
                    reg_data = reg_data & 0x1F; //Clear 7:5 Bits
                    power_up_time.tm_mon = getMonth() - 1;
                    break;
        }
    }
    return power_up_time;
}

/**************************************************************
 * 
 * Test Functions
 *  
**************************************************************/



uint8_t MCP7940::showRegister(uint8_t reg_address)
{
    return(_read_one_register(reg_address));
}

/**************************************************************
 * 
 * Private Functions
 *  
**************************************************************/


uint8_t MCP7940::_read_one_register(uint8_t reg_address)
{
    uint8_t reg_data;
    
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.endTransmission();

    Wire.requestFrom((int)_i2c_address,(int) 1);
    reg_data = Wire.read();
    return(reg_data);

}

void MCP7940::_write_one_register(uint8_t reg_address, uint8_t reg_data)
{
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.write(reg_data);
    Wire.endTransmission();
}

/* Helpers */

uint8_t MCP7940::bcd2bin(uint8_t val)
{
	return val - 6 * (val >> 4);
}

uint8_t MCP7940::bin2bcd(uint8_t val)
{
	return val + 6 * (val / 10);
}

//  -- END OF FILE --