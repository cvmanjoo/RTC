/*
 * DS3231.cpp - Library to set & get time from RTC DS3231
 * Created by Manjunath CV. July 08, 2017, 02:18 AM
 * Released into the public domain.
 */

#include <time.h>
#include <Arduino.h>
#include <Wire.h>
#include <I2C_RTC.h>

uint8_t DS3231::begin()
{
	Wire.begin();
    return(DS3231_ADDR);
	//Wire.endTransmission();
}


bool DS3231::isConnected()
{
	Wire.begin(); // join i2c bus
	Wire.beginTransmission(DS3231_ADDR);
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

	// if(data_e == 0 && data_f == 0)
	// 	return true;
	// else
	//return false;
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

	//bitWrite(data, 7, 0); // Write  OSF Register to 0 to start the clock.

	bitClear(data,7); // Write  OSF Register to 0 to start the clock.

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);
	Wire.write(data);
	Wire.endTransmission();


	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();

	//bitWrite(data, 7, 0); // Write  EOSC Register to 0 to start the clock.

	bitClear(data,7); // Write  OSF Register to 0 to start the clock.

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

	// Write  EOSC Register to 1 to diable the Oscillator.
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

	if(h_mode == CLOCK_H12 || h_mode == CLOCK_H24)
	{
		//Clock Hour Register
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x02);
		Wire.endTransmission();

		Wire.requestFrom(DS3231_ADDR, 1);
		data = Wire.read();

		bitWrite(data, 6, h_mode);

		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x02);
		Wire.write(data);
		Wire.endTransmission();

		//Alarm1 Hour Register
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x09);
		Wire.endTransmission();

		Wire.requestFrom(DS3231_ADDR, 1);
		data = Wire.read();

		bitWrite(data, 6, h_mode);

		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x09);
		Wire.write(data);
		Wire.endTransmission();

		//Alarm2 Hour Register
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x0C);
		Wire.endTransmission();

		Wire.requestFrom(DS3231_ADDR, 1);
		data = Wire.read();

		bitWrite(data, 6, h_mode);

		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x0C);
		Wire.write(data);
		Wire.endTransmission();
	}
}

uint8_t DS3231::getHourMode()
{
	bool clock_h_mode, alarm1_h_mode,alarm2_h_mode;
	uint8_t data;

	//Clock Hour Register
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x02);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();

	clock_h_mode = bitRead(data, 6);

	//Clock Alarm1 Register
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x09);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();

	alarm1_h_mode = bitRead(data, 6);

	//Clock Alarm2 Register
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0C);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();

	alarm2_h_mode = bitRead(data, 6);

	return (clock_h_mode | alarm1_h_mode | alarm2_h_mode);

}

/*-----------------------------------------------------------
setMeridiem(uint8_t meridiem)

-----------------------------------------------------------*/

void DS3231::setMeridiem(uint8_t meridiem)
{
	uint8_t data;
	if(meridiem == HOUR_AM || meridiem == HOUR_PM)
	{
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
	uint8_t seconds;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x00);  // Second Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	seconds = Wire.read();
	return (bcd2bin(seconds));
}

void DS3231::setSeconds(uint8_t seconds)
{
	if (seconds >= 00 && seconds <= 59)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x00);  // Second Register
		Wire.write(bin2bcd(seconds));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getMinutes
-----------------------------------------------------------*/

void DS3231::setMinutes(uint8_t minutes)
{
	if (minutes >= 00 && minutes <= 59)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x01);  // Minute Register
		Wire.write(bin2bcd(minutes));
		Wire.endTransmission();
	}
}

uint8_t DS3231::getMinutes()
{
	uint8_t minutes;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x01);  // Minute Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	minutes = Wire.read();
	return (bcd2bin(minutes));
}

/*-----------------------------------------------------------
getHour
-----------------------------------------------------------*/
uint8_t DS3231::getHours()
{
	uint8_t hours;
	bool h_mode;
	h_mode = getHourMode();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x02);  // Hour Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	hours = Wire.read();
	if (h_mode == CLOCK_H24)
	{
		return (bcd2bin(hours));
	}
	else if (h_mode == CLOCK_H12)
	{
		bitClear(hours, 5);
		bitClear(hours, 6);
		return (bcd2bin(hours));
	}
	else
	{
		return -1;
	}
}

