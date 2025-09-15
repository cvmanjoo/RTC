/*
* PCF8563.cpp - Library to set & get time from RTC PCF8563
* Created by Manjunath CV. March 23, 2019, 03:02 AM
* Released into the public domain.
* 
*	* Implement Soft 12 Hour Clock.
*	
*/

#include <time.h>
#include <Arduino.h>
#include <Wire.h>
#include <I2C_RTC.h>

#define R_CONTROL_STATUS_1 	0x00
#define R_CONTROL_STATUS_2 	0x01
#define R_VL_SECONDS		0x02
#define R_MINUTES			0x03
#define R_HOURS 			0x04
#define R_DAYS   			0x05
#define R_WEEKDAYS 			0x06
#define R_CENTURY_MONTHS 	0X07
#define R_YEARS			 	0x08
#define R_MINUTES_ALARM     0x09
#define R_HOURS_ALARM     	0x0A
#define R_DAY_ALARM       	0x0B
#define R_WEEKDAY_ALARM   	0x0C
#define R_CLKOUT_CONTROL   	0x0D
#define R_TIMER_CONTROL    	0x0E
#define R_TIMER           	0x0F

#define VL 7

uint8_t PCF8563::begin()
{
    Wire.begin(); // join i2c bus
    return(PCF8563_ADDR);
}

bool PCF8563::isConnected()
{
	Wire.begin();
	Wire.beginTransmission(PCF8563_ADDR);
	return(Wire.endTransmission() == 0 ? true : false);
}


bool PCF8563::isRunning(void)
{
	uint8_t reg_00, reg_02; 
	bool flag;

	reg_00 = _read_one_register(R_CONTROL_STATUS_1);
	reg_02 = _read_one_register(R_VL_SECONDS);

	reg_00 = bitRead(reg_00, 5); // Read STOP Bit to check RTC source clock runs
	reg_02 = bitRead(reg_02, 7); // Read VL Bit to check Clock Integrity 

	// Recheck the return value
	return (!(reg_00 | reg_02));
}

void PCF8563::startClock(void)
{
    uint8_t reg_00, reg_02;

	reg_00 = _read_one_register(R_CONTROL_STATUS_1);
	reg_02 = _read_one_register(R_VL_SECONDS);

	bitClear(reg_00, 5); // STOP Bit
	bitClear(reg_00, 3); // TESTC Bit
	bitClear(reg_02, VL); // VL Bit

	// In some cases, just clearing the STOP bit may not start the RTC
	// So rewriting the seconds register after clearing VL and CH bits
	// to ensure the clock starts

	reg_02 = bcd2bin(reg_02);
	if (reg_02 >= 0 && reg_02 <= 59)
		reg_02 = bin2bcd(reg_02);
	else
		reg_02 = 0x00;

	_write_one_register(R_CONTROL_STATUS_1,reg_00);
	_write_one_register(R_VL_SECONDS,reg_02);
}

void PCF8563::stopClock(void)
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_CONTROL_STATUS_1);
	bitSet(reg_data, 5); // STOP Bit
	_write_one_register(R_CONTROL_STATUS_1,reg_data);
}

void PCF8563::setHourMode(uint8_t h_mode)
{
	/* 
		PCF8563 supports only 24 Hour Mode.
		This function is for API compatibility.
		and avoid errors while compiling.
	*/
}

uint8_t PCF8563::getHourMode()
{
	return (CLOCK_H24);
}

void PCF8563::setMeridiem(uint8_t meridiem)
{
	/* 
		PCF8563 supports only 24 Hour Mode.
		This function is for API compatibility.
		and avoid errors while compiling.
	*/
}

uint8_t PCF8563::getMeridiem()
{
	return (HOUR_24);
}

/*-----------------------------------------------------------
get & set Second
-----------------------------------------------------------*/
uint8_t PCF8563::getSeconds()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_VL_SECONDS);
	bitClear(reg_data, VL); // Clearing CH Bit if Set.
	return (bcd2bin(reg_data));
}

