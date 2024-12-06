/* ------------------------------------------------------------
 * "THE BEERWARE LICENSE" (Revision 42):
 * <hi@cvmanjoo.in> wrote this code. As long as you retain this
 * notice, you can do whatever you want with this stuff. If we
 * meet someday, and you think this stuff is worth it, you can
 * buy me a beer in return.
 * ------------------------------------------------------------
 * DS1307_Manual.ino
 * Created by Manjunath CV. 
 * Released into the public domain.
 * ----------------------------------------------------------*/

#include <Wire.h>
#include <I2C_RTC.h>

static DS1307 RTC;

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

	/* Example to Set all the varibles individually */

	RTC.setHourMode(CLOCK_H12);
	//RTC.setHourMode(CLOCK_H24);

	RTC.setDay(13);
	RTC.setMonth(05);
	RTC.setYear(2020);

	RTC.setHours(9);
	RTC.setMinutes(47);
	RTC.setSeconds(56);

	if (RTC.getHourMode() == CLOCK_H12)
		RTC.setMeridiem(HOUR_PM);

	RTC.setWeek(5); //1 for sunday - 7 for Saturday

	Serial.println(RTC.getDateTimeString());

	/* Example to Set Time and Date Separately */

	RTC.setDate(22,07,21);
	RTC.setTime(23,00,00);

	Serial.println(RTC.getDateTimeString());

	RTC.startClock();
}

void loop()
{
	 /* Example to Get all the varibles
	 indivisually and Display them */
	switch (RTC.getWeek())
	{
	case 1:
		Serial.print("SUN");
		break;
	case 2:
		Serial.print("MON");
		break;
	case 3:
		Serial.print("TUE");
		break;
	case 4:
		Serial.print("WED");
		break;
	case 5:
		Serial.print("THU");
		break;
	case 6:
		Serial.print("FRI");
		break;
	case 7:
		Serial.print("SAT");
		break;
	}
	Serial.print(" ");
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
	Serial.print(" ");

	if (RTC.getHourMode() == CLOCK_H12)
	{
		switch (RTC.getMeridiem())
		{
		  case HOUR_AM :
			Serial.print(" AM");
			break;
		  case HOUR_PM :
			Serial.print(" PM");
			break;
		}
	}
	Serial.println("");
	delay(10000);
}