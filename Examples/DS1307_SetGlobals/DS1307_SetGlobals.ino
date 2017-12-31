#include <Wire.h>
#include <DS1307.h>

float temp;

void setup()
{
    Serial.begin(9600);
    RTC.begin();

    Serial.println();

    Serial.print("Clock Halt : ");
    if (RTC.isClockHalt())
    {
        Serial.println("Yes");
        RTC.setDateTime(__DATE__, __TIME__);
        //RTC.StartClock();
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