void  DS3231::setHours(uint8_t hours)
{
	bool h_mode = getHourMode();
	bool pm_flag;
	if (hours >= 00 && hours <= 23)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x02);  // Hour Register
		if (h_mode == CLOCK_H24)
		{
			Wire.write(bin2bcd(hours));
		}
		else if (h_mode == CLOCK_H12)
		{
			pm_flag = (hours >= 12);
			if (hours == 0)
				hours = 12;
			if (hours > 12)
				hours -= 12;
			hours = bin2bcd(hours);
			bitWrite(hours, 5, pm_flag);
			bitSet(hours, 6);
			Wire.write(hours);
		}
		Wire.endTransmission();
	}
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
	if (week >= 1 && week <= 7)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x03);  // Week Register
		Wire.write(week);
		Wire.endTransmission();
	}
}
/*-----------------------------------------------------------
Update Week based on date

TODO : Use all the data validation
-----------------------------------------------------------*/


void DS3231::updateWeek()
{
	uint16_t y;
	uint8_t m, d, weekday;
	
	y=getYear();
	m=getMonth();
	d=getDay();
	
	weekday  = (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;
	
	if (weekday >= 1 && weekday <= 7)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x03);  // Week Register
		Wire.write(weekday);
		Wire.endTransmission();
	}
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
	if (day >= 1 && day <= 31)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x04);  // Day Register
		Wire.write(bin2bcd(day));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getMonth ()

-----------------------------------------------------------*/
uint8_t DS3231::getMonth()
{
	uint8_t month;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x05);  // Month Register
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	month = Wire.read();
	bitClear(month,7);		//Clear Century Bit;
	return (bcd2bin(month));
}
/*-----------------------------------------------------------
setMonth()
-----------------------------------------------------------*/

void DS3231::setMonth(uint8_t month)
{
	uint8_t data, century_bit;
	if (month >= 1 && month <= 12)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x05);  // Month Register
		Wire.endTransmission();
		Wire.requestFrom(DS3231_ADDR, 1);
		data =  Wire.read();

		//Read Century bit and return it safe
		century_bit = bitRead(data, 7);
		month = bin2bcd(month);
		bitWrite(month,7,century_bit);

		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x05);  // Month Register
		Wire.write(month);
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getYear (Completed)

Century Bit
1 = 1900s
0 = 2000s
-----------------------------------------------------------*/
uint16_t DS3231::getYear()
{
	uint8_t century_bit,data;
	uint16_t century,year;
	
	// Read Month register for Century
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x05);  
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR,1);
	data =  Wire.read();
	century_bit = bitRead(data, 7);
	if(century_bit == 1)
	{
		century = 1900;
	}
	else if(century_bit == 0)
	{
		century = 2000;
	}
	
	//Read Year Register and add Century
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x06);  // Year Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	year = Wire.read();
	year = bcd2bin(year) + century;
	return (year);
}

void DS3231::setYear(uint16_t year)
{
	uint8_t century,data;

	if((year >= 00 && year <= 99) || (year >= 1900 && year <= 2099))
	{
		// If year is 2 digits set to 2000s.
		if(year >= 00 && year <= 99)
			year = year + 2000;

		//Century Calculation
		if(year >= 1900 && year <= 1999)
			century = 1;
		else if (year >= 2000 && year <= 2099)
			century = 0;

		// Find Century 
		year = year % 100;		//Converting to 2 Digit
		
		// Read Century bit from Month Register(0x05)
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x05);		// Century and month Register
		Wire.endTransmission();

		Wire.requestFrom(DS3231_ADDR, 1);
		data = Wire.read();
		
		// Set century bit to 1 for year > 2000;
		if(century == 1)
			bitSet(data,7);
		else
			bitClear(data,7);

		// Write Century bit to Month Register(0x05)
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x05);  // Months Register
		Wire.write(data);
		Wire.endTransmission();

		// Write 2 Digit year to Year Register(0x06)
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x06);  // Year Register to write year
		Wire.write(bin2bcd(year));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
