/* Still in Progress, May not work as expected! */

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

	Serial.println("*** RTC PCF8523 Timer A Example ***");

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

	RTC.setTimerA(255);


}

void loop()
{
	// if(RTC.isRunning())
	// {

	Serial.print("Timer Value : ");
	Serial.print(RTC.getTimerA());
	
	Serial.print(" Is Timmer A Triggered : ");
	if(RTC.isTimerATriggered())
		Serial.println("Yes");
	else
		Serial.println("No");
	//}
	delay(1000);
}

//  Serial.print(RTC.getDateTimeString());
