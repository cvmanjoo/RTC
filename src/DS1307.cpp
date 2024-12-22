/*
* DS1307.cpp - Library to set & get time from RTC DS1307
* Created by Manjunath CV. August 15, 2017, 07:00 PM
* Released into the public domain.
*/

#include <time.h>
#include <string.h>
#include <Arduino.h>
#include <Wire.h>
#include <I2C_RTC.h>

#define R_SECONDS 0x00
#define R_MINUTES 0x01
#define R_HOURS   0x02
#define R_WEEKDAY 0x03
#define R_DATE    0x04
#define R_MONTH   0x05
#define R_YEAR	  0x06
#define R_CONTROL 0X07

#define CH 7

// DS1307::DS1307()
// {
// 	/* May be in future But do not Wire.begin() here */
// }

uint8_t DS1307::begin()
{ 
	Wire.begin();
    return(DS1307_ADDR);
	//Wire.endTransmission();
}

bool DS1307::isConnected()
{
	Wire.begin();
	Wire.beginTransmission(DS1307_ADDR);
	return(Wire.endTransmission() == 0 ? true : false);
}

bool DS1307::isRunning()
{
	uint8_t reg_data;
   	reg_data = _read_one_register(R_SECONDS);
   	return(!bitRead(reg_data,CH));
}

void DS1307::startClock(void)
{
	uint8_t reg_data;	
	reg_data = _read_one_register(R_SECONDS);
	bitClear(reg_data, CH);
	_write_one_register(R_SECONDS,reg_data);

}

void DS1307::stopClock(void)
{
	uint8_t reg_data;	
	reg_data = _read_one_register(R_SECONDS);
	bitSet(reg_data, CH);
	_write_one_register(R_SECONDS,reg_data);
}

/*-----------------------------------------------------------
get & set HourMode
-----------------------------------------------------------*/

void DS1307::setHourMode(uint8_t h_mode)
{
	uint8_t reg_data;

	if(h_mode == CLOCK_H12 || h_mode == CLOCK_H24)
	{
		reg_data = _read_one_register(R_HOURS);
		bitWrite(reg_data, 6, h_mode);
		_write_one_register(R_HOURS,reg_data);
	}
}

uint8_t DS1307::getHourMode()
{
	uint8_t reg_data, h_mode;
	reg_data = _read_one_register(R_HOURS);
	h_mode = bitRead(reg_data, 6);
	return (h_mode);
}

/*-----------------------------------------------------------
-----------------------------------------------------------*/

void DS1307::setMeridiem(uint8_t meridiem)
{
	uint8_t reg_data, h_mode;
	if(meridiem == HOUR_AM || meridiem == HOUR_PM)
	{	
		reg_data = _read_one_register(R_HOURS);
		h_mode = bitRead(reg_data, 6);  // Read Hour mode from 6th bit
		if (h_mode == CLOCK_H12) 		// Set Meridiem only if time is 12 - Hour Mode
		{
			bitWrite(reg_data, 5, meridiem);
			_write_one_register(R_HOURS,reg_data);

		}
	}
}

uint8_t DS1307::getMeridiem()
{
	uint8_t reg_data, h_mode, meridiem;
	reg_data = _read_one_register(R_HOURS);
	h_mode = bitRead(reg_data, 6);  // Read Hour mode from 6th bit
	if (h_mode == CLOCK_H12) 		// Get Meridiem only if time is 12 - Hour Mode
	{
		meridiem = bitRead(reg_data, 5);
		return (meridiem);
	}
	else
		return (HOUR_24);
}

/*-----------------------------------------------------------
get & set Second
-----------------------------------------------------------*/
uint8_t DS1307::getSeconds()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_SECONDS);
	bitClear(reg_data, CH); // Clearing CH Bit if Set.
	return (bcd2bin(reg_data));
}

void DS1307::setSeconds(uint8_t seconds)
{
	uint8_t reg_data, ch_bit;

	if (seconds >= 00 && seconds <= 59)
	{
		reg_data = _read_one_register(R_SECONDS);
		ch_bit = bitRead(reg_data, CH);
		seconds = bin2bcd(seconds);
		bitWrite(seconds,CH,ch_bit);
		_write_one_register(R_SECONDS,seconds);
	}
}

