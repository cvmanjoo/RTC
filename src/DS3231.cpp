/*
 * DS3231.cpp - Library to set & get time from RTC DS3231
 * Created by Manjunath CV. July 08, 2017, 02:18 AM
 * Released into the public domain.
 */

#include <time.h>
#include <Wire.h>
#include <RTC.h>

bool DS3231::begin()
{
	Wire.begin(); // join i2c bus
	Wire.beginTransmission(0x68);
	return (Wire.endTransmission() == 0 ? true : false);
}

bool DS3231::isRunning(void)
{
	uint8_t data_e, data_f;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 2);

	data_e = Wire.read();	// Read EOSC Register to check Enable Oscillator
	data_f = Wire.read();	// Read  OSF Register to check status.

	data_e = bitRead(data_e, 7);
	data_f = bitRead(data_f, 7);

	return (!(data_e | data_f));
	//return 1;
}
/*
 * Working But Incomplete
 */


void DS3231::startClock(void)
{
	uint8_t data;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();

	bitWrite(data, 7, 0); // Write  OSF Register to 0 to start the clock.

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);
	Wire.write(data);
	Wire.endTransmission();


	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();

	bitWrite(data, 7, 0); // Write  EOSC Register to 0 to start the clock.

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);
	Wire.write(data);
	Wire.endTransmission();

}

/*
 *
 */

void DS3231::stopClock(void)
{
	uint8_t data;

	// Write  OSF Register to 1 to start the clock.
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();

	bitWrite(data, 7, 1);

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);
	Wire.write(data);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
get & set HourMode
-----------------------------------------------------------*/

void DS3231::setHourMode(uint8_t h_mode)
{
	uint8_t data;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x02);  // Hour Register
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();

	bitWrite(data, 6, h_mode);

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x02);  // Hour Register
	Wire.write(data);
	Wire.endTransmission();
}

uint8_t DS3231::getHourMode()
{
	uint8_t data, h_mode;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x02);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();

	h_mode = bitRead(data, 6);

	return (h_mode);
}

/*-----------------------------------------------------------
get & set HourMode

  void

-----------------------------------------------------------*/

void DS3231::setMeridiem(uint8_t meridiem)
{
	uint8_t data;
	if (getHourMode() == CLOCK_H12)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x02);  // Hour Register
		Wire.endTransmission();
		Wire.requestFrom(DS3231_ADDR, 1);
		data = Wire.read();
		bitWrite(data, 5, meridiem);
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x02);  // Hour Register
		Wire.write(data);
		Wire.endTransmission();
	}
}

uint8_t DS3231::getMeridiem()
{
	bool flag;
	uint8_t data;
	if (getHourMode() == CLOCK_H12)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x02);
		Wire.endTransmission();

		Wire.requestFrom(DS3231_ADDR, 1);
		data = Wire.read();

		flag = bitRead(data, 5);
		return (flag);
	}
	else
		return (HOUR_24);
}

uint8_t DS3231::getSeconds()
{
	uint8_t second;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x00);  // Second Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	second = Wire.read();
	return (bcd2bin(second));
}

void DS3231::setSeconds(uint8_t second)
{
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x00);  // Second Register
	Wire.write(bin2bcd(second));
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getMinute
-----------------------------------------------------------*/
uint8_t DS3231::getMinutes()
{
	uint8_t minute;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x01);  // Minute Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	minute = Wire.read();
	return (bcd2bin(minute));
}

void DS3231::setMinutes(uint8_t minute)
{
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x01);  // Minute Register
	Wire.write(bin2bcd(minute));
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getHour
-----------------------------------------------------------*/
uint8_t DS3231::getHours()
{
	uint8_t hour;
	bool h_mode;
	h_mode = getHourMode();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x02);  // Hour Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	hour = Wire.read();
	if (h_mode == CLOCK_H24)
	{
		return (bcd2bin(hour));
	}
	if (h_mode == CLOCK_H12)
	{
		bitClear(hour, 5);
		bitClear(hour, 6);
		return (bcd2bin(hour));
	}
}

void  DS3231::setHours(uint8_t hour)
{
	bool h_mode, meridiem;
	h_mode = getHourMode();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x02);  // Hour Register

	if (h_mode == CLOCK_H24)
	{
		Wire.write(bin2bcd(hour));
	}
	else if (h_mode == CLOCK_H12)
	{
		if (hour == 0)
			hour = 12;
		else if (hour < 12)
		{
			hour = bin2bcd(hour);
			bitSet(hour, 6);
			bitClear(hour, 5);
			Wire.write(hour);
		}
		else
		{
			hour = hour % 12;
			hour = bin2bcd(hour);
			bitSet(hour, 6);
			bitSet(hour, 5);
			Wire.write(hour);
		}
	}
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getWeek
-----------------------------------------------------------*/
uint8_t DS3231::getWeek()
{
	uint8_t week;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x03);  // Week Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	week = Wire.read();
	return week;
}

void DS3231::setWeek(uint8_t week)
{
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x03);  // Minute Register
	Wire.write(week);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getDay
