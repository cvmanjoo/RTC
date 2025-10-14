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
#define R_MINUTE_ALARM	0x0A
#define R_HOUR_ALARM	0x0B
#define R_DAY_ALARM		0x0C
#define R_WEEKDAY_ALARM	0x0D
#define R_OFFSET		0x0E
#define R_TMR_CLOCKOUT_CTRL	0x0F
#define R_TMR_A_FREQ_CTRL	0x10
#define R_TMR_A_REG			0x11
#define R_TMR_B_FREQ_CTRL	0x12
#define R_TMR_B_REG			0x13

uint8_t PCF8523::begin()
{
	Wire.begin(); // join i2c bus
	return(PCF8523_ADDR);
}

bool PCF8523::isConnected()
{
	Wire.begin();
	Wire.beginTransmission(PCF8523_ADDR);
	return(Wire.endTransmission() == 0 ? true : false);
}

bool PCF8523::isRunning(void)
{
    bool stop_bit, os_bit;
	
	stop_bit = bitRead(_read_one_register(R_CONTROL_1),5);
	os_bit = bitRead(_read_one_register(R_SECONDS),7);

	if(os_bit == 0 && stop_bit == 0) // Oscillator Stop Flag is set
		return true; //Optimise this in future
	else
		return false;

	// // 0 means Running 
	// // 1 means Stopped
    // return (!stop_bit); 
}

void PCF8523::startClock(void)
{
    uint8_t reg_data;

	reg_data = _read_one_register(R_SECONDS);
	bitClear(reg_data,7);	// Clear OS Bit to 0
	_write_one_register(R_SECONDS,reg_data);

}

void PCF8523::stopClock(void)
{
    uint8_t reg_data;

	reg_data = _read_one_register(R_SECONDS);
	bitSet(reg_data,7);	// Set OS Bit to 1
	_write_one_register(R_SECONDS,reg_data);
}

void PCF8523::setHourMode(uint8_t h_mode)
{
	uint8_t reg_data;
	if(h_mode == CLOCK_H12 || h_mode == CLOCK_H24)
	{
		reg_data = _read_one_register(R_CONTROL_1);
		bitWrite(reg_data, 3, h_mode);
		_write_one_register(R_CONTROL_1, reg_data);
	}
}

uint8_t PCF8523::getHourMode()
{
	uint8_t reg_data;
	bool h_mode;

	reg_data = _read_one_register(R_CONTROL_1);
	h_mode = bitRead(reg_data, 3);
	return (h_mode);
}

void PCF8523::setMeridiem(uint8_t meridiem)
{
	uint8_t ctrl_1_data, hour_reg_data;
	bool h_mode;

	ctrl_1_data = _read_one_register(R_CONTROL_1);
	h_mode = bitRead(ctrl_1_data, 3);

	hour_reg_data = _read_one_register(R_HOURS);

	if(h_mode == CLOCK_H12
		&& (meridiem == HOUR_AM || meridiem == HOUR_PM))
	{
		bitWrite(hour_reg_data, 5, meridiem);
		_write_one_register(R_HOURS, hour_reg_data);
	}
	// If 24 Hour Mode, do nothing.
}

uint8_t PCF8523::getMeridiem()
{
	uint8_t ctrl_1_data, hour_reg_data;
	bool h_mode, meridiem;

	ctrl_1_data = _read_one_register(R_CONTROL_1);
	h_mode = bitRead(ctrl_1_data, 3);

	hour_reg_data = _read_one_register(R_HOURS);
	meridiem = bitRead(hour_reg_data, 5);

	if (h_mode == CLOCK_H12)
		return (meridiem);
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
    uint8_t minutes = _read_one_register(R_MINUTES);
    return (bcd2bin(minutes));
}

void PCF8523::setMinutes(uint8_t minutes)
{
	if(minutes >= 00 && minutes <= 59)
		_write_one_register(R_MINUTES,bin2bcd(minutes));
}