/*-----------------------------------------------------------
getMinute
-----------------------------------------------------------*/
uint8_t DS1307::getMinutes()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_MINUTES);
	return (bcd2bin(reg_data));

}

void DS1307::setMinutes(uint8_t minutes)
{
	uint8_t reg_data;
	if(minutes >= 00 && minutes <= 59)
	{	
		_write_one_register(R_MINUTES,bin2bcd(minutes));
	}
}

/*-----------------------------------------------------------
getHours
-----------------------------------------------------------*/
uint8_t DS1307::getHours()
{
	uint8_t reg_data, h_mode;

	reg_data = _read_one_register(R_HOURS);
	h_mode = bitRead(reg_data, 6);  // Read Hour mode from 6th bit
	if(h_mode == CLOCK_H24)
	{
		return (bcd2bin(reg_data));
	}
	else //h_mode == CLOCK_H12
	{
		bitClear(reg_data, 5);
		bitClear(reg_data, 6);
		return (bcd2bin(reg_data));
	}
}

void  DS1307::setHours(uint8_t hours)
{
	uint8_t reg_hour; 
	bool h_mode, meridiem;

	reg_hour = _read_one_register(R_HOURS);
	h_mode = bitRead(reg_hour, 6); //Get hour mode

	if (hours >= 00 && hours <= 23)
	{
		//h_mode = getHourMode();

		if (h_mode == CLOCK_H24)
		{
			_write_one_register(R_HOURS,bin2bcd(hours));
		}
		else if (h_mode == CLOCK_H12)
		{
			meridiem = (hours >= 12);
			if (hours == 0)
				hours = 12;
			if (hours > 12)
				hours -= 12;
			hours = bin2bcd(hours);
			bitWrite(hours, 5, meridiem);
			bitSet(hours, 6);
			_write_one_register(R_HOURS,hours);
		}
	}
}

/*-----------------------------------------------------------
getWeek
-----------------------------------------------------------*/
uint8_t DS1307::getWeek()
{
	uint8_t reg_week;
	reg_week = _read_one_register(R_WEEKDAY);
	return reg_week;
}

void DS1307::setWeek(uint8_t week)
{
	if (week >= 1 && week <= 7)
		_write_one_register(R_WEEKDAY,week);
}

/*-----------------------------------------------------------
getDay
-----------------------------------------------------------*/
uint8_t DS1307::getDay()
{
	uint8_t reg_date;
	reg_date = _read_one_register(R_DATE);
	return(bcd2bin(reg_date));
}

void DS1307::setDay(uint8_t day)
{
	if (day >= 1 && day <= 31)
		_write_one_register(R_DATE,bin2bcd(day));
}

/*-----------------------------------------------------------
getMonth()
-----------------------------------------------------------*/
uint8_t DS1307::getMonth()
{
	uint8_t reg_month;
	reg_month = _read_one_register(R_MONTH);
	return(bcd2bin(reg_month));
}
/*-----------------------------------------------------------
setMonth()
-----------------------------------------------------------*/

void DS1307::setMonth(uint8_t month)
{
	if (month >= 1 && month <= 12)
		_write_one_register(R_MONTH,bin2bcd(month));
}

/*-----------------------------------------------------------
getYear (Completed)
-----------------------------------------------------------*/
uint16_t DS1307::getYear()
{
	uint8_t reg_year;
	reg_year = _read_one_register(R_YEAR);
	return(bcd2bin(reg_year) + 2000);
}

void DS1307::setYear(uint16_t year)
{	
	if((year >= 00 && year <= 99) || (year >= 2000 && year <= 2099))
	{
		year = year % 100; //Converting to 2 Digit
		_write_one_register(R_YEAR,bin2bcd(year));
	}
}

/*-----------------------------------------------------------
setTime

-----------------------------------------------------------*/