-----------------------------------------------------------*/
uint8_t DS3231::getDay()
{
	uint8_t day;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x04);  // Day Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	day = Wire.read();
	return (bcd2bin(day));
}

void DS3231::setDay(uint8_t day)
{
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x04);  // Day Register
	Wire.write(bin2bcd(day));
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getMonth (Perfect)
-----------------------------------------------------------*/
uint8_t DS3231::getMonth()
{
	uint8_t month;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x05);  // Month Register
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	month = Wire.read();

	return (bcd2bin(month));
}
/*-----------------------------------------------------------
setMonth (Perfect)
-----------------------------------------------------------*/

void DS3231::setMonth(uint8_t month)
{
	uint8_t century;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x05);  // Month Register
	Wire.write(month);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getYear (Completed)
-----------------------------------------------------------*/
uint16_t DS3231::getYear()
{
	uint8_t  data;
	uint16_t year;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x06);  // Year Register
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	year = bcd2bin(data) + 2000;
	return (year);
}

void DS3231::setYear(uint16_t year)
{
	year = year % 100; 						//Converting to 2 Digit   
	Wire.beginTransmission(DS3231_ADDR);	/* Writing 2 Digit year to Year Register(0x06) */
	Wire.write(0x06);  						// Year Register to write year
	Wire.write(bin2bcd(year));
	Wire.endTransmission();
}

/*-----------------------------------------------------------
setTime
-----------------------------------------------------------*/

void DS3231::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
	bool h_mode, meridiem;
	h_mode = getHourMode();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x00);  // Time Register
	Wire.write(bin2bcd(second));
	Wire.write(bin2bcd(minute));
	if (h_mode == CLOCK_H24)
	{
		Wire.write(bin2bcd(hour));
	}
	else if (h_mode == CLOCK_H12)
	{
		if (hour == 0)
		{
			hour = bin2bcd(12);
			bitSet(hour, 6);
			bitClear(hour, 5);
			Wire.write(hour);
			Wire.write(bin2bcd(hour));
		}
		else if (hour <= 11)
		{
			hour = bin2bcd(hour);
			bitSet(hour, 6);
			bitClear(hour, 5);
			Wire.write(hour);
		}
		else if(hour == 12)
		{
			hour = bin2bcd(hour);
			bitSet(hour, 6);
			bitSet(hour, 5);
			Wire.write(hour);
		}
		else
		{
			hour = hour % 12;
			hour = bin2bcd(hour);
			bitSet(hour, 6);
			bitSet(hour, 5);
			Wire.write(hour);
		}
	}
	Wire.endTransmission();
}

/*-----------------------------------------------------------
setDate
-----------------------------------------------------------*/
void DS3231::setDate(uint8_t day, uint8_t month, uint16_t year)
{
	year = year % 100; //Converting to 2 Digit

	Wire.beginTransmission(DS3231_ADDR);
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

void DS3231::setDateTime(char* date, char* time)
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

void DS3231::setEpoch(time_t epoch, time_t e_year, int16_t offset)
{
	time_t rawtime;
	struct tm epoch_tm, * ptr_epoch_tm;
	uint16_t year;

	epoch = epoch - e_year;

	rawtime = epoch;
	ptr_epoch_tm = gmtime(&rawtime);
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
time_t DS3231::getEpoch()
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

	return (epoch);
}

/*-----------------------------------------------------------
enableAlarm() *Incomplete*
-----------------------------------------------------------*/
void DS3231::enableAlarmPin()
{
	uint8_t reg;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);  // Hour Register
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	reg = Wire.read();

	bitWrite(reg, 2, 1); // Write bit INTCN to 1 to enable INT/SQW pin
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);  // Month Register
	Wire.write(bin2bcd(reg));
	Wire.endTransmission();
}

void DS3231::enableAlarm1()
{
	uint8_t data;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);               // Control Register (0Eh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 0, 1);             // Write  A1IE Register to 1 to enable Alarm 1
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);               // Control Register (0Eh)
	Wire.write(bin2bcd(data));
	Wire.endTransmission();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Status Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 0, 0);             // Write  A1F Register to 0 to clear Alaram 1 flag
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.write(bin2bcd(data));
	Wire.endTransmission();
}

void DS3231::enableAlarm2()
{
	uint8_t data;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);               // Control Register (0Eh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 1, 1);             // Write  A2IE Register to 1 to enable Alarm 2
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);               // Control Register (0Eh)
	Wire.write(bin2bcd(data));
	Wire.endTransmission();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Status Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 1, 0);             // Write  A2F Register to 0 to clear Alaram 2 flag
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.write(bin2bcd(data));
	Wire.endTransmission();
}

void DS3231::disableAlarm1()
{
	uint8_t data;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);               // Control Register (0Eh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 0, 0);             // Write  A1IE Register to 0 to disable Alarm 1
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);               // Control Register (0Eh)
	Wire.write(bin2bcd(data));
	Wire.endTransmission();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Status Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 0, 0);             // Write  A1F Register to 0 to clear Alaram 1 flag
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.write(bin2bcd(data));
	Wire.endTransmission();
}