setTime

-----------------------------------------------------------*/

void DS3231::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	if (hours >= 00 && hours <= 23 && minutes >= 00 && minutes <= 59 && seconds >= 00 && seconds <= 59)
	{
		bool h_mode;
		bool pm;
		h_mode = getHourMode();

		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x00);  // Time Register
		Wire.write(bin2bcd(seconds));  	// 0x00
		Wire.write(bin2bcd(minutes));	// 0x01
		if (h_mode == CLOCK_H24)
		{
			Wire.write(bin2bcd(hours));	// 0x02
		}
		else if (h_mode == CLOCK_H12)
		{
			pm = (hours >= 12);
			if (hours == 0) hours = 12;
			if (hours > 12) hours -= 12;
			hours = bin2bcd(hours);
			bitWrite(hours, 5, pm);
			bitSet(hours, 6);
			Wire.write(hours);
		}
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
setDate
	*Take Care Century Bit 
-----------------------------------------------------------*/
void DS3231::setDate(uint8_t day, uint8_t month, uint16_t year)
{
	year = year % 100; //Convert year to 2 Digit

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
setDateTime(__TIMESTAMP__)
Uses Built in variable to set time 

//Timestamp format Fri Mar 08 13:01:53 2024
-----------------------------------------------------------*/

void DS3231::setDateTime(String timestamp)
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

	setWeek(week);
	
	setDay(day);
	setMonth(month);
	setYear(year);

	setHours(hour);
	setMinutes(minute);
	setSeconds(second);
}

/*-----------------------------------------------------------
setEpoch()

https://en.wikipedia.org/wiki/Epoch_(computing)
-----------------------------------------------------------*/
//void DS3231::setEpoch(time_t epoch, bool is_unix_epoch=true)

void DS3231::setEpoch(time_t epoch, bool is_unix_epoch)
{
	uint8_t h_mode, data, century;
	uint16_t year;
	struct tm epoch_tm, *ptr_epoch_tm;

	// adjust UNIX epoch to ARDUINO epoch, otherwise `tm` struct
	// is one year and one (leap) day off.
	if (is_unix_epoch)
		epoch = epoch - UNIX_OFFSET;

	ptr_epoch_tm = gmtime(&epoch);
	epoch_tm = *ptr_epoch_tm;

	century = (epoch_tm.tm_year + 1900) / 100;	// Find Century 
	year = (epoch_tm.tm_year + 1900) % 100;		//Converting to 2 Digit


	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x00);  // Seconds Register
	Wire.write(bin2bcd(epoch_tm.tm_sec));	//0x00 Seconds
	Wire.write(bin2bcd(epoch_tm.tm_min));	//0x01 Minutes
	Wire.endTransmission();
	
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x02);
	Wire.endTransmission();
	
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();

	h_mode = bitRead(data, 6);

	
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x02);
	Wire.write(bin2bcd(epoch_tm.tm_hour));	//0x02 Hours
	Wire.write(bin2bcd(epoch_tm.tm_wday));	//0x03 Week Day
	Wire.write(bin2bcd(epoch_tm.tm_mday));	//0x04 Date 
	Wire.write(bin2bcd(epoch_tm.tm_mon+1));	//0x05 Month
	Wire.write(bin2bcd(year));				//0x06 Year
	Wire.endTransmission();

	/* Convert time to 24Hour if it is in 12 Hour */
	
	if (h_mode == CLOCK_H12)
	{
		//Serial.println("I'm here!");
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x02);  // Hour Register
		
		if (epoch_tm.tm_hour == 0)
		{
			epoch_tm.tm_hour = bin2bcd(12);
			bitSet(epoch_tm.tm_hour, 6);
			bitClear(epoch_tm.tm_hour, 5);
			Wire.write(epoch_tm.tm_hour);
		}
		else if (epoch_tm.tm_hour <= 11)
		{
			epoch_tm.tm_hour = bin2bcd(epoch_tm.tm_hour);
			bitSet(epoch_tm.tm_hour, 6);
			bitClear(epoch_tm.tm_hour, 5);
			Wire.write(epoch_tm.tm_hour);
		}
		else if (epoch_tm.tm_hour == 12)
		{
			epoch_tm.tm_hour = bin2bcd(epoch_tm.tm_hour);
			bitSet(epoch_tm.tm_hour, 6);
			bitSet(epoch_tm.tm_hour, 5);
			Wire.write(epoch_tm.tm_hour);
		}
		else
		{
			epoch_tm.tm_hour -= 12;
			epoch_tm.tm_hour = bin2bcd(epoch_tm.tm_hour);
			bitSet(epoch_tm.tm_hour, 6);
			bitSet(epoch_tm.tm_hour, 5);
			Wire.write(epoch_tm.tm_hour);
		}
		Wire.endTransmission();
	}
	/* Write Century Bit to Century Month Register (0x05) */

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x05);		// Century and month Register
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();

	// Set century bit to 1 for year > 2000;
	if(century == 20)
		bitSet(data,7);
	else
		bitClear(data,7);
	
	// Write Century bit to Month Register(0x05)
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x05);  //
	Wire.write(data);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getEpoch()