void DS1307::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	uint8_t reg_hours, reg_seconds, h_mode, ch_bit;

	reg_hours = _read_one_register(R_HOURS);
	reg_seconds = _read_one_register(R_SECONDS);

	if (hours >= 00 && hours <= 23 && minutes >= 00 && minutes <= 59 && seconds >= 00 && seconds <= 59)
	{
		h_mode = bitRead(reg_hours, 6); //Get hour mode
		ch_bit = bitRead(reg_seconds, CH); // Check the oscillator status to retain the status

		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(R_SECONDS);

		seconds = bin2bcd(seconds);
		bitWrite(seconds,CH,ch_bit);
		Wire.write(seconds);
		
		Wire.write(bin2bcd(minutes));
		
		if (h_mode == CLOCK_H24)
		{
			Wire.write(bin2bcd(hours));
		}
		else if (h_mode == CLOCK_H12)
		{
			if(hours < 12)
			{
				hours = bin2bcd(hours);
				bitSet(hours, 6);
				bitClear(hours, 5); //AM
				Wire.write(hours);
			}
			else if (hours == 12)
			{	
				hours = bin2bcd(hours);
				bitSet(hours, 6); // Set to 12 Hour mode again					
				bitSet(hours, 5); // PM
				Wire.write(hours);
			}
			else if (hours > 12)
			{
				hours = hours % 12;
				hours = bin2bcd(hours);
				bitSet(hours, 6); // Set to 12 Hour mode again
				bitSet(hours, 5); // PM
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
	Wire.write(R_DATE);
	Wire.write(bin2bcd(day));
	Wire.write(bin2bcd(month));
	Wire.write(bin2bcd(year));
	Wire.endTransmission();
}
/*-----------------------------------------------------------
setDateTime(__DATE__,__TIME__)

// sample input

__DATE__ = "Dec 26 2009"
__TIME__ = "12:34:56"

-----------------------------------------------------------*/

void DS1307::setDateTime(String date, String time)
{
	uint8_t reg_hours, reg_seconds, h_mode, ch_bit;
	uint8_t day, month, hours, minutes, seconds, year;
	bool meridiem;

	reg_hours   = _read_one_register(R_HOURS);
	reg_seconds = _read_one_register(R_SECONDS);

	h_mode = bitRead(reg_hours, 6); //Get hour mode
	ch_bit = bitRead(reg_seconds, CH); // Check the oscillator status to retain the status
	
	// Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
	switch (date[0])
	{
		case 'J': month = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7); break;
		case 'F': month = 2; break;
		case 'A': month = date[2] == 'r' ? 4 : 8; break;
		case 'M': month = date[2] == 'r' ? 3 : 5; break;
		case 'S': month = 9; break;
		case 'O': month = 10; break;
		case 'N': month = 11; break;
		case 'D': month = 12; break;
	}

	hours  = time.substring(0,2).toInt();
	minutes = time.substring(3,5).toInt();
	seconds = time.substring(6,8).toInt();

	day = date.substring(4,6).toInt();
	year = date.substring(9,11).toInt();

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(R_SECONDS);

	seconds = bin2bcd(seconds);
	bitWrite(seconds,CH,ch_bit);
	Wire.write(seconds);
	
	Wire.write(bin2bcd(minutes));
	
	if (h_mode == CLOCK_H24)
	{
		Wire.write(bin2bcd(hours));
	}
	else if (h_mode == CLOCK_H12)
	{
		meridiem = (hours >= 12);
		if (hours == 0)
			hours = 12;
		if (hours > 12)
			hours -= 12;
		hours = bin2bcd(hours);
		bitWrite(hours, 5, meridiem);
		bitSet(hours, 6);
		Wire.write(hours);
	}
	
	Wire.write(calculateDayOfWeek(day, month, year));
	Wire.write(bin2bcd(day));
	Wire.write(bin2bcd(month));
	Wire.write(bin2bcd(year));
	Wire.endTransmission();

}

/*-----------------------------------------------------------
	setDateTime(__TIMESTAMP__)
    Uses Built in variable to set time 

    //Timestamp format Fri Mar 08 13:01:53 2024
-----------------------------------------------------------*/

void DS1307::setDateTime(String timestamp)
{
	uint8_t reg_hours, reg_seconds, h_mode, ch_bit;
	uint8_t day, month, hours, week, minutes, seconds, year;
	String month_str, week_str;
	bool meridiem;

	reg_hours   = _read_one_register(R_HOURS);
	reg_seconds = _read_one_register(R_SECONDS);

	h_mode = bitRead(reg_hours, 6); //Get hour mode
	ch_bit = bitRead(reg_seconds, CH); // Check the oscillator status to retain the status
	
	week_str = timestamp.substring(0,3);
	month_str = timestamp.substring(4,7);
	day = timestamp.substring(8,11).toInt();
	hours = timestamp.substring(11,13).toInt();
	minutes = timestamp.substring(14,16).toInt();
	seconds = timestamp.substring(17,19).toInt();
	year = timestamp.substring(22,24).toInt();

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


	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(R_SECONDS);

	seconds = bin2bcd(seconds);
	bitWrite(seconds,CH,ch_bit);
	Wire.write(seconds);
	
	Wire.write(bin2bcd(minutes));
	
	if (h_mode == CLOCK_H24)
	{
		Wire.write(bin2bcd(hours));
	}
	else if (h_mode == CLOCK_H12)
	{
		meridiem = (hours >= 12);
		if (hours == 0)
			hours = 12;
		if (hours > 12)
			hours -= 12;
		hours = bin2bcd(hours);
		bitWrite(hours, 5, meridiem);
		bitSet(hours, 6);
		Wire.write(hours);
	}
	Wire.write(week);
	Wire.write(bin2bcd(day));
	Wire.write(bin2bcd(month));
	Wire.write(bin2bcd(year));
	Wire.endTransmission();
}

tm DS1307::getDateTime()
{
	bool h_mode;
	uint8_t seconds, minutes, hours, week, day, month;
	uint16_t year;
	struct tm rtc_time;

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(R_SECONDS);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 7);

	seconds = Wire.read();
	bitClear(seconds, 7);
	rtc_time.tm_sec = bcd2bin(seconds);

	minutes = Wire.read();
	rtc_time.tm_min = bcd2bin(minutes);

	hours = Wire.read();
	h_mode = bitRead(hours, 6);
	if (h_mode == CLOCK_H24)
	{
		rtc_time.tm_hour = bcd2bin(hours);
	}
	else if (h_mode == CLOCK_H12)
	{
		bitClear(hours, 5);
		bitClear(hours, 6);
		rtc_time.tm_hour = bcd2bin(hours);
	}

	week = Wire.read();
	rtc_time.tm_wday = week - 1;

	day = Wire.read();
	rtc_time.tm_mday = bcd2bin(day);
	
	month = Wire.read();
 	rtc_time.tm_mon  = bcd2bin(month) - 1;

	year = Wire.read();
	rtc_time.tm_year = bcd2bin(year) + 100;
	
    return rtc_time;
}

