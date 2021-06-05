
#include <Wire.h>
#include <RTC.h>

static DS3231 RTC;

void setup()
{
	Serial.begin(9600);
	RTC.begin();

	//RTC.stopClock();

	Serial.print("Is Clock Running: ");
	if (RTC.isRunning())
	{
		Serial.println("Yes");
		Serial.print(RTC.getDay());
		Serial.print("-");
		Serial.print(RTC.getMonth());
		Serial.print("-");
		Serial.print(RTC.getYear());
		Serial.print(" ");
		Serial.print(RTC.getHours());
		Serial.print(":");
		Serial.print(RTC.getMinutes());
		Serial.print(":");
		Serial.print(RTC.getSeconds());
		Serial.print("");
		if (RTC.getHourMode() == CLOCK_H12)
		{
			switch (RTC.getMeridiem()) {
			case HOUR_AM:
				Serial.print(" AM");
				break;
			case HOUR_PM:
				Serial.print(" PM");
				break;
			}
		}
		Serial.println("");
	delay(1000);
	}
	else
	{
		delay(200);

		Serial.println("No");
		Serial.println("Setting Time");
		RTC.setHourMode(CLOCK_H12);
		//RTC.setHourMode(CLOCK_H24);
		RTC.setDateTime(__DATE__, __TIME__);
		Serial.println("New Time Set");
		Serial.print(__DATE__);
		Serial.print(" ");
		Serial.println(__TIME__);
		RTC.startClock();
	}
}

void loop()
{

}
