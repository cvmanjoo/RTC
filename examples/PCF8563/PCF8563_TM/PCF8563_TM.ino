#include <Wire.h>
#include <I2C_RTC.h>

static PCF8563 RTC;

struct tm rtc_time;

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


	//SUN 16-00-2026 02:20:00

	rtc_time.tm_mday = 16;
	rtc_time.tm_mon = 9;  // Starts from 0-11
	rtc_time.tm_year = 2026;

	rtc_time.tm_hour = 2;	
	rtc_time.tm_min = 20;
	rtc_time.tm_sec = 36;


	RTC.setDateTime(rtc_time);

	Serial.println(RTC.getDateTimeString());

	delay(1000);
	
	Serial.print(RTC.getWeekString().substring(0,3));
	Serial.print(" ");
	Serial.print(RTC.getDateString());
	Serial.print(" ");
	Serial.println(RTC.getTimeString());
	
}

void loop() 
{
;

	delay(2000);
}
