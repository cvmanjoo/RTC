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
#define R_NVRAM	  0x08

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

// uint8_t DS1307::begin(uint8_t sda, uint8_t scl)
// { 
// 	Wire.begin();
// 	Wire.setSDA(sda);
// 	Wire.setSCL(scl);
//     return(DS1307_ADDR);
// 	//Wire.endTransmission();
// }


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
		ch_bit = bitRead(_read_one_register(R_SECONDS), CH);
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
	uint8_t minutes;
	minutes = _read_one_register(R_MINUTES);
	return (bcd2bin(minutes));

}

void DS1307::setMinutes(uint8_t minutes)
{
	if(minutes >= 00 && minutes <= 59)
		_write_one_register(R_MINUTES,bin2bcd(minutes));
}

/*-----------------------------------------------------------
getHours
-----------------------------------------------------------*/
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
			bitSet(hours, 6); //Might not needed!
			_write_one_register(R_HOURS,hours);
		}
	}
}


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


/*-----------------------------------------------------------
getWeek
-----------------------------------------------------------*/
void DS1307::setWeek(uint8_t week)
{
	if (week >= 1 && week <= 7)
		_write_one_register(R_WEEKDAY,week);
}

uint8_t DS1307::getWeek()
{
	uint8_t reg_week;
	reg_week = _read_one_register(R_WEEKDAY);
	return reg_week;
}

void DS1307::updateWeek()
{
	// Calculate Day of week based on date, month and year
	// and update the weekday register

	uint8_t day, month, week_day;
	uint16_t year;

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(R_DATE);
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDR, 3);

	day = bcd2bin(Wire.read());
	month =bcd2bin(Wire.read());
	year = bcd2bin(Wire.read()) + 2000;

	week_day = _calculateDayOfWeek(day, month, year);
	_write_one_register(R_WEEKDAY,week_day);
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
setMonth()
-----------------------------------------------------------*/

void DS1307::setMonth(uint8_t month)
{
	if (month >= 1 && month <= 12)
		_write_one_register(R_MONTH,bin2bcd(month));
}

uint8_t DS1307::getMonth()
{
	uint8_t reg_month;
	reg_month = _read_one_register(R_MONTH);
	return(bcd2bin(reg_month));
}

/*-----------------------------------------------------------
getYear (Completed)
-----------------------------------------------------------*/

void DS1307::setYear(uint16_t year)
{	
	if((year >= 00 && year <= 99) || (year >= 2000 && year <= 2099))
	{
		year = year % 100; //Converting to 2 Digit
		_write_one_register(R_YEAR,bin2bcd(year));
	}
}

uint16_t DS1307::getYear()
{
	uint8_t reg_year;
	reg_year = _read_one_register(R_YEAR);
	return(bcd2bin(reg_year) + 2000);
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

// Note: This function assumes that the input date and time strings are always valid
// and in the correct format as provided by the compiler macros __DATE__ and __TIME__
// It does not perform any error checking or validation on the input strings.
// Example input: __DATE__ = "Dec 26 2009", __TIME__= "12:34:56"
// The function extracts the day, month, year, hour, minute, and second from
// the input strings and sets the RTC accordingly.

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
	
	Wire.write(_calculateDayOfWeek(day, month, year));
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

	bool ch_bit, h_mode, meridiem;
	uint8_t reg_seconds, reg_hours;;
	struct tm epoch_tm, *ptr_epoch_tm;

	if(epoch >= UNIX_OFFSET)
	{
		#if defined(ARDUINO_ARCH_AVR)
			epoch = epoch - UNIX_OFFSET;
		#endif
			
		ch_bit = bitRead(_read_one_register(R_SECONDS), CH); // Check the oscillator status to retain the status
		h_mode = bitRead(_read_one_register(R_HOURS), 6); //Get hour mode

		ptr_epoch_tm = gmtime(&epoch);
		epoch_tm = *ptr_epoch_tm;

		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(R_SECONDS);

		reg_seconds = bin2bcd(epoch_tm.tm_sec);
		bitWrite(reg_seconds,CH,ch_bit);
		Wire.write(reg_seconds);	//0x00 Seconds
		
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

		Wire.write(epoch_tm.tm_wday + 1);			//0x03 Day of week
		Wire.write(bin2bcd(epoch_tm.tm_mday));		//0x04 Date
		Wire.write(bin2bcd(epoch_tm.tm_mon + 1));	//0x05 Month
		Wire.write(bin2bcd((epoch_tm.tm_year) % 100));		//0x06 Year
		Wire.endTransmission();	
	}
	else
	{
		//Serial.println("Error: Invalid Epoch");
	}

}

