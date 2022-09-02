/*
* PCF8523.cpp - Library to set & get time from RTC PCF8523
* Created by Manjunath CV. August 15, 2022, 08:07 PM
* Released into the public domain.
*/

#include <time.h>
#include <Arduino.h>
#include <Wire.h>
#include <I2C_RTC.h>

#define R_CONTROL_1	0x00
#define R_CONTROL_2	0x01
#define R_CONTROL_3	0x02
#define R_SECONDS	0x03
#define R_MINUTES	0x04
#define R_HOURS		0x05
#define R_DAYS		0x06
#define R_WEEKDAYS	0x07
#define R_MONTHS	0x08
#define R_YEARS		0x09


bool PCF8523::begin()
{
    Wire.begin(); // join i2c bus
    Wire.beginTransmission (PCF8523_ADDR);
    return (Wire.endTransmission() == 0 ?  true : false);
}


bool PCF8523::isRunning(void)
{
    uint8_t data;
    bool flag;

    Wire.beginTransmission(PCF8523_ADDR);
    Wire.write(R_CONTROL_1);
    Wire.endTransmission();

    Wire.requestFrom(PCF8523_ADDR, 1);
    data = Wire.read();

   	flag = bitRead(data,5);

    return (!flag);
}

void PCF8523::startClock(void)
{
    uint8_t data;

    Wire.beginTransmission(PCF8523_ADDR);
    Wire.write(R_CONTROL_1);
    Wire.endTransmission();

    Wire.requestFrom(PCF8523_ADDR, 1);
    data = Wire.read();

    bitClear(data, 5);

    Wire.beginTransmission(PCF8523_ADDR);
    Wire.write(R_CONTROL_1);  // Seconds Register
    Wire.write(data);
    Wire.endTransmission();
}

void PCF8523::stopClock(void)
{
	uint8_t data;

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_CONTROL_1);
	Wire.endTransmission();

	Wire.requestFrom(PCF8523_ADDR, 1);
	data = Wire.read();

	bitSet(data, 5);

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_CONTROL_1);  // Seconds Register
	Wire.write(data);
	Wire.endTransmission();
}

void PCF8523::setHourMode(uint8_t h_mode)
{
	uint8_t data;

	if(h_mode == CLOCK_H12 || h_mode == CLOCK_H24)
	{
		Wire.beginTransmission(PCF8523_ADDR);
		Wire.write(R_CONTROL_1);
		Wire.endTransmission();

		Wire.requestFrom(PCF8523_ADDR, 1);
		data = Wire.read();

		bitWrite(data, 3, h_mode);

		Wire.beginTransmission(PCF8523_ADDR);
		Wire.write(R_CONTROL_1);  // Hour Register
		Wire.write(data);
		Wire.endTransmission();
	}
}

uint8_t PCF8523::getHourMode()
{
	bool h_mode;
	uint8_t data;

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_CONTROL_1);
	Wire.endTransmission();

	Wire.requestFrom(PCF8523_ADDR, 1);
	data = Wire.read();

	h_mode = bitRead(data, 3);

	return (h_mode);
}

void PCF8523::setMeridiem(uint8_t meridiem)
{
	uint8_t data;
	if(meridiem == HOUR_AM || meridiem == HOUR_PM)
	{
		if (getHourMode() == CLOCK_H12)
		{
			Wire.beginTransmission(PCF8523_ADDR);
			Wire.write(R_HOURS);  // Hour Register
			Wire.endTransmission();
			Wire.requestFrom(PCF8523_ADDR, 1);
			data = Wire.read();
			bitWrite(data, 5, meridiem);
			Wire.beginTransmission(PCF8523_ADDR);
			Wire.write(R_HOURS);  // Hour Register
			Wire.write(data);
			Wire.endTransmission();
		}
	}
}

