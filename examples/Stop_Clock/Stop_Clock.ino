#include <Wire.h>
#include <I2C_RTC.h>

//Uncomment your RTC.

//static DS1307 RTC;
//static DS3231 RTC;
//static PCF8563 RTC;
static PCF8523 RTC;


void setup()
{
	Serial.begin(115200);
	while (!Serial); // wait for serial port to connect. Needed for native USB

	RTC.begin();

	RTC.stopClock();
	Serial.println("Clock Stopped! ");

	Serial.print("Is Clock Running : ");
	if (RTC.isRunning())
		Serial.println("Yes");
	else
		Serial.println("No");

}

void loop()
{
	//Nothing as usual
}
