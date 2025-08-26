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

static DS3231 RTC;

void setup()
{
    Serial.begin(115200);
    RTC.begin();

    RTC.setHourMode(CLOCK_H12);
    //RTC.setHourMode(CLOCK_H24);

    if (RTC.getHourMode() == CLOCK_H12)
    {
        //RTC.setMeridiem(HOUR_PM);
    }

    RTC.setDay(22);
    RTC.setMonth(5);
    RTC.setYear(2020);

    RTC.setHours(23);
    RTC.setMinutes(47);
    RTC.setSeconds(56);

    //RTC.setWeek(1); // To update the week manually.

	RTC.updateWeek(); // updates the week automatically based on date in the RTC.

    //RTC.setDate(22,07,29);
    //RTC.setTime(22,10,20);

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
