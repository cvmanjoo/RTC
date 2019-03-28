#include <Wire.h>
#include <RTC.h>

static DS1307 RTC;
static DS3231 RTC;
static PCF8563 RTC;

time_t epoch;

void setup()
{
    Serial.begin(9600);
    RTC.begin();
}

void loop()
{
    epoch = RTC.getEpoch();

    Serial.print("2000 Epoch : ");
    Serial.println(epoch);
    Serial.println(isotime(gmtime(&epoch)));
    Serial.println(asctime(localtime(&epoch)));
    Serial.println();
    delay(1000);
}
