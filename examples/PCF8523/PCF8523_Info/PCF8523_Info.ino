#include <Wire.h>
#include <I2C_RTC.h>

static PCF8523 RTC;

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial port to connect. Needed for native USB port only
    RTC.begin();

	//RTC.softReset(); Re

    Serial.println();

    if(RTC.isConnected() == false)
    {
        Serial.println("RTC Not Connected!");
        while(true);
    }

	Serial.println("*** RTC PCF8523 ***");
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

	Serial.print("Is Timer A Enabled : ");
	if (RTC.isTimerAEnabled())
		Serial.println("Yes");
	else
		Serial.println("No");

	Serial.print("Is Timmer A Triggered : ");
	if (RTC.isTimerATriggered())
		Serial.println("Yes");
	else
		Serial.println("No");




	Serial.print("Battery Status : ");
	if (RTC.isBatteryLow())
	Serial.println("Low");
	else
	Serial.println("Good");
 

}

void loop()
{
	if(RTC.isRunning())
	{	
		// Serial.print(RTC.getDay());
		// Serial.print("-");
		// Serial.print(RTC.getMonth());
		// Serial.print("-");
		// Serial.print(RTC.getYear());

		// Serial.print(" ");

		// Serial.print(RTC.getHours());
		// Serial.print(":");
		// Serial.print(RTC.getMinutes());
		// Serial.print(":");
		// Serial.print(RTC.getSeconds());
		// Serial.println(" ");

		Serial.print(RTC.getWeekString().substring(0, 3));
		Serial.print(" ");
		Serial.print(RTC.getDateString());
		Serial.print(" ");
		Serial.print(RTC.getTimeString());
		Serial.println();
	}
	delay(1000);
}

//  Serial.print(RTC.getDateTimeString());