void PCF8563::setSeconds(uint8_t seconds)
{
	uint8_t reg_data, vl_bit;
	if (seconds >= 00 && seconds <= 59)
	{
		reg_data = _read_one_register(R_VL_SECONDS);
		vl_bit = bitRead(reg_data, VL);
		seconds = bin2bcd(seconds);
		bitWrite(seconds,VL,vl_bit);
		_write_one_register(R_VL_SECONDS,seconds);
	}
}

/*-----------------------------------------------------------
getMinutes
-----------------------------------------------------------*/
uint8_t PCF8563::getMinutes()
{
	uint8_t minutes;
	minutes = _read_one_register(R_MINUTES);
	return(bcd2bin(minutes));
}

void PCF8563::setMinutes(uint8_t minutes)
{
	if(minutes >= 00 && minutes <= 59)
		_write_one_register(R_MINUTES,bin2bcd(minutes));
}

/*-----------------------------------------------------------
getHours
-----------------------------------------------------------*/
uint8_t PCF8563::getHours()
{
	uint8_t hours;
	hours = _read_one_register(R_HOURS);
	return(bcd2bin(hours));
}

void  PCF8563::setHours(uint8_t hours)
{
	if (hours >= 00 && hours <= 23)
		_write_one_register(R_HOURS,bin2bcd(hours));
}

/*-----------------------------------------------------------
getWeek
-----------------------------------------------------------*/
uint8_t PCF8563::getWeek()
{
	uint8_t week = _read_one_register(R_WEEKDAYS);
	return week+1;
}

void PCF8563::setWeek(uint8_t week)
{	
	if (week >= 1 && week <= 7)
		_write_one_register(R_WEEKDAYS,week-1);
}

void PCF8563::updateWeek()
{
	// Calculate Day of week based on date, month and year
	// and update the weekday register

	uint8_t day, month, week_day, century_bit;
	uint16_t century, year;

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(R_DAYS);  // Read Day, Weekday, Month and Year registers
	Wire.endTransmission();
	Wire.requestFrom(PCF8563_ADDR,4);
	
	day =  bcd2bin(Wire.read());
	week_day = Wire.read(); // Skip Weekday
	month = Wire.read();
	century_bit = bitRead(month, 7); // Read Century Bit
	month = bcd2bin(bitClear(month, 7));
	(century_bit == 1) ? century = 1900 : century = 2000;
	year = Wire.read();	
	year = bcd2bin(year) + century; // Converting to 4 Digit Year

	week_day = _calculateDayOfWeek(day, month, year) - 1; // Adjusting to 0-6 for PCF8563
	_write_one_register(R_WEEKDAYS,week_day);
}


/*-----------------------------------------------------------
getDay
-----------------------------------------------------------*/
uint8_t PCF8563::getDay()
{
    uint8_t day;
	day = _read_one_register(R_DAYS);	
	return (bcd2bin(day));
}

void PCF8563::setDay(uint8_t day)
{
	if (day >= 1 && day <= 31)
		_write_one_register(R_DAYS,bin2bcd(day));
}

/*-----------------------------------------------------------
getMonth()
-----------------------------------------------------------*/
uint8_t PCF8563::getMonth()
{
	uint8_t month;
	month = _read_one_register(R_CENTURY_MONTHS);
	bitClear(month,7);		//Clear Century;
	return (bcd2bin(month));
}
/*-----------------------------------------------------------
setMonth() 
-----------------------------------------------------------*/

void PCF8563::setMonth(uint8_t month)
{
	uint8_t data, century_bit;
	if (month >= 1 && month <= 12)
	{
		data = _read_one_register(R_CENTURY_MONTHS);
		century_bit = bitRead(data, 7); // Read Century Bit
		month = bin2bcd(month);
		bitWrite(month,7,century_bit);
		_write_one_register(R_CENTURY_MONTHS,month);
	}
}