-----------------------------------------------------------*/
//time_t DS3231::getEpoch(bool as_unix_epoch=true)
time_t DS3231::getEpoch(bool as_unix_epoch)
{
	uint8_t century_bit;
	uint16_t century;
	time_t epoch;
	struct tm epoch_tm;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x00);  // Seconds Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR,7);

	epoch_tm.tm_sec = bcd2bin(Wire.read());		//0x00 Seconds
	epoch_tm.tm_min = bcd2bin(Wire.read());		//0x01 Minutes
	epoch_tm.tm_hour = bcd2bin(Wire.read());	//0x02 Hours
	epoch_tm.tm_wday = bcd2bin(Wire.read());	//0x03 Week Day
	epoch_tm.tm_mday = bcd2bin(Wire.read());	//0x04 Date
	epoch_tm.tm_mon = Wire.read();				//0x05 Months
	epoch_tm.tm_year = bcd2bin(Wire.read());	//0x06 Years

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
	if(as_unix_epoch)
		epoch += UNIX_OFFSET;
	return (epoch);
}

/*-----------------------------------------------------------
enableAlarm() *Incomplete*
-----------------------------------------------------------*/
void DS3231::enableAlarmPin()
{
	uint8_t reg;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);  // Control Register (0Eh)
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	reg = Wire.read();

	bitWrite(reg, 2, 1); // Write bit INTCN to 1 to enable INT/SQW pin
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);  // Month Register
	Wire.write(reg);
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
	Wire.write(data);
	Wire.endTransmission();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Status Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 0, 0);             // Write  A1F Register to 0 to clear Alaram 1 flag
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.write(data);
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
	Wire.write(data);
	Wire.endTransmission();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Status Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 1, 0);             // Write  A2F Register to 0 to clear Alaram 2 flag
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.write(data);
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
	Wire.write(data);
	Wire.endTransmission();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Status Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 0, 0);             // Write  A1F Register to 0 to clear Alarm 1 flag
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.write(data);
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
	Wire.write(data);
	Wire.endTransmission();

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Status Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitWrite(data, 1, 0);             // Write  A1F Register to 0 to clear Alarm 2 flag
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.write(data);
	Wire.endTransmission();
}

bool DS3231::isAlarm1Enabled()
{
	uint8_t data;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);               // Control Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	return (bitRead(data, 0));
}

bool DS3231::isAlarm2Enabled()
{
	uint8_t data;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);               // Control Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	return (bitRead(data, 1));
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
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x07);
	Wire.write(bin2bcd(second));    // 0x07 Alarm1 Second
	Wire.write(bin2bcd(minute));    // 0x08 Alarm1 Minute
	Wire.write(0x80);               // 0x09 Alarm1 Hour
	Wire.write(0x80);               // 0x0A Alarm1 Day
	Wire.endTransmission();
}