void DS3231::disableAlarm2()
{
	int8_t data;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);               // Control Register (0Eh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 1, 0);             // Write  A1IE Register to 0 to disable Alarm 2
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);               // Control Register (0Eh)
	Wire.write(bin2bcd(data));
	Wire.endTransmission();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Status Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 1, 0);             // Write  A1F Register to 0 to clear Alaram 2 flag
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.write(bin2bcd(data));
	Wire.endTransmission();
}


/*-----------------------------------------------------------
setAlarm1() *Incomplete*
-----------------------------------------------------------*/

void DS3231::setAlarm1()
{
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x07);
	Wire.write(0x80);   // 0x07 Alarm2 Minute
	Wire.write(0x80);   // 0x08 Alarm2 Hour
	Wire.write(0x80);   // 0x09 Alarm2 Day
	Wire.write(0x80);   // 0x0A Alarm2 Day
	Wire.endTransmission();
}

void DS3231::setAlarm1(uint8_t second)
{
	enableAlarm1();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x07);
	Wire.write(bin2bcd(second));    // 0x07 Alarm1 Second
	Wire.write(0x80);               // 0x08 Alarm1 Minute
	Wire.write(0x80);               // 0x09 Alarm1 Hour
	Wire.write(0x80);               // 0x0A Alarm1 Day
	Wire.endTransmission();
}

void DS3231::setAlarm1(uint8_t minute, uint8_t second)
{
	enableAlarm1();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x07);
	Wire.write(bin2bcd(second));    // 0x07 Alarm1 Second
	Wire.write(bin2bcd(minute));    // 0x08 Alarm1 Minute
	Wire.write(0x80);               // 0x09 Alarm1 Hour
	Wire.write(0x80);               // 0x0A Alarm1 Day
	Wire.endTransmission();
}

void DS3231::setAlarm1(uint8_t hour, uint8_t minute, uint8_t second)
{
	enableAlarm1();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x07);
	Wire.write(bin2bcd(second));    // 0x07 Alarm1 Second
	Wire.write(bin2bcd(minute));    // 0x08 Alarm1 Minute
	Wire.write(bin2bcd(hour));      // 0x09 Alarm1 Hour
	Wire.write(0x80);               // 0x0A Alarm1 Day
	Wire.endTransmission();
}

void DS3231::setAlarm1(uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
	enableAlarm1();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x07);
	Wire.write(bin2bcd(second));    // 0x07 Alarm1 Second
	Wire.write(bin2bcd(minute));    // 0x08 Alarm1 Minute
	Wire.write(bin2bcd(hour));      // 0x09 Alarm1 Hour
	Wire.write(bin2bcd(day));       // 0x0A Alarm1 Day
	Wire.endTransmission();
}

//Alarm2

void DS3231::setAlarm2()
{
	enableAlarm2();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0B);
	Wire.write(0x80);    // 0x0B Alarm2 Minute
	Wire.write(0x80);               // 0x0C Alarm2 Hour
	Wire.write(0x80);               // 0x0D Alarm2 Day
	Wire.endTransmission();
}

void DS3231::setAlarm2(uint8_t minute)
{
	enableAlarm2();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0B);
	Wire.write(bin2bcd(minute));    // 0x0B Alarm2 Minute
	Wire.write(0x80);               // 0x0C Alarm2 Hour
	Wire.write(0x80);               // 0x0D Alarm2 Day
	Wire.endTransmission();
}

void DS3231::setAlarm2(uint8_t hour, uint8_t minute)
{
	enableAlarm2();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0B);
	Wire.write(bin2bcd(minute));    // 0x0B Alarm2 Minute
	Wire.write(bin2bcd(hour));      // 0x0C Alarm2 Hour
	Wire.write(0x80);      // 0x0D Alarm2 Day
	Wire.endTransmission();
}

void DS3231::setAlarm2(uint8_t day, uint8_t hour, uint8_t minute)
{
	enableAlarm2();

	//Serial.print("CPP:Minute : ");
	//Serial.print(minute,BIN);
	//Serial.println();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0B);
	Wire.write(bin2bcd(minute));    // 0x0B Alarm2 Minute
	Wire.write(bin2bcd(hour));      // 0x0C Alarm2 Hour
	Wire.write(bin2bcd(day));       // 0x0D Alarm2 Day
	Wire.endTransmission();
}

bool DS3231::isAlarm1Tiggered()
{
	uint8_t data;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	return (bitRead(data, 0));
}

bool DS3231::isAlarm2Tiggered()
{
	uint8_t data;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	return (bitRead(data, 1));
}

/*-----------------------------------------------------------
DS3231 Exclusive Functions
-----------------------------------------------------------*/

float DS3231::getTemp()
{
	float f_temp;
	uint8_t temp_msb, temp_lsb;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x11);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 2);
	temp_msb = Wire.read();
	temp_lsb = Wire.read() >> 6;

	f_temp = (0.25 * temp_lsb) + temp_msb;

	return(f_temp);
}

/* Helpers */

uint8_t DS3231::bcd2bin(uint8_t val)
{
	return val - 6 * (val >> 4);
}
uint8_t DS3231::bin2bcd(uint8_t val)
{
	return val + 6 * (val / 10);
}
