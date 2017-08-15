#include <Wire.h>
#include <DS3231.h>

float temp;

void setup()
{
    Serial.begin(9600);
    RTC.begin();

    Serial.println();

    temp = RTC.getTemp();
    Serial.print("RTC Temperature : ");
    Serial.println(temp);

    Serial.print("Lost Power : ");
    if (RTC.lostPower())
    {
        Serial.println("Yes");
        RTC.setDate(12,8,2017);
        RTC.setTime(20,35,00);
        RTC.StartClock();
    }
    else
        Serial.println("No");   
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