/*-----------------------------------------------------------
setEpoch()

AVR is still under testing.
-----------------------------------------------------------*/

void DS1307::setEpoch(time_t epoch)
{
	struct tm epoch_tm, * ptr_epoch_tm;
	uint8_t reg_hours, reg_seconds, h_mode, ch_bit;
	uint8_t day, month, hours, week, minutes, seconds, year;
	bool meridiem;
	
	if(epoch >= UNIX_OFFSET)
	{
		reg_hours   = _read_one_register(R_HOURS);
		reg_seconds = _read_one_register(R_SECONDS);

		h_mode = bitRead(reg_hours, 6); //Get hour mode
		ch_bit = bitRead(reg_seconds, CH); // Check the oscillator status to retain the status

		#if defined(__AVR__)
			epoch = epoch - UNIX_OFFSET;		
		#endif

		ptr_epoch_tm = gmtime(&epoch);
		epoch_tm = *ptr_epoch_tm;

		// Serial.println(epoch);
		// Serial.println(epoch_tm.tm_sec);
		// Serial.println(epoch_tm.tm_min);
		// Serial.println(epoch_tm.tm_hour);
		// Serial.println(epoch_tm.tm_wday);
		// Serial.println(epoch_tm.tm_mday);
		// Serial.println(epoch_tm.tm_mon);
		// Serial.println(epoch_tm.tm_year);

		seconds = epoch_tm.tm_sec;
		minutes = epoch_tm.tm_min;
		hours = epoch_tm.tm_hour;
		week = epoch_tm.tm_wday + 1;
		day = epoch_tm.tm_mday;
		month = epoch_tm.tm_mon + 1;
		year = epoch_tm.tm_year % 100;

		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(R_SECONDS);

		seconds = bin2bcd(seconds);
		bitWrite(seconds,CH,ch_bit);
		Wire.write(seconds);
		
		Wire.write(bin2bcd(minutes));
		
		if (h_mode == CLOCK_H24)
		{
			Wire.write(bin2bcd(hours));
		}
		else if (h_mode == CLOCK_H12)
		{
			meridiem = (hours >= 12);
			if (hours == 0)
				hours = 12;
			if (hours > 12)
				hours -= 12;
			hours = bin2bcd(hours);
			bitWrite(hours, 5, meridiem);
			bitSet(hours, 6);
			Wire.write(hours);
		}
		
		Wire.write(week);
		Wire.write(bin2bcd(day));
		Wire.write(bin2bcd(month));
		Wire.write(bin2bcd(year));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getEpoch()
-----------------------------------------------------------*/
time_t DS1307::getEpoch()
{
	uint8_t seconds, minutes, hours, week, day, month, year, meridiem, h_mode;
	//uint16_t year;
	time_t epoch;
	struct tm epoch_tm;

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(R_SECONDS);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 7);

	seconds = Wire.read();
	bitClear(seconds, 7);
	epoch_tm.tm_sec = bcd2bin(seconds);

	minutes = Wire.read();
	epoch_tm.tm_min = bcd2bin(minutes);

	hours = Wire.read();
	h_mode = bitRead(hours, 6);
	meridiem = bitRead(hours, 5); 
	
	if (h_mode == CLOCK_H24)
	{
		epoch_tm.tm_hour = bcd2bin(hours);
	}
	else //h_mode == CLOCK_H12
	{	
		bitClear(hours, 5);
		bitClear(hours, 6);
		if(meridiem == HOUR_AM)
		{
			if(bcd2bin(hours) == 12)
				epoch_tm.tm_hour = 0;
			else
				epoch_tm.tm_hour = bcd2bin(hours);
		}
		else //if (meridiem == HOUR_PM)
		{	if(bcd2bin(hours) == 12)
				epoch_tm.tm_hour = 12;
			else
				epoch_tm.tm_hour = bcd2bin(hours) + 12;
		}
	}

	week = Wire.read();
	epoch_tm.tm_wday = week - 1;

	day = Wire.read();
	epoch_tm.tm_mday = bcd2bin(day);
	
	month = Wire.read();
 	epoch_tm.tm_mon  = bcd2bin(month) - 1;

	year = Wire.read();
	epoch_tm.tm_year = bcd2bin(year) + 100;

	// Serial.println(epoch_tm.tm_sec);
	// Serial.println(epoch_tm.tm_min);
	// Serial.println(epoch_tm.tm_hour);
	// Serial.println(epoch_tm.tm_wday);
	// Serial.println(epoch_tm.tm_mday);
	// Serial.println(epoch_tm.tm_mon);
	// Serial.println(epoch_tm.tm_year);
	
	epoch = mktime(&epoch_tm);

	#if defined(__AVR__)
		//Serial.println("Debug;");
		epoch = epoch + UNIX_OFFSET;
	#endif

	return (epoch);
}

/* SQW/OUT pin functions */

void DS1307::setOutPin(uint8_t mode)
{
	switch (mode)
	{
		case HIGH:
			_write_one_register(R_CONTROL,0b10000000);
			break;
		case LOW:
			_write_one_register(R_CONTROL,0b00000000);
			break;
		case SQW001Hz:
			_write_one_register(R_CONTROL,0b00010000);
			break;
		case SQW04kHz:
			_write_one_register(R_CONTROL,0b0010001);
			break;
		case SQW08kHz:
			_write_one_register(R_CONTROL,0b0010010);
			break;
		case SQW32kHz:
			_write_one_register(R_CONTROL,0b0010011);
			break;
	}
}

bool DS1307::isOutPinEnabled()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_CONTROL);
	reg_data = bitRead(reg_data, 7);
	return (reg_data);
}