/*-----------------------------------------------------------
getHours
-----------------------------------------------------------*/
uint8_t PCF8523::getHours()
{
	uint8_t ctrl_1_data, hour_reg_data;
	bool h_mode, meridiem;

	ctrl_1_data = _read_one_register(R_CONTROL_1);
	h_mode = bitRead(ctrl_1_data, 3);

	hour_reg_data = _read_one_register(R_HOURS);

	if (h_mode == CLOCK_H24)
	{
		//bitClear(hour_reg_data, 5);
		return (bcd2bin(hour_reg_data));
	}
	else //if (h_mode == CLOCK_H12)
	{
		bitClear(hour_reg_data, 5);
		return (bcd2bin(hour_reg_data));
	}
}

void  PCF8523::setHours(uint8_t hours)
{
	uint8_t ctrl_1_data, hour_reg_data;
	bool h_mode, meridiem;

	if (hours >= 00 && hours <= 23)
	{
		ctrl_1_data = _read_one_register(R_CONTROL_1);
		h_mode = bitRead(ctrl_1_data, 3);

		if (h_mode == CLOCK_H24)
		{
			_write_one_register(R_HOURS, bin2bcd(hours));

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
			_write_one_register(R_HOURS, hours);
		}
	}
}

/*-----------------------------------------------------------
getWeek
-----------------------------------------------------------*/
uint8_t PCF8523::getWeek()
{
	uint8_t week = _read_one_register(R_WEEKDAYS);
	return week+1;
}

void PCF8523::setWeek(uint8_t week)
{
	if (week >= 1 && week <= 7)
		_write_one_register(R_WEEKDAYS,week-1);
}

void PCF8523::updateWeek()
{
	// Calculate Day of week based on date, month and year
	// and update the weekday register

	uint8_t day, month, week_day, century_bit;
	uint16_t century, year;

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_DAYS);  // Day Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR, 4);

	day = bcd2bin(Wire.read()); // Read Day Register
	week_day = Wire.read(); // Read Weekday Register (Not used)
	month = bcd2bin(Wire.read()); // Read Month Register
	year = bcd2bin(Wire.read()); + 2000; // Read Year Register and Converting to 4 Digit Year
	
	week_day = _calculateDayOfWeek(day, month, year) - 1; // Adjusting to 0-6 for PCF8523
	_write_one_register(R_WEEKDAYS,week_day);
}



/*-----------------------------------------------------------
getDay
-----------------------------------------------------------*/
uint8_t PCF8523::getDay()
{
    uint8_t day = _read_one_register(R_DAYS);
    return (bcd2bin(day));

	//return(bcd2bin(_read_one_register(R_DAYS)));
}

void PCF8523::setDay(uint8_t day)
{
	if(day >= 1 && day <= 31)
		_write_one_register(R_DAYS,bin2bcd(day));
}

/*-----------------------------------------------------------
getMonth()
-----------------------------------------------------------*/
uint8_t PCF8523::getMonth()
{
	uint8_t month = _read_one_register(R_MONTHS);	
	return (bcd2bin(month));
}
/*-----------------------------------------------------------
setMonth() 
-----------------------------------------------------------*/

void PCF8523::setMonth(uint8_t month)
{
	if (month >= 1 && month <= 12)
		_write_one_register(R_MONTHS,bin2bcd(month));
}