/*-----------------------------------------------------------
getYear (Completed)
	* Always return 4 Digit year 
	* Takes Care of Century.
	* Century Bit
		1 = 1900s
		0 = 2000s
-----------------------------------------------------------*/
uint16_t PCF8563::getYear()
{
	uint8_t months_reg, years_reg, century_bit;
	uint16_t century,years;

	// Read Century bit from Month Register(0x07)
	//months_reg = _read_one_register(R_CENTURY_MONTHS);

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(R_CENTURY_MONTHS);  // Read Month register for Century
	Wire.endTransmission();
	Wire.requestFrom(PCF8563_ADDR,2);
	months_reg =  Wire.read();
	years_reg = Wire.read();

	years = bcd2bin(years_reg);
	century_bit = bitRead(months_reg, 7);

	if(century_bit == 0)
		return (years + 2000);
	else
		return (years + 1900);
}

/*-----------------------------------------------------------
setYear (Completed)
	* Takes Care of Century.
	* Accepts both 2 and 4 Digit Years.
-----------------------------------------------------------*/
void PCF8563::setYear(uint16_t year)
{
	uint8_t reg_month,century_bit;
	
	if((year >= 00 && year <= 99) || (year >= 1900 && year <= 2099))
	{
		// If year is 2 digits set to 2000s.
		if(year >= 00 && year <= 99)
			year = year + 2000;

		//Century Calculation
		if(year >= 1900 && year <= 1999)
			century_bit = 1;
		else if (year >= 2000 && year <= 2099)
			century_bit = 0;

		// Find Century 
		year = year % 100;		//Converting to 2 Digit

		// Read Century bit from Month Register(0x05)
		reg_month = _read_one_register(R_CENTURY_MONTHS);
		if(century_bit == 1)
			bitSet(reg_month,7);
		else
			bitClear(reg_month,7);
		
		_write_one_register(R_CENTURY_MONTHS,reg_month);
		_write_one_register(R_YEARS,bin2bcd(year));
	}
}

/*-----------------------------------------------------------
setTime (Should be Optimised)
-----------------------------------------------------------*/

