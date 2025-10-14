/*
 * DS3231.cpp - Library to set & get time from RTC DS3231
 * Created by Manjunath CV. July 08, 2017, 02:18 AM
 * Released into the public domain.
 */

#include <time.h>
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

#define R_ALM1_SECONDS 	0X07
#define R_ALM1_MINUTES 	0X08
#define R_ALM1_HOURS	0X09
#define R_ALM1_WEEKDAY 	0X0A

#define R_ALM2_MINUTES 	0X0B
#define R_ALM2_HOURS	0X0C
#define R_ALM2_WEEKDAY 	0X0D

#define R_CONTROL	0X0E
#define R_STATUS	0X0F
#define R_OFFSET	0X10

#define R_TEMP_MSB	0X11
#define R_TEMP_LSB 	0X12



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
	Wire.write(R_CONTROL);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 2);

	data_e = Wire.read();	// Read EOSC Register to check Enable Oscillator
	data_f = Wire.read();	// Read  OSF Register to check status.

	data_e = bitRead(data_e, 7);
	data_f = bitRead(data_f, 7);

	if(data_e == 0 && data_f == 0)
		return true;
	else
		return false;

	//return (!(data_e | data_f));
}

/*
 * 
 */

void DS3231::startClock(void)
{
	uint8_t reg_control, reg_status;

	reg_control = _read_one_register(R_CONTROL);
	reg_status = _read_one_register(R_STATUS);

	bitClear(reg_control, 7); // Write  OSF Register to 0 to start the clock.
	bitClear(reg_status, 7); // Write  EOSC Register to 0 to start the clock.

	_write_one_register(R_CONTROL,reg_control);
	_write_one_register(R_STATUS,reg_status);
}



void DS3231::stopClock(void)
{
	uint8_t reg_control;

	reg_control = _read_one_register(R_CONTROL);
	bitSet(reg_control, 7); // Write EOSC Register to 1 to stop the clock.
	_write_one_register(R_CONTROL,reg_control);
}



/*-----------------------------------------------------------
get & set HourMode
-----------------------------------------------------------*/

void DS3231::setHourMode(uint8_t h_mode)
{
	uint8_t reg_hours, reg_alm1_hours, reg_alm2_hours;

	if(h_mode == CLOCK_H12 || h_mode == CLOCK_H24)
	{
		reg_hours = _read_one_register(R_HOURS);
		reg_alm1_hours = _read_one_register(R_ALM1_HOURS);
		reg_alm2_hours = _read_one_register(R_ALM2_HOURS);

		bitWrite(reg_hours, 6, h_mode);	
		bitWrite(reg_alm1_hours, 6, h_mode);
		bitWrite(reg_alm2_hours, 6, h_mode);

		_write_one_register(R_HOURS,reg_hours);
		_write_one_register(R_ALM1_HOURS,reg_alm1_hours);
		_write_one_register(R_ALM2_HOURS,reg_alm2_hours);
	}
}

uint8_t DS3231::getHourMode()
{
	// May be there is no need to read all the registers
	bool clock_h_mode, alarm1_h_mode,alarm2_h_mode;
	uint8_t reg_hours, reg_alm1_hours, reg_alm2_hours;

	reg_hours = _read_one_register(R_HOURS);
	reg_alm1_hours = _read_one_register(R_ALM1_HOURS);
	reg_alm2_hours = _read_one_register(R_ALM2_HOURS);

	clock_h_mode = bitRead(reg_hours, 6);
	alarm1_h_mode = bitRead(reg_alm1_hours, 6);
	alarm2_h_mode = bitRead(reg_alm2_hours, 6);

	return (clock_h_mode | alarm1_h_mode | alarm2_h_mode);
}

/*-----------------------------------------------------------
setMeridiem(uint8_t meridiem)
-----------------------------------------------------------*/

void DS3231::setMeridiem(uint8_t meridiem)
{
	uint8_t reg_data, h_mode;
	if(meridiem == HOUR_AM || meridiem == HOUR_PM)
	{
		reg_data = _read_one_register(R_HOURS);
		h_mode = bitRead(reg_data, 6); //Get hour mode
		if(h_mode == CLOCK_H12)
		{
			bitWrite(reg_data, 5, meridiem);
			_write_one_register(R_HOURS,reg_data);
		}
	}
}

uint8_t DS3231::getMeridiem()
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

uint8_t DS3231::getSeconds()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_SECONDS);
	return (bcd2bin(reg_data));
}

void DS3231::setSeconds(uint8_t seconds)
{
	uint8_t reg_data;
	if (seconds >= 00 && seconds <= 59)
		_write_one_register(R_SECONDS,bin2bcd(seconds));
}

/*-----------------------------------------------------------
getMinutes
-----------------------------------------------------------*/