/*-----------------------------------------------------------
getEpoch()
-----------------------------------------------------------*/
time_t DS1307::getEpoch()
{

	bool h_mode, meridiem, century_bit;
	uint8_t reg_seconds, reg_hours;

	time_t epoch;
	struct tm epoch_tm;

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(R_SECONDS);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 7);

	reg_seconds = Wire.read(); //0x00 Seconds

	epoch_tm.tm_sec = bcd2bin(bitClear(reg_seconds,CH));		
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
		{
			epoch_tm.tm_hour = reg_hours + 12;
		}
		else if (meridiem == HOUR_PM && reg_hours == 12)
		{
			epoch_tm.tm_hour = 12;
		}
		else if (meridiem == HOUR_AM && reg_hours == 12)
		{	
			epoch_tm.tm_hour = 0;
		}
		else
		{
			epoch_tm.tm_hour = reg_hours;
		}
	}

	epoch_tm.tm_wday = Wire.read()-1;				//0x03 Week Day
	epoch_tm.tm_mday = bcd2bin(Wire.read());		//0x04 Date
	epoch_tm.tm_mon  = bcd2bin(Wire.read()) - 1;	//0x05 Months
	epoch_tm.tm_year = bcd2bin(Wire.read()) + 100;	//0x06 Years since 1900
	
	epoch = mktime(&epoch_tm);

	#if defined(ARDUINO_ARCH_AVR)
		epoch = epoch + UNIX_OFFSET;
		return (epoch);
	#else
		return (epoch);
	#endif
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

	//Get h_mode here to avoid multiple I2C reads
	h_mode = getHourMode();

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(R_SECONDS);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 3);

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
    uint8_t day, month;
	uint16_t year;
	String dateString;

	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(R_DATE);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 7);

	day = Wire.read();
	day = bcd2bin(day);
	
	month = Wire.read();
 	month  = bcd2bin(month);

	year = Wire.read();
	year = bcd2bin(year) + 2000;

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

String DS1307::getWeekString()
{
	uint8_t week;
	String weekString;

	week = _read_one_register(R_WEEKDAY);
	
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


/* NVRAM Functions */

bool NVRAM::begin()
{
	Wire.begin(); // join i2c bus
	Wire.beginTransmission(DS1307_ADDR);
	return (Wire.endTransmission() == 0 ? true : false);
}

uint8_t NVRAM::read(uint8_t address)
{
	uint8_t data = 0x00;
	if(address >= 1 or address <=56 )
	{
		address = (address % length()) + R_NVRAM;
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
		address = address + R_NVRAM;
		Wire.beginTransmission(DS1307_ADDR);
		Wire.write(address);
		Wire.write(data);
		Wire.endTransmission();
	}
}

void NVRAM::clear()
{
	{ //clear DS1397 RAM with 56 bytes = 7 chunks a 8 bytes
		for (byte chunk=0;chunk<7;chunk++) 
		{
			Wire.beginTransmission(DS1307_ADDR);
			Wire.write(8+chunk*8); // set register pointer to 8 + Offset
			for (byte i=0; i<8; i++) 
				Wire.write(0xFF); // zero out RAM address
			Wire.endTransmission();    
		}
	}

	// Wire.beginTransmission(DS1307_ADDR);
	// Wire.write(R_NVRAM);

	// for (uint8_t address = 0; address < 56; address++)
	// 		Wire.write(0xAA);
	// Wire.endTransmission();
}

uint16_t NVRAM::getCRC16()
{
	uint16_t crc=0; // starting value as you like, must be the same before each calculation
	for (int i=0;i<_length;i++) // for each character in the string
	{
		crc= _crc16_update (crc, read(i)); // update the crc value
	}
	return crc;

	// uint32_t crc = 0L;
	// const uint32_t crc_table[16] = {
	// 0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
	// 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
	// 0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
	// 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
	// };

	// for (int index = 0 ; index < _length  ; ++index)
    // {
    //     crc = crc_table[(crc ^ read(index)) & 0x0f] ^ (crc >> 4);
    //     crc = crc_table[(crc ^ (read(index) >> 4)) & 0x0f] ^ (crc >> 4);
    //     crc = ~crc;
    // }

	// return crc;
    
}

/*Operator [] Read function*/
uint8_t NVRAM::operator[](uint8_t address) const
{
	uint8_t data = 0x00;
	if(address >= 1 or address <=56 )
	{
		//Serial.print("address = ");
		//Serial.println(address);
		address = (address % _length) + R_NVRAM;
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

uint8_t DS1307::_calculateDayOfWeek(uint8_t day, uint8_t month, uint16_t year)
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

uint16_t NVRAM::_crc16_update(uint16_t crc, uint8_t a)
{
	int i;
	crc ^= a;
	for (i = 0; i < 8; ++i)
	{
		if (crc & 1)
			crc = (crc >> 1) ^ 0xA001;
		else
			crc = (crc >> 1);
	}
	return crc;
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