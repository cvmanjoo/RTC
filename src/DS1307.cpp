/*
* DS1307.cpp - Library to set & get time from RTC DS1307
* Created by Manjunath CV. August 15, 2017, 07:00 PM
* Released into the public domain.
*/

#include <time.h>
#include <Arduino.h>
#include <Wire.h>
#include <I2C_RTC.h>

bool DS1307::begin()
{
	Wire.begin(); // join i2c bus
	Wire.beginTransmission(DS1307_ADDR);
	return (Wire.endTransmission() == 0 ? true : false);
}

bool DS1307::isRunning(void)
{
	uint8_t data;
	bool flag;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	data = Wire.read();
	flag = bitRead(data, 7);
	return (!flag);
}

void DS1307::startClock(void)
{
	uint8_t data;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	data = Wire.read();
	bitClear(data, 7);
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x00);  // Seconds Register
	Wire.write(data);
	Wire.endTransmission();
}

void DS1307::stopClock(void)
{
	uint8_t data;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	data = Wire.read();
	bitSet(data, 7);
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x00);  // Seconds Register
	Wire.write(data);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
get & set HourMode
-----------------------------------------------------------*/

void DS1307::setHourMode(uint8_t h_mode)
{
	uint8_t data;
	if(h_mode == CLOCK_H12 || h_mode == CLOCK_H24)
	{
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x02);  // Hour Register
		Wire.endTransmission();

		Wire.requestFrom(DS1307_ADDR, 1);
		data = Wire.read();
		
		bitWrite(data, 6, h_mode);
		
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x02);  // Hour Register
		Wire.write(data);
		Wire.endTransmission();
	}
}

uint8_t DS1307::getHourMode()
{
	bool h_mode;
	uint8_t data;

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x02);
	Wire.endTransmission();

	Wire.requestFrom(DS1307_ADDR, 1);
	data = Wire.read();

	h_mode = bitRead(data, 6);

	return (h_mode);
}

/*-----------------------------------------------------------
-----------------------------------------------------------*/

void DS1307::setMeridiem(uint8_t meridiem)
{
	uint8_t data;
	if(meridiem == HOUR_AM || meridiem == HOUR_PM)
	{
		if (getHourMode() == CLOCK_H12) 
		{
			Wire.beginTransmission(DS1307_ADDR);
			Wire.write(0x02);  // Hour Register
			Wire.endTransmission();
			Wire.requestFrom(DS1307_ADDR, 1);
			data = Wire.read();
			bitWrite(data, 5, meridiem);
			Wire.beginTransmission(DS1307_ADDR);
			Wire.write(0x02);  // Hour Register
			Wire.write(data);
			Wire.endTransmission();
		}
	}
}

uint8_t DS1307::getMeridiem()
{
	bool flag;
	uint8_t data;
	if (getHourMode() == CLOCK_H12)
	{
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x02);
		Wire.endTransmission();

		Wire.requestFrom(DS1307_ADDR, 1);
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
uint8_t DS1307::getSeconds()
{
	uint8_t seconds;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x00);
	Wire.endTransmission();
	
	Wire.requestFrom(DS1307_ADDR, 1);
	seconds = Wire.read();
	bitClear(seconds, 7); // Clearing CH Bit if Set.
	return (bcd2bin(seconds));
}

void DS1307::setSeconds(uint8_t seconds)
{
	uint8_t data, ch_bit;
	if (seconds >= 00 && seconds <= 59)
	{
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x00);  // Second Register
		Wire.endTransmission();
		Wire.requestFrom(DS1307_ADDR, 1);
		data = Wire.read();

		ch_bit = bitRead(data, 7);
		seconds = bin2bcd(seconds);
		bitWrite(seconds,7,ch_bit);

		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x00);  // Second Register
		Wire.write(bin2bcd(seconds));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getMinute
-----------------------------------------------------------*/
uint8_t DS1307::getMinutes()
{
	uint8_t minute;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x01);  // Minute Register
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	minute = Wire.read();
	return (bcd2bin(minute));
}