/*-----------------------------------------------------------
getYear (Completed)
-----------------------------------------------------------*/
uint16_t PCF8523::getYear()
{
	uint16_t year = _read_one_register(R_YEARS);
	return(bcd2bin(year) + 2000);
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
		_write_one_register(R_YEARS,bin2bcd(year));	// Write 2 Digit year to Year Register
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
setDate

-----------------------------------------------------------*/
void PCF8523::setDate(uint8_t day, uint8_t month, uint16_t year)
{
	uint8_t year_4digit;
	year_4digit = year; 

	if(day >=1 && day <=31 && month >=1 && month <=12)
	{
		if((year >= 00 && year <= 99) || (year >= 2000 && year <= 2099))
			year = year % 2000; //Converting to 2 Digit
		
		Wire.beginTransmission(PCF8523_ADDR);
		Wire.write(R_DAYS);  // Day Register
		Wire.write(bin2bcd(day));	//0x05 Day
		Wire.write(_calculateDayOfWeek(day, month, year_4digit)-1); //0x06 Weekday
		Wire.write(bin2bcd(month));	//0x07 Months
		Wire.write(bin2bcd(year));	//0x08 Years
		Wire.endTransmission();

	}
}

/*-----------------------------------------------------------
setDateTime(__DATE__,__TIME__)

// Note: This function assumes that the input date and time strings are always valid
// and in the correct format as provided by the compiler macros __DATE__ and __TIME__
// It does not perform any error checking or validation on the input strings.
// Example input: __DATE__ = "Dec 26 2009", __TIME__= "12:34:56"
// The function extracts the day, month, year, hour, minute, and second from
// the input strings and sets the RTC accordingly.

-----------------------------------------------------------*/

void PCF8523::setDateTime(String date, String time)
{

	uint8_t ctrl_1_data, reg_seconds, h_mode, vl_bit;
	uint8_t day, month, hours, minutes, seconds, year;
	bool meridiem;

	ctrl_1_data = _read_one_register(R_CONTROL_1);
	h_mode = bitRead(ctrl_1_data, 3);

	reg_seconds = _read_one_register(R_SECONDS);
	vl_bit = bitRead(reg_seconds, 7); //Get VL Bit

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

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_SECONDS);  // Seconds Register
	Wire.write(bin2bcd(seconds) | (vl_bit << 7));	//0x02 Seconds
	Wire.write(bin2bcd(minutes));	//0x03 Minutes
	if (h_mode == CLOCK_H24)
	{
		Wire.write(bin2bcd(hours));		//0x04 Hours
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
		Wire.write(hours);		//0x04 Hours
	}
	Wire.write(bin2bcd(day));	//0x05 Day
	Wire.write(_calculateDayOfWeek(day, month, year)-1); //0x06 Weekday
	Wire.write(bin2bcd(month));	//0x07 Months
	Wire.write(bin2bcd(year));	//0x08 Years
	Wire.endTransmission();
}

/*-----------------------------------------------------------
	setDateTime(__TIMESTAMP__)
    Uses Built in variable to set time 

    //Timestamp format Fri Mar 08 13:01:53 2024
-----------------------------------------------------------*/

void PCF8523::setDateTime(String timestamp)
{
	uint8_t ctrl_1_data, reg_seconds, h_mode, vl_bit;
	uint8_t day, month, hours, week, minutes, seconds, year;
	String week_str, month_str;
	bool meridiem;

	ctrl_1_data = _read_one_register(R_CONTROL_1);
	h_mode = bitRead(ctrl_1_data, 3);

	reg_seconds = _read_one_register(R_SECONDS);
	vl_bit = bitRead(reg_seconds, 7); //Get VL Bit

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


	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_SECONDS);  // Seconds Register
	Wire.write(bin2bcd(seconds) | (vl_bit << 7));	//0x02 Seconds
	Wire.write(bin2bcd(minutes));	//0x03 Minutes
	if (h_mode == CLOCK_H24)
	{
		Wire.write(bin2bcd(hours));		//0x04 Hours
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
		Wire.write(hours);		//0x04 Hours
	}
	Wire.write(bin2bcd(day));	//0x05 Day
	Wire.write(week-1); //0x06 Weekday
	Wire.write(bin2bcd(month));	//0x07 Months
	Wire.write(bin2bcd(year));	//0x08 Years
	Wire.endTransmission();
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

String PCF8523::getTimeString()
{
	uint8_t seconds, minutes, hours;
	uint8_t h_mode, meridiem;
	String timeString;

	h_mode = bitRead(_read_one_register(R_CONTROL_1), 3);

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_SECONDS);  // Seconds Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR, 3);

	seconds = Wire.read();
	bitClear(seconds, 7);
	seconds = bcd2bin(seconds);

	minutes = Wire.read();
	minutes = bcd2bin(minutes);

	hours = Wire.read();

	// If 12 Hour Mode, clear the 5th bit (AM/PM)
	if (h_mode == CLOCK_H12)
		bitClear(hours, 5);

	hours = bcd2bin(hours);

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
		meridiem = bitRead(_read_one_register(R_HOURS), 5);
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

String PCF8523::getDateString()
{
    uint8_t day, week, month, century_bit;
	uint16_t century, year;
	String dateString;

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_DAYS);  // Day Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR, 4);

	day = Wire.read();			//0x05 Day
	day = bcd2bin(day);

	week = Wire.read();			//0x06 Weekday
	
	month = Wire.read();		//0x07 Months
	month = bcd2bin(month);

	year = Wire.read();			//0x08 Years
	year = bcd2bin(year);
	year = year + 2000;

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