void DS3231::setAlarm1(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	bool h_mode = getHourMode();

	if (hours <= 23 && minutes <= 59 && seconds <= 59)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x07);
		Wire.write(bin2bcd(seconds));    // 0x07 Alarm1 Second
		Wire.write(bin2bcd(minutes));    // 0x08 Alarm1 Minute
		if (h_mode == CLOCK_H24)
		{
			Wire.write(bin2bcd(hours));
		}
		else if (h_mode == CLOCK_H12)
		{
			if (hours == 0)
			{
				hours = bin2bcd(12);
				bitSet(hours, 6);
				bitClear(hours, 5);
				Wire.write(hours);
			}
			else if (hours <= 11)
			{
				hours = bin2bcd(hours);
				bitSet(hours, 6);
				bitClear(hours, 5);
				Wire.write(hours);
			}
			else if (hours == 12)
			{
				hours = bin2bcd(hours);
				bitSet(hours, 6);
				bitSet(hours, 5);
				Wire.write(hours);
			}
			else
			{
				hours -= 12;
				hours = bin2bcd(hours);
				bitSet(hours, 6);
				bitSet(hours, 5);
				Wire.write(hours);
			}
		}
		Wire.write(0x80); 
		Wire.endTransmission();
	}
}

void DS3231::setAlarm1(uint8_t day, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	if (hours <= 23 && minutes <= 59 && seconds <= 59)
	{
		bool h_mode;
		h_mode = getHourMode();

		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x07);
		Wire.write(bin2bcd(seconds));    // 0x07 Alarm1 Second
		Wire.write(bin2bcd(minutes));    // 0x08 Alarm1 Minute
		if (h_mode == CLOCK_H24)
		{
			Wire.write(bin2bcd(hours));
		}
		else if (h_mode == CLOCK_H12)
		{
			if (hours == 0)
			{
				hours = bin2bcd(12);
				bitSet(hours, 6);
				bitClear(hours, 5);
				Wire.write(hours);
			}
			else if (hours <= 11)
			{
				hours = bin2bcd(hours);
				bitSet(hours, 6);
				bitClear(hours, 5);
				Wire.write(hours);
			}
			else if (hours == 12)
			{
				hours = bin2bcd(hours);
				bitSet(hours, 6);
				bitSet(hours, 5);
				Wire.write(hours);
			}
			else
			{
				hours -= 12;
				hours = bin2bcd(hours);
				bitSet(hours, 6);
				bitSet(hours, 5);
				Wire.write(hours);
			}
		}
		Wire.write(bin2bcd(day)); 
		Wire.endTransmission();
	}
}

/*
DateTime DS3231::getAlarm1()
{
	DateTime Alarm1;
	uint8_t seconds,minutes,hours,date;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x07);
	Wire.endTransmission();
    Wire.requestFrom(DS3231_ADDR, 4);

    seconds = Wire.read();
	bitClear(seconds,7);
	Alarm1.seconds = bcd2bin(seconds);

	minutes = Wire.read();
	bitClear(minutes,7);
	Alarm1.minutes = bcd2bin(minutes);

	hours = Wire.read();
	bitClear(hours,7);
	Alarm1.hours = bcd2bin(hours);

	date = Wire.read();
	bitClear(date,7);
	Alarm1.day = bcd2bin(date);

	return(Alarm1);
}
*/
//Alarm2

void DS3231::setAlarm2Minutes(uint8_t minutes)
{
	if (minutes >= 00 && minutes <= 59)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x0B);
		Wire.write(bin2bcd(minutes));

		//Serial.println(bin2bcd(minutes),BIN);
		Wire.endTransmission();
	}
}

uint8_t DS3231::getAlarm2Minutes()
{
	uint8_t minutes;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0B);  // Minute Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	minutes = Wire.read();

	//Serial.println(minutes);

	bitClear(minutes,7);
	return (bcd2bin(minutes));
}




void DS3231::setAlarm2()
{
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0B);
	Wire.write(0x80);    // 0x0B Alarm2 Minute
	Wire.write(0x80);               // 0x0C Alarm2 Hour
	Wire.write(0x80);               // 0x0D Alarm2 Day
	Wire.endTransmission();
}