bool DS1307::isSqweEnabled()
{
	int8_t reg_data;
	reg_data = _read_one_register(R_CONTROL);
	reg_data = bitRead(reg_data, 4);
	return (reg_data);
}

String DS1307::getDateTimeString()
{
	uint8_t seconds, minutes, hours, week, day, month, meridiem, h_mode;
	uint16_t year;
	String dateTimeString;

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(R_SECONDS);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 7);

	seconds = Wire.read();
	bitClear(seconds, 7);
	seconds = bcd2bin(seconds);

	minutes = Wire.read();
	minutes = bcd2bin(minutes);

	hours = Wire.read();
	h_mode = bitRead(hours, 6);
	meridiem = bitRead(hours, 5); 
	
	if (h_mode == CLOCK_H24)
	{
		hours = bcd2bin(hours);
	}
	else //h_mode == CLOCK_H12
	{
		bitClear(hours, 5);
		bitClear(hours, 6);
		hours = bcd2bin(hours);
	}

	week = Wire.read();
	week = week;

	day = Wire.read();
	day = bcd2bin(day);
	
	month = Wire.read();
 	month  = bcd2bin(month);

	year = Wire.read();
	year = bcd2bin(year) + 2000;

	switch (week)
	{
	case 1:
		dateTimeString.concat("SUN");
		break;
	case 2:
		dateTimeString.concat("MON");
		break;
	case 3:
		dateTimeString.concat("TUE");
		break;
	case 4:
		dateTimeString.concat("WED");
		break;
	case 5:
		dateTimeString.concat("THU");
		break;
	case 6:
		dateTimeString.concat("FRI");
		break;
	case 7:
		dateTimeString.concat("SAT");
		break;
	}
	
	dateTimeString.concat(" ");
	if(day<10)
		dateTimeString.concat("0");
	dateTimeString.concat(day);
	dateTimeString.concat("-");
	if(month<10)
		dateTimeString.concat("0");
	dateTimeString.concat(month);
	dateTimeString.concat("-");
	dateTimeString.concat(year);

	dateTimeString.concat(" ");

	if(hours<10)
		dateTimeString.concat("0");
	dateTimeString.concat(hours);
	dateTimeString.concat(":");
	if(minutes<10)
		dateTimeString.concat("0");
	dateTimeString.concat(minutes);
	dateTimeString.concat(":");
	if(seconds<10)
		dateTimeString.concat("0");
	dateTimeString.concat(seconds);
	if (h_mode == CLOCK_H12)
	{
		switch (meridiem)
		{
			case HOUR_AM :
			dateTimeString.concat(" AM");
			break;
			case HOUR_PM :
			dateTimeString.concat(" PM");
			break;
		}
	}
	return (dateTimeString);
}