String PCF8523::getWeekString()
{
	uint8_t week;
	String weekString;

	week = _read_one_register(R_WEEKDAYS);
	
	switch (week+1)
	{
		case 1:
			weekString.concat("Sunday");
			break;
		case 2:
			weekString.concat("Monday");
			break;
		case 3:
			weekString.concat("Tuesday");
			break;
		case 4:
			weekString.concat("Wednesday");
			break;
		case 5:
			weekString.concat("Thursday");
			break;
		case 6:
			weekString.concat("Friday");
			break;
		case 7:
			weekString.concat("Saturday");
			break;
	}
	return (weekString);
}

/*-----------------------------------------------------------
getDateTimeString()
	* Returns Date and Time in "YYYY-MM-DD HH:MM:SS" format

-----------------------------------------------------------*/
String PCF8523::getDateTimeString()
{
	String dateTimeString;
	dateTimeString = getWeekString().substring(0,3) + " " +  getDateString() + " " + getTimeString();
	return (dateTimeString);
}



/*-----------------------------------------------------------
getDateTime()
	* Returns a tm structure with the current date and time
-----------------------------------------------------------*/
// tm structure reference: https://www.cplusplus.com/reference/ctime/tm/
// struct tm {
//    int tm_sec;   // seconds after the minute - [0, 60] including leap second
//    int tm_min;   // minutes after the hour - [0, 59]
//    int tm_hour;  // hours since midnight - [0, 23]
//    int tm_mday;  // day of the month - [1, 31
//    int tm_mon;   // months since January - [0, 11]
//    int tm_year;  // years since 1900
//    int tm_wday;  // days since Sunday - [0, 6]
//    int tm_yday;  // days since January 1 - [0, 365]
//    int tm_isdst; // daylight savings time flag
//	

// Sets the RTC with the date and time provided in the tm structure
// Note: tm_year is years since 1900, so we need to add 1900 to get the full year

void PCF8523:: setDateTime(tm datetime)
{
	uint8_t ctrl_1_data, reg_seconds, h_mode, vl_bit;
	uint8_t day, month, hours, week, minutes, seconds, year;
	String week_str, month_str;
	bool meridiem;

	mktime(&datetime); // Normalize the tm structure	

	ctrl_1_data = _read_one_register(R_CONTROL_1);
	h_mode = bitRead(ctrl_1_data, 3);

	reg_seconds = _read_one_register(R_SECONDS);
	vl_bit = bitRead(reg_seconds, 7); //Get VL Bit

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_SECONDS);  // Seconds Register
	Wire.write(bin2bcd(datetime.tm_sec) | (vl_bit << 7));	//0x03 Seconds
	Wire.write(bin2bcd(datetime.tm_min));	//0x04 Minutes
	if(h_mode == CLOCK_H24)
	{
		Wire.write(bin2bcd(datetime.tm_hour));		//0x05 Hours
	}
	else if(h_mode == CLOCK_H12)
	{
		meridiem = (datetime.tm_hour >= 12);
		if(datetime.tm_hour == 0)
			datetime.tm_hour = 12;
		if(datetime.tm_hour > 12)
			datetime.tm_hour -= 12;
		hours = bin2bcd(datetime.tm_hour);
		bitWrite(hours, 5, meridiem);
		Wire.write(hours);		//0x05 Hours
	}
	Wire.write(bin2bcd(datetime.tm_mday));	//0x06 Day
	Wire.write(datetime.tm_wday); //0x07 Weekday
	Wire.write(bin2bcd(datetime.tm_mon + 1));	//0x08 Months
	Wire.write(bin2bcd(datetime.tm_year % 100));	//0x09 Years
	Wire.endTransmission();

}

// Returns a tm structure with the current date and time

