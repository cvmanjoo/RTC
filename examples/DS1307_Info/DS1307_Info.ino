#include <Wire.h>
#include <DS1307.h>

float temp;

void setup()
{
    Serial.begin(9600);
    RTC.begin();
    RTC.setHourMode(CLOCK_H12);

    Serial.println();

    Serial.print("Is Clock Running : ");
    if (RTC.isRunning())
            Serial.println("Yes");
    else
        Serial.println("No");

    
    Serial.print("Hour Mode : ");
    if (RTC.getHourMode() == CLOCK_H24)
        Serial.println("24 Hours");
    else
        Serial.println("12 Hours");
}

void loop()
{
    Serial.print(RTC.getDay());
    Serial.print("-");
    Serial.print(RTC.getMonth());
    Serial.print("-");
    Serial.print(RTC.getYear());

    Serial.print(" ");

    Serial.print(RTC.getHour());
    Serial.print(":");
    Serial.print(RTC.getMinute());
    Serial.print(":");
    Serial.print(RTC.getSecond());
    Serial.println(" ");

    delay(1000);
}
