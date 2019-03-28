#include <Wire.h>
#include <RTC.h>

static DS3231 RTC;

float temp;

void setup()
{
    Serial.begin(9600);
    RTC.begin();

    Serial.println();

	temp = RTC.getTemp();
    Serial.print("RTC Temperature : ");
    Serial.println(temp);

    Serial.print("Is Clock Running : ");
    if (RTC.isRunning())
    {
        Serial.println("Yes");
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