void PCF8563::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	uint8_t reg_seconds, vl_bit;

	if (hours >= 00 && hours <= 23 && minutes >= 00 && minutes <= 59 && seconds >= 00 && seconds <= 59)
	{
		reg_seconds = _read_one_register(R_VL_SECONDS);
		vl_bit = bitRead(reg_seconds, VL); // Check the oscillator status to retain the status

		Wire.beginTransmission(PCF8563_ADDR);
		Wire.write(R_VL_SECONDS); // Set register pointer to Seconds
		seconds = bin2bcd(seconds);
		bitWrite(seconds,VL,vl_bit);
		Wire.write(seconds);
		Wire.write(bin2bcd(minutes));
		Wire.write(bin2bcd(hours));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
setDate
-----------------------------------------------------------*/
void PCF8563::setDate(uint8_t day, uint8_t month, uint16_t year)
{
	uint8_t month_full, year_full, century_bit;

	if(day >=1 && day <=31 && month >=1 && month <=12)
	{
		// If year is 2 digits set to 2000s.
		if(year >= 00 && year <= 99)
			year = year + 2000;

		//Century Calculation
		if(year >= 1900 && year <= 1999)
			century_bit = 1;
		else if (year >= 2000 && year <= 2099)
			century_bit = 0;

		// Find Century 
		year_full = year;
		year = year % 100;		//Converting to 2 Digit

		month_full = month;

		Wire.beginTransmission(PCF8563_ADDR);
		Wire.write(R_DAYS);

		Wire.write(bin2bcd(day));
		Wire.write(_calculateDayOfWeek(day, month_full, year_full)-1); // Adjusting to 0-6 for PCF8563

		month = bin2bcd(month);
		bitWrite(month,7,century_bit);
		Wire.write(month);

		Wire.write(bin2bcd(year));
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
setDateTime(__DATE__,__TIME__)

// sample input

__DATE__ = "Dec 26 2009"
__TIME__ = "12:34:56"

-----------------------------------------------------------*/

void PCF8563::setDateTime(String date, String time)
{
	uint8_t reg_seconds, vl_bit, century_bit;
	uint8_t day, month, hours, minutes, seconds;
	uint16_t year, year_full, month_full;

	reg_seconds = _read_one_register(R_VL_SECONDS);
	vl_bit = bitRead(reg_seconds, VL); // Check the oscillator status to retain the status
	
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
	year = date.substring(7,11).toInt();

	if(year >= 1900 && year <= 1999)
		century_bit = 1;
	else if (year >= 2000 && year <= 2099)
		century_bit = 0;

	year_full = year;
	year = year % 100;		//Converting to 2 Digit

	month_full = month;


	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(R_VL_SECONDS);

	seconds = bin2bcd(seconds);
	bitWrite(seconds,VL,vl_bit);
	Wire.write(seconds);
	
	Wire.write(bin2bcd(minutes));
	Wire.write(bin2bcd(hours));

	Wire.write(bin2bcd(day));
	Wire.write(_calculateDayOfWeek(day, month_full, year_full)-1);

	month = bin2bcd(month);
	bitWrite(month,7,century_bit);
	Wire.write(month);
	
	Wire.write(bin2bcd(year));
	Wire.endTransmission();
}

/*-----------------------------------------------------------
	setDateTime(__TIMESTAMP__)
    Uses Built in variable to set time 

    //Timestamp format Fri Mar 08 13:01:53 2024
-----------------------------------------------------------*/

void PCF8563::setDateTime(String timestamp)
{
	uint8_t reg_seconds, century_bit, vl_bit;
	uint8_t day, month, hours, week, minutes, seconds, year;
	uint16_t year_full;
	String month_str, week_str;

	reg_seconds = _read_one_register(R_VL_SECONDS);
	vl_bit = bitRead(reg_seconds, VL); // Check the oscillator status to retain the status
	
	week_str = timestamp.substring(0,3);
	month_str = timestamp.substring(4,7);
	day = timestamp.substring(8,11).toInt();
	hours = timestamp.substring(11,13).toInt();
	minutes = timestamp.substring(14,16).toInt();
	seconds = timestamp.substring(17,19).toInt();
	year_full = timestamp.substring(19,24).toInt();

	if(year_full >= 1900 && year_full <= 1999)
		century_bit = 1;
	else if (year_full >= 2000 && year <= 2099)
		century_bit = 0;

	year = year_full % 100;		//Converting to 2 Digit

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

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(R_VL_SECONDS);

	seconds = bin2bcd(seconds);
	bitWrite(seconds,VL,vl_bit);
	Wire.write(seconds);
	
	Wire.write(bin2bcd(minutes));
	Wire.write(bin2bcd(hours));
	Wire.write(bin2bcd(day));
	Wire.write(week-1);

	month = bin2bcd(month);
	bitWrite(month,7,century_bit);
	Wire.write(month);

	Wire.write(bin2bcd(year));
	Wire.endTransmission();
}

String PCF8563::getTimeString()
{
   	uint8_t seconds, minutes, hours;
	String timeString;

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(R_VL_SECONDS);
	Wire.endTransmission();
	Wire.requestFrom(PCF8563_ADDR, 3);

	seconds = Wire.read();
	bitClear(seconds, 7);
	seconds = bcd2bin(seconds);

	minutes = Wire.read();
	minutes = bcd2bin(minutes);

	hours = Wire.read();
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

	return (timeString);
}

String PCF8563::getDateString()
{
    uint8_t day, week, month, century_bit;
	uint16_t century, year;
	String dateString;

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(R_DAYS);
	Wire.endTransmission();
	Wire.requestFrom(PCF8563_ADDR, 7);

	day = Wire.read();
	day = bcd2bin(day);

	week = Wire.read(); // Read Weekday but not used
	
	month = Wire.read();
	century_bit = bitRead(month, 7);
	month = bcd2bin(bitClear(month, 7));

	(century_bit == 1) ? century = 1900 : century = 2000;

	year = Wire.read();
	year = bcd2bin(year) + century;

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

String PCF8563::getWeekString()
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

// Returns SAT 08-03-2047 15:00:04


String PCF8563::getDateTimeString()
{
	uint8_t seconds, minutes, hours;
	uint8_t day, week, month, century_bit;
	uint16_t century, year;
	String dateTimeString;

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(R_VL_SECONDS);
	Wire.endTransmission();
	Wire.requestFrom(PCF8563_ADDR, 10);

	seconds = Wire.read();
	bitClear(seconds, 7);
	seconds = bcd2bin(seconds);

	minutes = Wire.read();
	minutes = bcd2bin(minutes);

	hours = Wire.read();
	hours = bcd2bin(hours);

	day = Wire.read();
	day = bcd2bin(day);

	week = Wire.read(); // Read Weekday but not used
	month = Wire.read();
	
	century_bit = bitRead(month, 7);
	month = bcd2bin(bitClear(month, 7));
	(century_bit == 1) ? century = 1900 : century = 2000;
	year = Wire.read();	
	year = bcd2bin(year) + century;

	switch (week+1)
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

	// Formatting Date
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
	
	// Formatting Time
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
	
	return (dateTimeString);
}

void PCF8563::setDateTime(tm rtc_time)
{
	uint8_t day, month, week, year_2d;
	uint8_t hours, minutes, seconds,reg_seconds;
	uint8_t vl_bit, century_bit;
	uint16_t year_4d;

	reg_seconds = _read_one_register(R_VL_SECONDS);
	vl_bit = bitRead(reg_seconds, VL); // Check the oscillator status to retain the status
	

	seconds = rtc_time.tm_sec;
	minutes = rtc_time.tm_min;
	hours = rtc_time.tm_hour;
	day = rtc_time.tm_mday;
	month = rtc_time.tm_mon + 1; // tm_mon is 0-11
	year_4d = rtc_time.tm_year + 1900; // tm_year is years since 1900
	week = rtc_time.tm_wday; // tm_wday is 0-6

	if(year_4d >= 1900 && year_4d <= 1999)
		century_bit = 1;
	else if (year_4d >= 2000 && year_4d <= 2099)
		century_bit = 0;

	year_2d = year_4d % 100;		//Converting to 2 Digit

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(R_VL_SECONDS);

	seconds = bin2bcd(seconds);
	bitWrite(seconds,VL,vl_bit);
	Wire.write(bin2bcd(seconds));
	
	Wire.write(bin2bcd(minutes));
	Wire.write(bin2bcd(hours));
	Wire.write(bin2bcd(day));
	Wire.write(week);
	
	month = bin2bcd(month);
	bitWrite(month,7,century_bit);
	Wire.write(month);

	Wire.write(bin2bcd(year_2d)); // Only last two digits
	Wire.endTransmission();
}
/*-----------------------------------------------------------
setEpoch()
	* Weekday Might not work properly
	* Fix Year Logic
-----------------------------------------------------------*/

void PCF8563::setEpoch(time_t epoch)
{
	uint8_t data, century;
	uint16_t year;
	struct tm epoch_tm, *ptr_epoch_tm;

	ptr_epoch_tm = gmtime(&epoch);
	epoch_tm = *ptr_epoch_tm;

	century = (epoch_tm.tm_year + 1870) / 100;	// Find Century 
	year = (epoch_tm.tm_year + 1870) % 100;		//Converting to 2 Digit

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x02);  // Seconds Register
	Wire.write(bin2bcd(epoch_tm.tm_sec));	//0x02
	Wire.write(bin2bcd(epoch_tm.tm_min));	//0x03
	Wire.write(bin2bcd(epoch_tm.tm_hour));	//0x04
	Wire.write(bin2bcd(epoch_tm.tm_mday));	//0x05
	Wire.write(bin2bcd(epoch_tm.tm_wday));	//0x06
	Wire.write(bin2bcd(epoch_tm.tm_mon+1));	//0x07
	Wire.write(bin2bcd(year));	//0x08
	Wire.endTransmission();

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x07);		// Century and month Register
	Wire.endTransmission();

	Wire.requestFrom(PCF8563_ADDR, 1);
	data = Wire.read();

	// Set century bit to 1 for year > 2000;
	if(century == 20)
		bitSet(data,7);
	else
		bitClear(data,7);
	
	// Write Century bit to Month Register(0x07)
	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x07);  //
	Wire.write(data);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getEpoch()
	* Weekday Might not work properly
-----------------------------------------------------------*/
time_t PCF8563::getEpoch()
{
	uint8_t century_bit;
	uint16_t century;
	time_t epoch;
	struct tm epoch_tm;
	
	Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x02);  // Seconds Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR,7);
	
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

/*-----------------------------------------------------------
Alarm Functions
-----------------------------------------------------------*/
void PCF8563::setAlarmMinutes(uint8_t minutes)
{	if (minutes >= 00 && minutes <= 59)
	{
		Wire.beginTransmission(PCF8563_ADDR);
		Wire.write(0x09);
		Wire.write(bin2bcd(minutes));
		Wire.endTransmission();
	}
}

void PCF8563::setAlarmHours(uint8_t hours)
{
	if (hours >= 00 && hours <= 23)
	{
		Wire.beginTransmission(PCF8563_ADDR);
		Wire.write(0x0A);
		Wire.write(bin2bcd(hours));
		Wire.endTransmission();
	}
}

void PCF8563::setAlarmDay(uint8_t day)
{
	if (day >= 1 && day <= 31)
	{
		Wire.beginTransmission(PCF8563_ADDR);
		Wire.write(0x0B);
		Wire.write(bin2bcd(day));
		Wire.endTransmission();
	}
}
void PCF8563::setAlarmWeek(uint8_t week)
{
	if (week >= 1 && week <= 7)
	{
		Wire.beginTransmission(PCF8563_ADDR);
		Wire.write(0x0C);
		Wire.write(bin2bcd(week));
		Wire.endTransmission();
	}
}

uint8_t PCF8563::getAlarmMinutes()
{
	uint8_t minutes;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 4);
    
	minutes = Wire.read();
	bitClear(minutes,7);
	minutes = bcd2bin(minutes);

	return (minutes);
}


uint8_t PCF8563::getAlarmHours()
{
	uint8_t hours;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0A);
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 4);

	hours = Wire.read();
	bitClear(hours,7);
	hours = bcd2bin(hours);

	return hours;


}