String DS1307::getTimeString()
{
   	uint8_t seconds, minutes, hours, meridiem, h_mode;
	uint16_t year;
	String timeString;

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(R_SECONDS);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 3);

	seconds = Wire.read();
	bitClear(seconds, 3);
	seconds = bcd2bin(seconds);

	minutes = Wire.read();
	minutes = bcd2bin(minutes);

	hours = Wire.read();
	h_mode = bitRead(hours, 6);
	meridiem = bitRead(hours, 5); 
	
	if (h_mode == CLOCK_H24)
	{
		hours = bcd2bin(hours);
	}
	else //h_mode == CLOCK_H12
	{
		bitClear(hours, 5);
		bitClear(hours, 6);
		hours = bcd2bin(hours);
	}

	if(hours<10)
		timeString.concat("0");
	timeString.concat(hours);
	timeString.concat(":");
	if(minutes<10)
		timeString.concat("0");
	timeString.concat(minutes);
	timeString.concat(":");
	if(seconds<10)
		timeString.concat("0");
	timeString.concat(seconds);
	if (h_mode == CLOCK_H12)
	{
		switch (meridiem)
		{
			case HOUR_AM :
			timeString.concat(" AM");
			break;
			case HOUR_PM :
			timeString.concat(" PM");
			break;
		}
	}
	return (timeString);
}