void DS1307::setMinutes(uint8_t minutes)
{
	if(minutes >= 00 && minutes <= 59)
	{	
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x01);  // Minute Register
		Wire.write(bin2bcd(minutes));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getHours
-----------------------------------------------------------*/
uint8_t DS1307::getHours()
{
	uint8_t hour;
	bool h_mode;
	h_mode = getHourMode();
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x02);  // Hour Register
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	hour = Wire.read();
	if (h_mode == CLOCK_H24)
	{
		return (bcd2bin(hour));
	}
	else if (h_mode == CLOCK_H12)
	{
		bitClear(hour, 5);
		bitClear(hour, 6);
		return (bcd2bin(hour));
	}
}

void  DS1307::setHours(uint8_t hours)
{
	bool h_mode;
	if (hours >= 00 && hours <= 23)
	{
		h_mode = getHourMode();

		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x02);  // Hour Register
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
				bitSet(hours, 6);
				bitSet(hours, 5);
				Wire.write(hours);
			}
			else
			{
				hours = bin2bcd(hours);
				bitSet(hours, 6);
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
uint8_t DS1307::getWeek()
{
	uint8_t week;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x03);  // Week Register
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	week = Wire.read();
	return week;
}

void DS1307::setWeek(uint8_t week)
{
	if (week >= 1 && week <= 7)
	{
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x03);  // Week Register
		Wire.write(week);
		Wire.endTransmission();
	}
}

void DS1307::updateWeek()
{
	uint16_t y;
	uint8_t m, d, weekday;
	
	y=getYear();
	m=getMonth();
	d=getDay();
	
	weekday  = (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;
	
	if (weekday >= 1 && weekday <= 7)
	{
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x03);  // Week Register
		Wire.write(weekday);
		Wire.endTransmission();
	}
}


/*-----------------------------------------------------------
getDay
-----------------------------------------------------------*/
uint8_t DS1307::getDay()
{
	uint8_t day;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x04);  // Day Register
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	day = Wire.read();
	return (bcd2bin(day));
}

void DS1307::setDay(uint8_t day)
{
	if (day >= 1 && day <= 31)
	{
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x04);  // Day Register
		Wire.write(bin2bcd(day));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getMonth()
-----------------------------------------------------------*/
uint8_t DS1307::getMonth()
{
	uint8_t month;

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x05);  // Month Register
	Wire.endTransmission();

	Wire.requestFrom(DS1307_ADDR, 1);
	month = Wire.read();

	return (bcd2bin(month));
}
/*-----------------------------------------------------------
setMonth()
-----------------------------------------------------------*/

void DS1307::setMonth(uint8_t month)
{
	if (month >= 1 && month <= 12)
	{
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x05);  // Month Register
		Wire.write(bin2bcd(month));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getYear (Completed)
-----------------------------------------------------------*/
uint16_t DS1307::getYear()
{
	uint16_t year;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x06);  // Year Register
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	year = Wire.read();
	return (bcd2bin(year) + 2000);
}

void DS1307::setYear(uint16_t year)
{	
	if((year >= 00 && year <= 99) || (year >= 2000 && year <= 2099))
	{

		year = year % 100; //Converting to 2 Digit

		Wire.beginTransmission(DS1307_ADDR);	/* Writing 2 Digit year to Year Register(0x06) */
		Wire.write(0x06);  // Year Register to write year
		Wire.write(bin2bcd(year));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
setTime

TODO : Use all the data validation
-----------------------------------------------------------*/

void DS1307::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	bool h_mode;
	if (hours >= 00 && hours <= 23 && minutes >= 00 && minutes <= 59 && seconds >= 00 && seconds <= 59)
	{
		h_mode = getHourMode();
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(0x00);
		Wire.write(bin2bcd(seconds));
		Wire.write(bin2bcd(minutes));
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
				bitSet(hours, 6);
				bitSet(hours, 5);
				Wire.write(hours);
			}
			else
			{
				hours = bin2bcd(hours);
				bitSet(hours, 6);
				bitClear(hours, 5);
				Wire.write(hours);
			}
		}
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
setDate
-----------------------------------------------------------*/
void DS1307::setDate(uint8_t day, uint8_t month, uint16_t year)
{
	if(day >= 1 && day <= 31 && month >= 1 && month <= 12 && ((year >= 00 && year <= 99) || (year >= 2000 && year <= 2099)))

	year = year % 100; //Convert year to 2 Digit
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x04);
	Wire.write(bin2bcd(day));
	Wire.write(bin2bcd(month));
	Wire.write(bin2bcd(year));
	Wire.endTransmission();
}
/*-----------------------------------------------------------
setDateTime()
Taken from https://github.com/adafruit/RTClib/
-----------------------------------------------------------*/