uint8_t PCF8563::getAlarmDay()
{
	uint8_t day;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0B);
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 4);

	day = Wire.read();
	bitClear(day,7);
	day = bcd2bin(day);

	return(day);
}

uint8_t PCF8563::getAlarmWeek()
{
	uint8_t week;


    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0C);
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 4);
    


	week = Wire.read();
	bitClear(week,7);
	week = bcd2bin(week);

	return(week);
}


/*-----------------------------------------------------------
void disableAlarm()
-----------------------------------------------------------*/

void PCF8563::disableAlarm()
{
    uint8_t minutes,hours,day,weekday;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);  // Alarm Hour Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 4);

    minutes = Wire.read();
    hours = Wire.read();
    day = Wire.read();
    weekday = Wire.read();

    bitSet(minutes,7);
    bitSet(hours,7);
    bitSet(day,7);
    bitSet(weekday,7);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);  // Month Register
    Wire.write(minutes);
    Wire.write(hours);
    Wire.write(day);
    Wire.write(weekday);
    Wire.endTransmission();
}


/*-----------------------------------------------------------
void clearAlarm();
-----------------------------------------------------------*/

void PCF8563::clearAlarm(void)
{
    uint8_t data;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data = Wire.read();
    bitClear(data,3);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.write(data);
    Wire.endTransmission();
}

