#include <Wire.h>
#include <I2C_RTC.h>

static PCF8523 RTC;

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial port to connect. Needed for native USB port only
    RTC.begin();
    Serial.println();

    if(RTC.isConnected() == false)
    {
        Serial.println("RTC Not Connected!");
        while(true);
    }

	RTC.setHourMode(CLOCK_H24);
	RTC.setTime(3,40,40);
	RTC.startClock();

	RTC.clearAlarm();
	//setAlarm(uint8_t minutes,uint8_t hours, uint8_t days, uint8_t weekday)
	RTC.setAlarm(41,3, -1, -1);

	

	Serial.println("*** RTC PCF8523 Alarm Example ***");
	Serial.print("Is Clock Running : ");
	if (RTC.isRunning())
		Serial.println("Yes");
	else
		Serial.println("No");

	Serial.print("Is Alarm Enabled : ");
	if (RTC.isAlarmEnabled())
		Serial.println("Yes");
	else
		Serial.println("No");

	Serial.print("Is Alarm Triggered : ");
	if (RTC.isAlarmTriggered())
		Serial.println("Yes");
	else
		Serial.println("No");

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
		Serial.print(" Alarm Triggered? : ");

		if(RTC.isAlarmTriggered())
			Serial.println("Yes");
		else
			Serial.println("No");
	}
	delay(1000);
}

//  Serial.print(RTC.getDateTimeString());