uint8_t PCF8523::getMeridiem()
{
	bool flag;
	uint8_t data;
	if (getHourMode() == CLOCK_H12)
	{
		Wire.beginTransmission(PCF8523_ADDR);
		Wire.write(R_HOURS);
		Wire.endTransmission();

		Wire.requestFrom(PCF8523_ADDR, 1);
		data = Wire.read();

		flag = bitRead(data, 5);
		return (flag);
	}
	else
		return (HOUR_24);
}


/*-----------------------------------------------------------
get & set Second
-----------------------------------------------------------*/
uint8_t PCF8523::getSeconds()
{
    uint8_t seconds;

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_SECONDS);  // Seconds Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR, 1);
	seconds = Wire.read();
	bitClear(seconds, 7); // Clearing OS Bit if Set.
	return (bcd2bin(seconds));
}

void PCF8523::setSeconds(uint8_t seconds)
{
	uint8_t data, os_bit;
	if (seconds >= 00 && seconds <= 59)
	{
		Wire.beginTransmission(PCF8523_ADDR);
		Wire.write(R_SECONDS);  // Second Register
		Wire.write(bin2bcd(seconds));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getMinutes
-----------------------------------------------------------*/
uint8_t PCF8523::getMinutes()
{
    uint8_t minutes;
    Wire.beginTransmission(PCF8523_ADDR);
    Wire.write(R_MINUTES);  // Minute Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8523_ADDR, 1);
    minutes = Wire.read();
    return (bcd2bin(minutes));
}

void PCF8523::setMinutes(uint8_t minutes)
{
	if(minutes >= 00 && minutes <= 59)
	{
		Wire.beginTransmission(PCF8523_ADDR);
		Wire.write(R_MINUTES);  // Minute Register
		Wire.write(bin2bcd(minutes));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getHours
-----------------------------------------------------------*/
uint8_t PCF8523::getHours()
{
    uint8_t hours;
	bool h_mode;
	h_mode = getHourMode();

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_HOURS);  // Hour Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR, 1);
	hours = Wire.read();
	if (h_mode == CLOCK_H24)
	{
		return (bcd2bin(hours));
	}
	else if (h_mode == CLOCK_H12)
	{
		bitClear(hours, 5);
		return (bcd2bin(hours));
	}
}

void  PCF8523::setHours(uint8_t hours)
{
	bool h_mode;
	if (hours >= 00 && hours <= 23)
	{
		h_mode = getHourMode();

		Wire.beginTransmission(PCF8523_ADDR);
		Wire.write(R_HOURS);  // Hour Register
		if (h_mode == CLOCK_H24)
		{
			Wire.write(bin2bcd(hours));
		}
		else if (h_mode == CLOCK_H12)
		{
			if (hours > 12)
			{
				hours = hours % 12;
				hours = bin2bcd(hours);
				bitSet(hours, 5);
				Wire.write(hours);
			}
			else
			{
				hours = bin2bcd(hours);
				bitClear(hours, 5);
				Wire.write(hours);
			}
		}
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getWeek
-----------------------------------------------------------*/
uint8_t PCF8523::getWeek()
{
	uint8_t week;
	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_WEEKDAYS);  // Week Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR, 1);
	week = Wire.read();
	return week+1;
}

void PCF8523::setWeek(uint8_t week)
{
	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_WEEKDAYS);  // Week Register
	Wire.write(week-1);
	Wire.endTransmission();
}

void PCF8523::updateWeek()
{
	uint16_t y;
	uint8_t m, d, weekday;
	
	y=getYear();
	m=getMonth();
	d=getDay();
	
	weekday  = (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;

	if (weekday >= 1 && weekday <= 7)
	{
		Wire.beginTransmission(PCF8523_ADDR);
		Wire.write(0x07);  // Week Register
		Wire.write(weekday-1);
		Wire.endTransmission();
	}
}



/*-----------------------------------------------------------
getDay
-----------------------------------------------------------*/
uint8_t PCF8523::getDay()
{
    uint8_t day;
    Wire.beginTransmission(PCF8523_ADDR);
    Wire.write(R_DAYS);  // Day Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8523_ADDR, 1);
    day = Wire.read();
    return (bcd2bin(day));
}

void PCF8523::setDay(uint8_t day)
{
    Wire.beginTransmission(PCF8523_ADDR);
    Wire.write(R_DAYS);  // Day Register
    Wire.write(bin2bcd(day));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getMonth()
-----------------------------------------------------------*/
uint8_t PCF8523::getMonth()
{
	uint8_t month;
	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_MONTHS);  // Month Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR, 1);
	month = Wire.read();
	return (bcd2bin(month));
}
/*-----------------------------------------------------------
setMonth() 
-----------------------------------------------------------*/

void PCF8523::setMonth(uint8_t month)
{
	uint8_t data, century_bit;
	if (month >= 1 && month <= 12)
	{
		Wire.beginTransmission(PCF8523_ADDR);
		Wire.write(R_MONTHS);  // Month Register
		Wire.write(month);
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getYear (Completed)
	* Always return 4 Digit year 
	* Takes Care of Century.
-----------------------------------------------------------*/
uint16_t PCF8523::getYear()
{
	uint16_t year;
	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_YEARS);  // Read Month register for Century
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR,1);
 	year = Wire.read();
	return (bcd2bin(year) + 2000);
}

/*-----------------------------------------------------------
setYear (Completed)
	* Accepts both 2 and 4 Digit Years.
-----------------------------------------------------------*/
void PCF8523::setYear(uint16_t year)
{
	if((year >= 00 && year <= 99) || (year >= 2000 && year <= 2099))
	{
		year = year % 100; //Converting to 2 Digit

		// Write 2 Digit year to Year Register
		Wire.beginTransmission(PCF8523_ADDR);
		Wire.write(R_YEARS);
		Wire.write(bin2bcd(year));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
setTime
-----------------------------------------------------------*/

void PCF8523::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    Wire.beginTransmission(PCF8523_ADDR);
    Wire.write(R_SECONDS); 
    Wire.write(bin2bcd(second));	//0x02 Seconds
    Wire.write(bin2bcd(minute));	//0x03 Minutes
    Wire.write(bin2bcd(hour));		//0x04 Hours
    Wire.endTransmission();
}

/*-----------------------------------------------------------
setDate (Should be Optimised)

Not working
-----------------------------------------------------------*/
void PCF8523::setDate(uint8_t day, uint8_t month, uint16_t year)
{
	uint8_t century, data;
	
	// If year is 2 digits.
	if(year < 100)
		year = year + 2000;
	
	century = year  / 100;	// Find Century 
	year = year % 100;		//Converting to 2 Digit

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(0x05);
	Wire.write(bin2bcd(day));	//0x05 Day
	Wire.write(0);				//0x06 Weekday
	Wire.write(bin2bcd(month));	//0x07 Months
	Wire.write(bin2bcd(year));	//0x08 Years
	Wire.endTransmission();

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(0x07);		// Century and month Register
	Wire.endTransmission();

	Wire.requestFrom(PCF8523_ADDR, 1);
	data = Wire.read();

	// Set century bit to 1 for year > 2000;
	if(century == 20)
		bitSet(data,7);
	else
		bitClear(data,7);

	// Write Century bit to Month Register(0x07)
	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(0x07);  //
	Wire.write(data);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
setDateTime()
Taken from https://github.com/adafruit/RTClib/
-----------------------------------------------------------*/

void PCF8523::setDateTime(char* date, char* time)
{
	uint8_t day, month, hour, minute, second;
	uint16_t year;
	// sample input: date = "Dec 26 2009", time = "12:34:56"
	year = atoi(date + 9);
	setYear(year);
	// Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
	switch (date[0]) {
		case 'J': month = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7); break;
		case 'F': month = 2; break;
		case 'A': month = date[2] == 'r' ? 4 : 8; break;
		case 'M': month = date[2] == 'r' ? 3 : 5; break;
		case 'S': month = 9; break;
		case 'O': month = 10; break;
		case 'N': month = 11; break;
		case 'D': month = 12; break;
	}
	setMonth(month);
	day = atoi(date + 4);
	setDay(day);
	hour = atoi(time);
	setHours(hour);
	minute = atoi(time + 3);
	setMinutes(minute);
	second = atoi(time + 6);
	setSeconds(second);
}

/*-----------------------------------------------------------
setEpoch()
	* Weekday Might not work properly
	* Remove Century
-----------------------------------------------------------*/

void PCF8523::setEpoch(time_t epoch)
{
	uint8_t data, century;
	uint16_t year;
	struct tm epoch_tm, *ptr_epoch_tm;

	ptr_epoch_tm = gmtime(&epoch);
	epoch_tm = *ptr_epoch_tm;

	century = (epoch_tm.tm_year + 1870) / 100;	// Find Century 
	year = (epoch_tm.tm_year + 1870) % 100;		//Converting to 2 Digit

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_SECONDS);  // Seconds Register
	Wire.write(bin2bcd(epoch_tm.tm_sec));	//0x02
	Wire.write(bin2bcd(epoch_tm.tm_min));	//0x03
	Wire.write(bin2bcd(epoch_tm.tm_hour));	//0x04
	Wire.write(bin2bcd(epoch_tm.tm_mday));	//0x05
	Wire.write(bin2bcd(epoch_tm.tm_wday));	//0x06
	Wire.write(bin2bcd(epoch_tm.tm_mon+1));	//0x07
	Wire.write(bin2bcd(year));	//0x08
	Wire.endTransmission();

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(0x07);		// Century and month Register
	Wire.endTransmission();

	Wire.requestFrom(PCF8523_ADDR, 1);
	data = Wire.read();

	// Set century bit to 1 for year > 2000;
	if(century == 20)
		bitSet(data,7);
	else
		bitClear(data,7);
	
	// Write Century bit to Month Register(0x07)
	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(0x07);  //
	Wire.write(data);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getEpoch()
	* Weekday Might not work properly
-----------------------------------------------------------*/
time_t PCF8523::getEpoch()
{
	uint8_t century_bit;
	uint16_t century;
	time_t epoch;
	struct tm epoch_tm;
	
	Wire.beginTransmission(PCF8523_ADDR);
    Wire.write(0x02);  // Seconds Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8523_ADDR,7);
	
	epoch_tm.tm_sec = bcd2bin(Wire.read());		//0x02 Seconds
	epoch_tm.tm_min = bcd2bin(Wire.read());		//0x03 Minutes
	epoch_tm.tm_hour = bcd2bin(Wire.read());	//0x04 Hours
	epoch_tm.tm_mday = bcd2bin(Wire.read());	//0x05 Day
	epoch_tm.tm_wday = bcd2bin(Wire.read());	//0x06 Weekday
	epoch_tm.tm_mon = Wire.read();				//0x07 Months
	epoch_tm.tm_year = bcd2bin(Wire.read());	//0x08 Years
	
	// Read Century Bit from Month Register
	century_bit = bitRead(epoch_tm.tm_mon, 7);
	bitClear(epoch_tm.tm_mon,7);
	epoch_tm.tm_mon = bcd2bin(epoch_tm.tm_mon)-1;
	
	if(century_bit == 0)
		century = 1900;
	else
		century = 2000;
	
	epoch_tm.tm_year = epoch_tm.tm_year + century - 1870;

    epoch = mktime(&epoch_tm);
    return (epoch);
}

/* Helpers */

uint8_t PCF8523::bcd2bin (uint8_t val)
{
    return val - 6 * (val >> 4);
}
uint8_t PCF8523::bin2bcd (uint8_t val)
{
    return val + 6 * (val / 10);
}