/*-----------------------------------------------------------
bool isAlarmTriggered();
-----------------------------------------------------------*/

bool PCF8563::isAlarmTriggered(void)
{
    uint8_t data;
    bool flag;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data= Wire.read();
    flag = bitRead(data,3);

    return (flag);
}

bool PCF8563::isAlarmEnabled(void)
{
    uint8_t minutes,hours,day,weekday;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 4);
    minutes = Wire.read();
    hours = Wire.read();
    day = Wire.read();
    weekday = Wire.read();

    minutes = bitRead(minutes,7);
    hours = bitRead(hours,7);
    day = bitRead(day,7);
    weekday = bitRead(weekday,7);

    if(minutes && hours && day && weekday)
        return false;
    else
        return true;
}


void PCF8563::enableAlarmInterrupt(void)
{
    uint8_t reg_01;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    reg_01 = Wire.read();
    bitSet(reg_01,1);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.write(reg_01);
    Wire.endTransmission();
}

void PCF8563::disableAlarmInterrupt(void)
{
    uint8_t reg_01;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    reg_01 = Wire.read();
    bitClear(reg_01,1);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.write(reg_01);
    Wire.endTransmission();
}

/*-----------------------------------------------------------
Timer Functions

bool isTimerEnabled();
-----------------------------------------------------------*/