void DS3231::setMinutes(uint8_t minutes)
{
	uint8_t reg_data;
	if(minutes >= 00 && minutes <= 59)
		_write_one_register(R_MINUTES,bin2bcd(minutes));
}

uint8_t DS3231::getMinutes()
{
	uint8_t reg_data;
	reg_data = _read_one_register(R_MINUTES);
	return (bcd2bin(reg_data));
}

/*-----------------------------------------------------------
getHour
-----------------------------------------------------------*/

void  DS3231::setHours(uint8_t hours)
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

uint8_t DS3231::getHours()
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



/*-----------------------------------------------------------
getWeek
-----------------------------------------------------------*/
void DS3231::setWeek(uint8_t week)
{
	if (week >= 1 && week <= 7)
		_write_one_register(R_WEEKDAY,week);
}

uint8_t DS3231::getWeek()
{
	uint8_t reg_week;
	reg_week = _read_one_register(R_WEEKDAY);
	return reg_week;
}

void DS3231::updateWeek()
{
	// Calculate Day of week based on date, month and year
	// and update the weekday register

	uint8_t day, month, century_bit, week_day;
	uint16_t century, year;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(R_DATE);
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 7);

	day = bcd2bin(Wire.read());
	
	month = Wire.read();
	century_bit = bitRead(month, 7);
	month = bcd2bin(bitClear(month, 7));

	(century_bit == 1) ? century = 1900 : century = 2000;

	year = Wire.read();
	year = bcd2bin(year) + century;

	week_day = _calculateDayOfWeek(day, month, year);

	_write_one_register(R_WEEKDAY,week_day);
}

/*-----------------------------------------------------------
getDay
-----------------------------------------------------------*/
uint8_t DS3231::getDay()
{
	uint8_t reg_date;
	reg_date = _read_one_register(R_DATE);
	return(bcd2bin(reg_date));
}

void DS3231::setDay(uint8_t day)
{
	if (day >= 1 && day <= 31)
		_write_one_register(R_DATE,bin2bcd(day));
}
/*-----------------------------------------------------------
setMonth()
-----------------------------------------------------------*/

void DS3231::setMonth(uint8_t month)
{
	uint8_t reg_month, century_bit;

	if (month >= 1 && month <= 12)
	{
		reg_month = _read_one_register(R_MONTH);
		century_bit = bitRead(reg_month, 7);
		month = bin2bcd(month);
		bitWrite(month, 7, century_bit);
		_write_one_register(R_MONTH,month);
	}
}

/*-----------------------------------------------------------
getMonth ()

-----------------------------------------------------------*/

uint8_t DS3231::getMonth()
{
	uint8_t reg_month;
	reg_month = _read_one_register(R_MONTH);
	bitClear(reg_month,7);		//Clear Century Bit;
	return(bcd2bin(reg_month));
}

/*-----------------------------------------------------------
getYear (Completed)

Century Bit
1 = 1900s
0 = 2000s
-----------------------------------------------------------*/
void DS3231::setYear(uint16_t year)
{
	uint8_t reg_month,century_bit,reg_year;

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
		reg_month = _read_one_register(R_MONTH);
		if(century_bit == 1)
			bitSet(reg_month,7);
		else
			bitClear(reg_month,7);
		
		_write_one_register(R_MONTH,reg_month);
		_write_one_register(R_YEAR,bin2bcd(year));
	}
}