tm PCF8523::getDateTime()
{
	uint8_t h_mode, meridiem;
	tm datetime;

	h_mode = bitRead(_read_one_register(R_CONTROL_1), 3);

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_SECONDS);  // Seconds Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR, 7);

	datetime.tm_sec = bcd2bin(Wire.read() & 0x7F); 	//0x02 Seconds Clearing OS Bit if Set.
	
	datetime.tm_min = bcd2bin(Wire.read());			//0x03 Minutes

	datetime.tm_hour = Wire.read(); // 0x04 Hours Clearing 12/24 and AM/PM Bits if Set.

	if(h_mode == CLOCK_H24)
		datetime.tm_hour = bcd2bin(datetime.tm_hour);
	else
	{
		meridiem = bitRead(datetime.tm_hour, 5); // AM/PM Bit
		datetime.tm_hour = bcd2bin(datetime.tm_hour & 0x1F);
		if (meridiem == HOUR_PM && datetime.tm_hour != 12)
			datetime.tm_hour += 12;
		if (meridiem == HOUR_AM && datetime.tm_hour == 12)
			datetime.tm_hour = 0;
	}
	
	datetime.tm_mday = bcd2bin(Wire.read());		//0x05 Day
	datetime.tm_wday = Wire.read();					//0x06 Weekday
	datetime.tm_mon = bcd2bin(Wire.read()) - 1;		//0x07 Months
	datetime.tm_year = bcd2bin(Wire.read())+100;	//0x08 Years since 1900
	datetime.tm_isdst = -1; 	// Not considering Daylight Saving Time
	
	return (datetime);
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

// Reset the RTC by writing 0x58 to Control_1 Register

void PCF8523::softReset()
{
	_write_one_register(R_CONTROL_1,0x58);
}

bool PCF8523::isBatteryLow()
{
   	_read_one_register(R_CONTROL_3);
	return (bitRead(_read_one_register(R_CONTROL_3), 2)); // BLF Bit
}

void PCF8523::enableAlarm()
{
	uint8_t alarm_registers[4];

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_MINUTE_ALARM);
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR,4);
	for(int i=0;i<4;i++)
	{
		alarm_registers[i] = Wire.read();
		bitClear(alarm_registers[i],7); // Disable Minute Alarm
	}
	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_MINUTE_ALARM);  // Control_2 Register
	for(int i=0;i<4;i++)
		Wire.write(alarm_registers[i]);
	Wire.endTransmission();
}

void PCF8523::disableAlarm()
{
	uint8_t alarm_registers[4];

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_MINUTE_ALARM);
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR,4);
	for(int i=0;i<4;i++)
	{
		alarm_registers[i] = Wire.read();
		bitSet(alarm_registers[i],7); // Disable Minute Alarm
	}

	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_MINUTE_ALARM);  // Control_2 Register
	for(int i=0;i<4;i++)
		Wire.write(alarm_registers[i]);
	Wire.endTransmission();
}

void PCF8523::clearAlarm()
{
	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_MINUTE_ALARM);  // Control_2 Register
	Wire.write(0x80); // Disable Minute Alarm
	Wire.write(0x80); // Disable Hour Alarm	
	Wire.write(0x80); // Disable Day Alarm
	Wire.write(0x80); // Disable Weekday Alarm
	Wire.endTransmission();
}

bool PCF8523::isAlarmEnabled()
{
	uint8_t alarm_registers[4];
	Wire.beginTransmission(PCF8523_ADDR);
	Wire.write(R_MINUTE_ALARM);  // 0x0A Alarm Minutes Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8523_ADDR,4);
	for(int i=0;i<4;i++)
		alarm_registers[i] = Wire.read();
	return (bitRead(alarm_registers[0],7) == 0
		|| bitRead(alarm_registers[1],7) == 0 
		|| bitRead(alarm_registers[2],7) == 0 
		|| bitRead(alarm_registers[3],7) == 0);
}

bool PCF8523::isAlarmTriggered()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_CONTROL_2);
	return (bitRead(reg_data, 3)); // AF Bit
}

void PCF8523::setAlarm(uint8_t minutes,uint8_t hours, uint8_t days, uint8_t weekday)
{
	if(minutes >= 0 && minutes <= 59)
	{
		_write_one_register(R_MINUTE_ALARM,bin2bcd(minutes));
	}

	if(hours >= 0 && hours <= 23)
	{
		_write_one_register(R_HOUR_ALARM,bin2bcd(hours));

	}
	if(days >= 1 && days <= 31)
	{
		_write_one_register(R_DAY_ALARM,bin2bcd(days));
	}
	if(weekday >= 1 && weekday <= 7)
	{
		weekday = weekday - 1; // Adjusting to 0-6 for PCF8523
		_write_one_register(R_WEEKDAY_ALARM,weekday);
	}
}