void DS1307::setDateTime(char* date, char* time)
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
-----------------------------------------------------------*/

//void DS1307::setEpoch(time_t epoch, bool is_unix_epoch=true)
void DS1307::setEpoch(time_t epoch, bool is_unix_epoch)
{
	struct tm epoch_tm, * ptr_epoch_tm;
	uint16_t year;
	// adjust UNIX epoch to ARDUINO epoch, otherwise `tm` struct
	// is one year and one (leap) day off.
	if(is_unix_epoch)
		epoch = epoch - UNIX_OFFSET;

	ptr_epoch_tm = gmtime(&epoch);
	epoch_tm = *ptr_epoch_tm;
	setSeconds(epoch_tm.tm_sec); //0x00 - Seconds
	setMinutes(epoch_tm.tm_min);
	setHours(epoch_tm.tm_hour);
	setWeek(epoch_tm.tm_wday + 1);
	setDay(epoch_tm.tm_mday);
	setMonth(epoch_tm.tm_mon + 1);
	setYear(epoch_tm.tm_year + 1900);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getEpoch()
-----------------------------------------------------------*/
//time_t DS1307::getEpoch(bool as_unix_epoch=true)
time_t DS1307::getEpoch(bool as_unix_epoch)
{
	time_t epoch;
	struct tm epoch_tm;
	epoch_tm.tm_sec = getSeconds();
	epoch_tm.tm_min = getMinutes();
	epoch_tm.tm_hour = getHours();
	epoch_tm.tm_wday = getWeek() - 1;
	epoch_tm.tm_mday = getDay();
	epoch_tm.tm_mon = getMonth() - 1;
	epoch_tm.tm_year = getYear() - 1900;
	epoch = mktime(&epoch_tm);
	if(as_unix_epoch)
		epoch += UNIX_OFFSET;
	return (epoch);
}

/* SQW/OUT pin functions */

void DS1307::setOutPin(uint8_t mode)
{
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x07);
	switch (mode) {
	case HIGH:
		Wire.write(B10000000);
		break;
	case LOW:
		Wire.write(B00000000);
		break;
	case SQW001Hz:
		Wire.write(B00010000);
		break;
	case SQW04kHz:
		Wire.write(B00010001);
		break;
	case SQW08kHz:
		Wire.write(B00010010);
		break;
	case SQW32kHz:
		Wire.write(B00010011);
		break;
	}
	Wire.endTransmission();
}

bool DS1307::isOutPinEnabled()
{
	uint8_t data;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x07);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	data = Wire.read();
	data = bitRead(data, 7);
	return (data);
}

bool DS1307::isSqweEnabled()
{
	uint8_t data;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x07);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	data = Wire.read();
	data = bitRead(data, 4);
	return (data);
}

/* NVRAM Functions */

bool NVRAM::begin()
{
	Wire.begin(); // join i2c bus
	Wire.beginTransmission(0x68);
	return (Wire.endTransmission() == 0 ? true : false);
}


uint8_t NVRAM::read(uint8_t address)
{
	uint8_t data = 0x00;
	address = (address % length) + 0x08;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(address);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	data = Wire.read();
	return (data);
}
void NVRAM::write(uint8_t address, uint8_t data)
{
	address = (address % length) + 0x08;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(address);
	Wire.write(data);
	Wire.endTransmission();
}

void NVRAM::read(uint8_t address, uint8_t* buf, uint8_t size)
{
	int addrByte = 0x08 + address;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(addrByte);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 9);
	for (uint8_t pos = 0; pos < size; ++pos)
	{
		buf[pos] = Wire.read();
	}
}

void NVRAM::write(uint8_t address, uint8_t* buf, uint8_t size)
{
	int addrByte = 0x08 + address;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(addrByte);
	for (uint8_t pos = 0; pos < size; ++pos)
	{
		Wire.write(buf[pos]);
	}
	Wire.endTransmission();
}

/* Helpers */

uint8_t DS1307::bcd2bin(uint8_t val)
{
	return val - 6 * (val >> 4);
}
uint8_t DS1307::bin2bcd(uint8_t val)
{
	return val + 6 * (val / 10);
}