uint16_t DS3231::getYear()
{
	uint8_t reg_month,century_bit,reg_year;
	uint16_t century,year;

	// Read Month register for Century
	reg_month = _read_one_register(R_MONTH);
	century_bit = bitRead(reg_month, 7);

	(century_bit == 1) ? century = 1900 : century = 2000;

	reg_year = _read_one_register(R_YEAR);
	year = bcd2bin(reg_year) + century;
	return (year);

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
/*-----------------------------------------------------------
setDateTime(__DATE__,__TIME__)

// sample input

__DATE__ = "Dec 26 2009"
__TIME__ = "12:34:56"

-----------------------------------------------------------*/

void DS3231::setDateTime(String date, String time)
{
	bool h_mode, meridiem, century_bit;
	uint8_t reg_hours;
	uint8_t day, month, hours, minutes, seconds, month_full;
	uint16_t year, year_full;
	
	reg_hours   = _read_one_register(R_HOURS);
	h_mode = bitRead(reg_hours, 6); //Get hour mode

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
	month = bin2bcd(month);

	bitWrite(month, 7, century_bit);

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(R_SECONDS);

	Wire.write(bin2bcd(seconds));
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
	
	Wire.write(_calculateDayOfWeek(day, month_full, year_full));
	Wire.write(bin2bcd(day));
	Wire.write(month);
	Wire.write(bin2bcd(year));
	Wire.endTransmission();
}
/*-----------------------------------------------------------
setDateTime(__TIMESTAMP__)
Uses Built in variable to set time 

//__TIMESTAMP__ = "Fri Mar 08 13:01:53 2024"
-----------------------------------------------------------*/

void DS3231::setDateTime(String timestamp)
{
	bool h_mode, meridiem, century_bit;
	uint8_t reg_hours;
	uint8_t week, day, month, hours, minutes, seconds;
	uint16_t year;
	String month_str, week_str;

	reg_hours   = _read_one_register(R_HOURS);
	h_mode = bitRead(reg_hours, 6); //Get hour mode

	week_str = timestamp.substring(0,3);
	month_str = timestamp.substring(4,7);
	day = timestamp.substring(8,11).toInt();
	hours = timestamp.substring(11,13).toInt();
	minutes = timestamp.substring(14,16).toInt();
	seconds = timestamp.substring(17,19).toInt();
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

	if(year >= 1900 && year <= 1999)
		century_bit = 1;
	else if (year >= 2000 && year <= 2099)
		century_bit = 0;
	
	year = year % 100;		//Converting to 2 Digit

	month = bin2bcd(month);
	bitWrite(month, 7, century_bit);

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(R_SECONDS);  // Seconds Register

	Wire.write(bin2bcd(seconds));
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
	Wire.write(month);
	Wire.write(bin2bcd(year));
	Wire.endTransmission();

}

void DS3231::setDateTime(tm)
{
}

/*-----------------------------------------------------------
setEpoch()

https://en.wikipedia.org/wiki/Epoch_(computing)
-----------------------------------------------------------*/
//void DS3231::setEpoch(time_t epoch, bool is_unix_epoch=true)

void DS3231::setEpoch(time_t epoch)
{
	#if defined(__AVR__)
		if(epoch > UNIX_OFFSET) // 1970 to 2000
			epoch = epoch - UNIX_OFFSET;
		else
			epoch = 0;
	#endif

	bool h_mode, meridiem, century_bit;
	uint8_t reg_hours,reg_month,reg_years;
	struct tm epoch_tm, *ptr_epoch_tm;
	
	h_mode = bitRead(_read_one_register(R_HOURS), 6); //Get hour mode
	
	ptr_epoch_tm = gmtime(&epoch);
	epoch_tm = *ptr_epoch_tm;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(R_SECONDS);  // Seconds Register
	Wire.write(bin2bcd(epoch_tm.tm_sec));		//0x00 Seconds
	Wire.write(bin2bcd(epoch_tm.tm_min));		//0x01 Minutes
	if (h_mode == CLOCK_H24)
	{
		Wire.write(bin2bcd(epoch_tm.tm_hour));	//0x02 Hours
	}
	else if (h_mode == CLOCK_H12)
	{
		meridiem = (epoch_tm.tm_hour >= 12);
		if (epoch_tm.tm_hour == 0)
			epoch_tm.tm_hour = 12;
		if (epoch_tm.tm_hour > 12)
			epoch_tm.tm_hour -= 12;
		reg_hours = bin2bcd(epoch_tm.tm_hour);
		bitWrite(reg_hours, 5, meridiem);
		bitSet(reg_hours, 6);
		Wire.write(reg_hours); //0x02 Hours
	}
	Wire.write(bin2bcd(epoch_tm.tm_wday+1));	//0x03 Week Day
	Wire.write(bin2bcd(epoch_tm.tm_mday));		//0x04 Date

	if(epoch_tm.tm_year < 100)
		century_bit = 1; // 1900s
	else
		century_bit = 0; // 2000s

	reg_month = bin2bcd(epoch_tm.tm_mon+1); //Converting to 1-12
	bitWrite(reg_month, 7, century_bit); 	// Set Century Bit in Month Register(0x05)
	Wire.write(reg_month);					//0x05 Month with Century Bit

	reg_years = epoch_tm.tm_year % 100; //Converting to 2 Digit
	Wire.write(bin2bcd(reg_years));				//0x06 Year

	Wire.endTransmission();
}

/*-----------------------------------------------------------
getEpoch()
-----------------------------------------------------------*/
time_t DS3231::getEpoch()
{

	bool h_mode, meridiem, century_bit;
	uint8_t reg_hours,reg_month;
	int16_t reg_years;

	time_t epoch;
	struct tm epoch_tm;


	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(R_SECONDS);  // Seconds Register
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR,7);

	epoch_tm.tm_sec = bcd2bin(Wire.read());		//0x00 Seconds
	epoch_tm.tm_min = bcd2bin(Wire.read());		//0x01 Minutes

	reg_hours = Wire.read();					//0x02 Hours
	h_mode = bitRead(reg_hours, 6);
	
	if (h_mode == CLOCK_H24)
	{
		epoch_tm.tm_hour = bcd2bin(reg_hours);	
	}
	else //h_mode == CLOCK_H12
	{
		meridiem = bitRead(reg_hours, 5);
		bitClear(reg_hours, 6);
		bitClear(reg_hours, 5);
		reg_hours = bcd2bin(reg_hours);

		if (meridiem == HOUR_PM && reg_hours != 12)
			epoch_tm.tm_hour += 12;
		else if (meridiem == HOUR_PM && reg_hours == 12)
			epoch_tm.tm_hour = 12;
		else if (meridiem == HOUR_AM && reg_hours == 12)
			epoch_tm.tm_hour = 0;
		else
			epoch_tm.tm_hour = reg_hours;
	}
	epoch_tm.tm_wday = Wire.read()-1;	//0x03 Week Day
	epoch_tm.tm_mday = bcd2bin(Wire.read());	//0x04 Date
	
	reg_month = Wire.read();					//0x05 Months
	century_bit = bitRead(reg_month, 7);
	bitClear(reg_month,7);
	epoch_tm.tm_mon = bcd2bin(reg_month)-1;

	reg_years = bcd2bin(Wire.read());	//0x06 Years
	
	if(century_bit == 1)
		epoch_tm.tm_year = reg_years;
	else
		epoch_tm.tm_year = reg_years + 100;

	epoch = mktime(&epoch_tm);

	#if defined(__AVR__)
		epoch = epoch + UNIX_OFFSET;		
		return (epoch);
	#else
		return (epoch);
	#endif
}