//Timer functions


void PCF8523::enableTimerA()
{
	
	uint8_t reg_data;
	reg_data = _read_one_register(R_TMR_CLOCKOUT_CTRL);

	//Write 01 to TMR_A_CTRL to enable Timer A in Countdown timed out mode
	bitClear(reg_data, 0);
	bitSet(reg_data, 1);

	// Write back to Register
	_write_one_register(R_TMR_CLOCKOUT_CTRL,reg_data); 

	// Set Timer A Frequency to 1Hz
	_write_one_register(R_TMR_A_FREQ_CTRL,0b10); 

}

void PCF8523::disableTimerA()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_CONTROL_2);
	bitClear(reg_data, 2); // Disable Timer
	_write_one_register(R_CONTROL_2,reg_data);
}

void PCF8523::clearTimerA()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_CONTROL_2);
	bitClear(reg_data, 1); // Clear Timer Flag
	_write_one_register(R_CONTROL_2,reg_data);
}

bool PCF8523::isTimerAEnabled()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_TMR_CLOCKOUT_CTRL);

	reg_data = reg_data & 0x06; // Masking other bits
	reg_data = reg_data >> 1; // Shifting to get Timer A bits in LSB
	if(reg_data == 0b00 || reg_data == 0b11)
		return false; // Timer A Disabled
	else
		return true; // Timer A Enabled
}

bool PCF8523::isTimerATriggered()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_CONTROL_2);
	return (bitRead(reg_data, 6)); // TF Bit
}

void PCF8523::setTimerA(uint8_t t_seconds)
{
	if(t_seconds >= 1 && t_seconds <= 255)
	{
		_write_one_register(R_TMR_A_REG, t_seconds); // Set Timer A value
	}
}

uint8_t PCF8523::getTimerA()
{
	return (_read_one_register(R_TMR_A_REG));
}



// void PCF8523::setSquareWave(uint8_t rate)
// {
// 	uint8_t reg_data;
// 	reg_data = _read_one_register(R_CONTROL_2);
	
// 	if(rate == SQWAVE_OFF || rate == SQWAVE_1HZ || rate == SQWAVE_4096HZ
// 		|| rate == SQWAVE_8192HZ || rate == SQWAVE_32768HZ)
// 	{
// 		bitWrite(reg_data, 4, (rate >> 1) & 0x01); // RS1
// 		bitWrite(reg_data, 5, (rate >> 0) & 0x01); // RS0
// 		if(rate == SQWAVE_OFF)
// 			bitClear(reg_data, 3); // Disable Square Wave Output
// 		else
// 			bitSet(reg_data, 3); // Enable Square Wave Output
		
// 		_write_one_register(R_CONTROL_2,reg_data);
// 	}
// }



/**************************************************************
 * 
 * Private Functions
 *  
**************************************************************/


uint8_t PCF8523::_read_one_register(uint8_t reg_address)
{
    uint8_t reg_data;
    
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.endTransmission();

    Wire.requestFrom((int)_i2c_address,(int) 1);
    reg_data = Wire.read();
    return(reg_data);

}

void PCF8523::_write_one_register(uint8_t reg_address, uint8_t reg_data)
{
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.write(reg_data);
    Wire.endTransmission();
}

uint8_t PCF8523::bcd2bin(uint8_t val)
{
	return val - 6 * (val >> 4);
}

uint8_t PCF8523::bin2bcd(uint8_t val)
{
	return val + 6 * (val / 10);
}

uint8_t PCF8523::_calculateDayOfWeek(uint8_t day, uint8_t month, uint16_t year)
{
	// Zeller's Congruence (Gregorian calendar)
	// Works for any date after 01 Mar 2000.
	// Adjust months and years for Zeller's Congruence
	if (month < 3) {
		month += 12;
		year -= 1;
	}

	int K = year % 100;      // Year within century
	int J = year / 100;      // Zero-based century

	// Zeller's formula
	int h = (day + 13*(month + 1)/5 + K + K/4 + J/4 + 5*J) % 7;

	// Convert Zeller's output to 1=Sunday ... 7=Saturday
	int weekday = ((h + 6) % 7) + 1;

	return weekday;
}

// #669