void DS3231::setAlarm2(uint8_t minute)
{
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0B);
	Wire.write(bin2bcd(minute));    // 0x0B Alarm2 Minute
	Wire.write(0x80);               // 0x0C Alarm2 Hour
	Wire.write(0x80);               // 0x0D Alarm2 Day
	Wire.endTransmission();
}

void DS3231::setAlarm2(uint8_t hour, uint8_t minute)
{
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0B);
	Wire.write(bin2bcd(minute));    // 0x0B Alarm2 Minute
	Wire.write(bin2bcd(hour));      // 0x0C Alarm2 Hour
	Wire.write(0x80);      // 0x0D Alarm2 Day
	Wire.endTransmission();
}

void DS3231::setAlarm2(uint8_t week, uint8_t hours, uint8_t minute)
{
	bool h_mode = getHourMode();
	bool pm_flag;

	if (hours >= 00 && hours <= 23)
	{
		Wire.beginTransmission(DS3231_ADDR);
		Wire.write(0x0B);
		Wire.write(bin2bcd(minute));    // 0x0B Alarm2 Minute
		if (h_mode == CLOCK_H24)
		{
			Wire.write(bin2bcd(hours));
		}
		else if (h_mode == CLOCK_H12)
		{
			pm_flag = (hours >= 12);
			if (hours == 0)
				hours = 12;
			if (hours > 12)
				hours -= 12;
			hours = bin2bcd(hours);
			bitWrite(hours, 5, pm_flag);
			bitSet(hours, 6);			// Week mode
			Wire.write(hours);
		}
		Wire.write(bin2bcd(week));       // 0x0D Alarm2 Day
		Wire.endTransmission();
	}
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

void DS3231::clearAlarm1()
{
	uint8_t data;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitClear(data,0);
	
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);				// Control Register (0Fh)
	Wire.write(data);               
	Wire.endTransmission();
	
}

void DS3231::clearAlarm2()
{
	uint8_t data;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);               // Control Register (0Fh)
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	bitClear(data,1);
	
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0F);				// Control Register (0Fh)
	Wire.write(data);               
	Wire.endTransmission();
}

void DS3231:: setOutPin(uint8_t mode)
{
	uint8_t data;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	
	switch (mode)
	{
		case SQW001Hz:
			bitClear(data,4);
			bitClear(data,3);
			break;
		case SQW01kHz:
			bitClear(data,4);
			bitSet(data,3);
			break;
		case SQW04kHz:
			bitSet(data,4);
			bitClear(data,3);
			break;
		case SQW08kHz:
			bitSet(data,4);
			bitSet(data,3);
			break;
	}
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);
	Wire.write(data);
	Wire.endTransmission();
}

bool DS3231::getINTPinMode()
{
	uint8_t reg;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);  // Control Register
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	reg = Wire.read();
	
	return(bitRead(reg,2));

}

void DS3231::enableSqwePin()
{
	uint8_t reg;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);  
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 1);
	reg = Wire.read();

	bitWrite(reg, 2, 0);
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x0E);  
	Wire.write(reg);
	Wire.endTransmission();
}


/*-----------------------------------------------------------
DS3231 Exclusive Functions
-----------------------------------------------------------*/
int8_t DS3231::getAgingOffset()
{
	int8_t data;
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x10);               // Aging Offset
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	data = Wire.read();
	return data;
}
void DS3231::setAgingOffset(int8_t data)
{
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(0x10);              // Aging Offset
	Wire.write(data);
	Wire.endTransmission();
}


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


uint8_t DS3231::_read_one_register(uint8_t reg_address)
{
    uint8_t reg_data;
    
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.endTransmission();

    Wire.requestFrom((int)_i2c_address,(int) 1);
    reg_data = Wire.read();
    return(reg_data);
}

void DS3231::_write_one_register(uint8_t reg_address, uint8_t reg_data)
{
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.write(reg_data);
    Wire.endTransmission();
}

uint8_t DS3231::bcd2bin(uint8_t val)
{
	return val - 6 * (val >> 4);
}
uint8_t DS3231::bin2bcd(uint8_t val)
{
	return val + 6 * (val / 10);
}