String DS3231::getTimeString()
{
   	uint8_t seconds, minutes, hours, meridiem, h_mode;
	uint16_t year;
	String timeString;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(R_SECONDS);
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 3);

	seconds = bcd2bin(Wire.read());
	minutes = bcd2bin(Wire.read());

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

String DS3231::getDateString()
{
    uint8_t day, month, century_bit;
	uint16_t century, year;
	String dateString;

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(R_DATE);
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 7);

	day = bcd2bin(Wire.read());
	
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

String DS3231::getWeekString()
{
	uint8_t week;
	String weekString;
	
	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(R_WEEKDAY);
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 1);
	week = Wire.read();
	
	switch (week)
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
enableAlarmPin()
-----------------------------------------------------------*/
void DS3231::enableAlarmPin()
{
	uint8_t reg_control;

	reg_control = _read_one_register(R_CONTROL);
	bitSet(reg_control, 2); // Write bit INTCN to 1 to enable INT/SQW pin
	_write_one_register(R_CONTROL,reg_control);

}

void DS3231::enableAlarm1()
{
	uint8_t reg_control, reg_status;

	reg_control = _read_one_register(R_CONTROL);
	bitSet(reg_control, 0); // Write  A1IE Register to 1 to enable Alarm 1
	_write_one_register(R_CONTROL,reg_control);

	reg_status = _read_one_register(R_STATUS);
	bitClear(reg_status, 0); // Write  A1F Register to 0 to clear Alaram 1 flag
	_write_one_register(R_STATUS,reg_status);
}

void DS3231::enableAlarm2()
{
	uint8_t reg_control, reg_status;

	reg_control = _read_one_register(R_CONTROL);
	bitSet(reg_control, 1); // Write  A2IE Register to 1 to enable Alarm 2
	_write_one_register(R_CONTROL,reg_control);

	reg_status = _read_one_register(R_STATUS);
	bitClear(reg_status, 1); // Write  A2F Register to 0 to clear Alaram 2 flag
	_write_one_register(R_STATUS,reg_status);

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
void DS3231::setAgingOffset(int8_t data)
{
	_write_one_register(R_OFFSET, data);  // Aging Offset
}

int8_t DS3231::getAgingOffset()
{
	return(_read_one_register(R_OFFSET));  // Aging Offset
}


float DS3231::getTemp()
{
	float f_temp;
	uint8_t reg_control, reg_status, temp_msb, temp_lsb;

	reg_control = _read_one_register(R_CONTROL);
	bitWrite(reg_control, 5, 1); // Force Temp Conversion
	_write_one_register(R_CONTROL, reg_control);

	reg_status = _read_one_register(R_STATUS);
	while (!bitRead(reg_status, 2))
	{
		reg_status = _read_one_register(R_STATUS);
	}

	Wire.beginTransmission(DS3231_ADDR);
	Wire.write(R_TEMP_MSB);
	Wire.endTransmission();

	Wire.requestFrom(DS3231_ADDR, 2);
	temp_msb = Wire.read();
	temp_lsb = Wire.read() >> 6;

	f_temp = (0.25 * temp_lsb) + temp_msb;

	return(f_temp);
}

/* Private Functions */

uint8_t DS3231::_calculateDayOfWeek(uint8_t day, uint8_t month, uint16_t year)
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

/* LINE NO 1465 */