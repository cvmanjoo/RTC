#include <Wire.h>
#include <I2C_RTC.h>

static DS3231 RTC;

void setup()
{
    Serial.begin(115200);
    RTC.begin();

    //RTC.setHourMode(CLOCK_H24);
    RTC.setHourMode(CLOCK_H12);

    //RTC.setEpoch(1622904335);
}

void loop()
{
    Serial.println(); 
    Serial.print("UnixTime : ");
    Serial.println(RTC.getEpoch()); 

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
        if(RTC.getMeridiem() == HOUR_AM)
        Serial.println(" AM");
        if (RTC.getMeridiem() == HOUR_PM)
        Serial.println(" PM");     
    }

    delay(1000);
}