String DS1307::getDateString()
{
    uint8_t week, day, month, meridiem, h_mode;
	uint16_t year;
	String dateString;

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(R_WEEKDAY);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 7);

	week = Wire.read();
	week = week;

	day = Wire.read();
	day = bcd2bin(day);
	
	month = Wire.read();
 	month  = bcd2bin(month);

	year = Wire.read();
	year = bcd2bin(year) + 2000;

	switch (week)
	{
	case 1:
		dateString.concat("SUN");
		break;
	case 2:
		dateString.concat("MON");
		break;
	case 3:
		dateString.concat("TUE");
		break;
	case 4:
		dateString.concat("WED");
		break;
	case 5:
		dateString.concat("THU");
		break;
	case 6:
		dateString.concat("FRI");
		break;
	case 7:
		dateString.concat("SAT");
		break;
	}
	
	dateString.concat(" ");
	if(day<10)
		dateString.concat("0");
	dateString.concat(day);
	dateString.concat("-");
	if(month<10)
		dateString.concat("0");
	dateString.concat(month);
	dateString.concat("-");
	dateString.concat(year);

	return (dateString);
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
	if(address >= 1 or address <=56 )
	{
		address = (address % length()) + 0x08;
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(address);
		Wire.endTransmission();
		Wire.requestFrom(DS1307_ADDR, 1);
		data = Wire.read();
	}
	return (data);
}
void NVRAM::write(uint8_t address, uint8_t data)
{
	if(address >= 1 or address <=56 )
	{
		address = address + 0x08;
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(address);
		Wire.write(data);
		Wire.endTransmission();
	}
}

/*Operator [] Read function*/
uint8_t NVRAM::operator[](uint8_t address) const
{
	uint8_t data = 0x00;
	if(address >= 1 or address <=56 )
	{
		//Serial.print("address = ");
		//Serial.println(address);
		address = (address % _length) + 0x08;
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(address);
		Wire.endTransmission();
		Wire.requestFrom(DS1307_ADDR, 1);
		data = Wire.read();
	}
	return (data);
}

/*Operator [] Write function
uint8_t& NVRAM::operator[](uint8_t address)
{
	//uint8_t data;
	if(address >= 1 or address <=56 )
	{
		address = address + 0x08;
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(address);
		Wire.write(data);
		Wire.endTransmission();
	}
}

*/

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

uint8_t NVRAM::length()
{
	return(_length);
}

/********************************************************
 * 
 * Private Functions
 *  
**************************************************************/

uint8_t DS1307::calculateDayOfWeek(uint8_t day, uint8_t month, uint16_t year)
{
	//Based on Zeller's Congruence algorithm 
	uint8_t week;
    if (month < 3)
	{
        month += 12;
        year -= 1;
    }
    int K = year % 100;
    int J = year / 100;
    int f = day + 13 * (month + 1) / 5 + K + K / 4 + J / 4 + 5 * J;
	week = f % 7;
	
	if(week == 0)
		return(7);
	else
    	return(week);
}

uint8_t DS1307::_read_one_register(uint8_t reg_address)
{
    uint8_t reg_data;

    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.endTransmission();

    Wire.requestFrom((int)_i2c_address,(int) 1);
    reg_data = Wire.read();
    return(reg_data);
}

void DS1307::_write_one_register(uint8_t reg_address, uint8_t reg_data)
{
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.write(reg_data);
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

//  -- END OF FILE