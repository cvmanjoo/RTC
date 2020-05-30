#include <Wire.h>
#include <RTC.h>

static DS3231 RTC;
//static DS1307 RTC;

void setup()
{
  Serial.begin(9600);
  RTC.begin();

  RTC.stopClock();
  Serial.println("Clock Stopped! ");

  Serial.print("Is Clock Running : ");
  if (RTC.isRunning())
    Serial.println("Yes");
  else
    Serial.println("No");

}

void loop() {}
