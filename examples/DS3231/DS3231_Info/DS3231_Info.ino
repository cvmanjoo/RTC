#include <Wire.h>
#include <I2C_RTC.h>

static DS3231 RTC;

void setup()
{
	Serial.begin(115200);
	while (!Serial); // wait for serial port to connect. Needed for native USB
	RTC.begin(); //Not required if you are calling isConnected() function before calling other RTC functions

	Serial.println();

	if(RTC.isConnected() == false)
    {
        Serial.println("RTC Not Connected!");
        while(true);
    }

	Serial.println("*** RTC DS3231 ***");
	Serial.print("Is Clock Running : ");
	if(RTC.isRunning())
		Serial.println("Yes");
	else
		Serial.println("No. Time may be Inaccurate");

	Serial.print("Hour Mode : ");
	if (RTC.getHourMode() == CLOCK_H12)
		Serial.println("12 Hours");
	else
		Serial.println("24 Hours");

	Serial.print("Alarm1 Enabled : ");
	if (RTC.isAlarm1Enabled())
		Serial.println("Yes");
	else
	Serial.println("No");

	Serial.print("Alarm2 Enabled : ");
	if (RTC.isAlarm2Enabled())
		Serial.println("Yes");
	else
		Serial.println("No");

	Serial.print("Alarm1 Tiggered : ");
	if (RTC.isAlarm1Tiggered())
		Serial.println("Yes");
	else
		Serial.println("No");

	Serial.print("Alarm2 Tiggered : ");
	if (RTC.isAlarm2Tiggered())
		Serial.println("Yes");
	else
		Serial.println("No");

	Serial.print("INT/SQW Pin Mode : ");
	if (RTC.getINTPinMode())
		Serial.println("Alarm");
	else
		Serial.println("SQW");

	//Aging Offset Value
	Serial.print("Aging Offset : "); 
	Serial.print(RTC.getAgingOffset());
	Serial.println();

	Serial.print("RTC Temperature : "); 
	Serial.print(RTC.getTemp());
	Serial.print("'C");
	Serial.println();
	Serial.println();
}

void loop()
{       
	if(RTC.isRunning())
	{
		Serial.print(RTC.getWeekString().substring(0, 3));
		Serial.print(" ");
		Serial.print(RTC.getDateString());
		Serial.print(" ");
		Serial.print(RTC.getTimeString());
		Serial.println();
	}
	delay(1000);
}