bool PCF8563::isTimerEnabled(void)
{
    uint8_t reg_data;
	bool flag;
	reg_data = _read_one_register(R_TIMER_CONTROL);
	flag = bitRead(reg_data,7);
	return (flag);
}


void PCF8563::enableTimer(void)
{
    uint8_t reg_data;
	reg_data = _read_one_register(R_TIMER_CONTROL);
	bitSet(reg_data,7);  // Enable Timer
	bitSet(reg_data,1);  // Timer Clock 1Hz
	bitClear(reg_data,0); // can write 0x83 to enable Timer at 1Hz
	_write_one_register(R_TIMER_CONTROL,reg_data);
}


void PCF8563::disableTimer(void)
{	
	uint8_t reg_data;
	reg_data = _read_one_register(R_TIMER_CONTROL);
	bitClear(reg_data,7);  // Disable Timer
	_write_one_register(R_TIMER_CONTROL,reg_data);
}

bool PCF8563::getTimerFlag()
{
	uint8_t reg_data;
	bool flag;
	reg_data = _read_one_register(R_CONTROL_STATUS_2);
	flag = bitRead(reg_data,2);
	return (flag);
}

void PCF8563::clearTimerFlag()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_CONTROL_STATUS_2);
	bitClear(reg_data,2);  // Clear Timer Flag
	_write_one_register(R_CONTROL_STATUS_2,reg_data);
}

void PCF8563::enableTimerInterrupt(void)
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_CONTROL_STATUS_2);
	bitSet(reg_data,0);  // Enable Timer Interrupt
	_write_one_register(R_CONTROL_STATUS_2,reg_data);	

}

void PCF8563::disableTimerInterrupt(void)
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_CONTROL_STATUS_2);
	bitClear(reg_data,0);  // Disable Timer Interrupt
	_write_one_register(R_CONTROL_STATUS_2,reg_data);
}


void PCF8563::setTimer(uint8_t t_seconds)
{
	_write_one_register(R_TIMER, t_seconds);
}

uint8_t PCF8563::getTimer(void)
{
    uint8_t t_seconds;
	t_seconds = _read_one_register(R_TIMER);
	return (t_seconds);
}




/**************************************************************
 * 
 * Private Functions
 *  
**************************************************************/


uint8_t PCF8563::_read_one_register(uint8_t reg_address)
{
    uint8_t reg_data;
    
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.endTransmission();

    Wire.requestFrom((int)_i2c_address,(int) 1);
    reg_data = Wire.read();
    return(reg_data);

}

void PCF8563::_write_one_register(uint8_t reg_address, uint8_t reg_data)
{
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.write(reg_data);
    Wire.endTransmission();
}

uint8_t PCF8563::bcd2bin(uint8_t val)
{
	return val - 6 * (val >> 4);
}

uint8_t PCF8563::bin2bcd(uint8_t val)
{
	return val + 6 * (val / 10);
}

uint8_t PCF8563::_calculateDayOfWeek(uint8_t day, uint8_t month, uint16_t year)
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
