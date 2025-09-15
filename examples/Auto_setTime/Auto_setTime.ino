/* ------------------------------------------------------------
 * "THE BEERWARE LICENSE" (Revision 42):
 * <hi@cvmanjoo.in> wrote this code. As long as you retain this
 * notice, you can do whatever you want with this stuff. If we
 * meet someday, and you think this stuff is worth it, you can
 * buy me a beer in return.
 * ------------------------------------------------------------
 * Auto_setTime.ino
 * Created by Manjunath CV. 
 * Released into the public domain.
 * ----------------------------------------------------------*/

#include <Wire.h>
#include <I2C_RTC.h>

//static DS1307 RTC;
//static DS3231 RTC;
//static DS3232 RTC;
static PCF8523 RTC;
//static PCF8563 RTC;
//static MCP7940 RTC;

int hours,minutes,seconds,day,month,year;

void setup()
{
	Serial.begin(115200);
	while (!Serial); // wait for serial port to connect. Needed for native USB
    RTC.begin();

	if(RTC.isConnected() == false)
	{
		Serial.println("RTC Not Connected!");
		while(true);
	}
		
	Serial.print("Is Clock Running: ");
	if(RTC.isRunning())
	{
		Serial.println("Yes");
		while(true)
		{
			switch (RTC.getWeek())
			{
				case 1: Serial.print("SUN");
				break;
				case 2: Serial.print("MON");
				break;
				case 3: Serial.print("TUE");
				break;
				case 4: Serial.print("WED");
				break;
				case 5: Serial.print("THU");
				break;
				case 6: Serial.print("FRI");
				break;
				case 7: Serial.print("SAT");
				break;
			}
			Serial.print(" ");
			
			day=RTC.getDay();
			month=RTC.getMonth();
			year=RTC.getYear();

			if(day<10)
				Serial.print("0");
			Serial.print(RTC.getDay());
			Serial.print("-");
			if(month<10)
				Serial.print("0");
			Serial.print(RTC.getMonth());
			Serial.print("-");
			Serial.print(RTC.getYear());
			Serial.print(" ");
			
			hours = RTC.getHours();
			minutes = RTC.getMinutes(); 
			seconds = RTC.getSeconds();

			if(hours<10)
				Serial.print("0");
			Serial.print(hours);
			Serial.print(":");
			if(minutes<10)
				Serial.print("0");
			Serial.print(minutes);
			Serial.print(":");
			if(seconds<10)
				Serial.print("0");
			Serial.print(seconds);
			Serial.print("");
			if (RTC.getHourMode() == CLOCK_H12)
			{
				switch (RTC.getMeridiem())
				{
					case HOUR_AM:
							Serial.print(" AM");
							break;
					case HOUR_PM:
							Serial.print(" PM");
							break;
				}   
			}
			Serial.println();
			delay(1000);
		}
	}
	else
	{
		delay(250);

		Serial.println("No");
		Serial.println("Setting Time");

		RTC.setHourMode(CLOCK_H12); //Comment if RTC PCF8563
		//RTC.setHourMode(CLOCK_H24);  

		RTC.setDateTime(__TIMESTAMP__);
        Serial.println(__TIMESTAMP__);
        //RTC.setDateTime("Fri Mar 08 00:01:53 2024");

		// RTC.setDateTime(__DATE__, __TIME__);
		// Serial.print(__DATE__);
		// Serial.print(" ");
		// Serial.println(__TIME__);

		// RTC.setDateTime("Dec 26 2009", "00:34:56");

		Serial.println("New Time Set, Reset Manually ");

		RTC.startClock(); //Start the Clock;
        Serial.println();
	}
}

void loop()
{

}