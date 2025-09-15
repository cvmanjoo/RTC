#include <Wire.h>
#include <I2C_RTC.h>

static PCF8523 RTC;

struct tm rtc_time, get_rtc_time;

void setup()
{
	Serial.begin(115200);
    while (!Serial); // wait for serial port to connect. Needed for native USB port only
	
	RTC.begin();

	if(RTC.isConnected() == false)
	{
		Serial.println("RTC Not Connected!");
		while(true);
	}

	Serial.println("----------------------------");
	//SUN 16-00-2026 02:20:00

	

	// Sets the RTC with the date and time provided in the tm structure
	// Note: tm_year is years since 1900, so we need to add 1900 to get the full year

	//RTC.setHourMode(CLOCK_H12);
	RTC.setHourMode(CLOCK_H24);

	rtc_time.tm_sec = 0;	// seconds after the minute - [0, 60] including leap second
	rtc_time.tm_min = 0; 	// hours since midnight - [0, 23]
	rtc_time.tm_hour = 13;	// minutes after the hour - [0, 59]

	rtc_time.tm_mday = 4;		// day of the month - [1, 31]
	rtc_time.tm_mon = 9 - 1;  // months since January - [0, 11]
	rtc_time.tm_year = 2025 - 1900; // years since 1900
	


	mktime(&rtc_time);

	RTC.setDateTime(rtc_time);

	get_rtc_time = RTC.getDateTime();
	Serial.print(" ");
	Serial.print(asctime(&get_rtc_time));

	//Serial.print(RTC.getDateTimeString());
}

void loop() 
{
	
}
