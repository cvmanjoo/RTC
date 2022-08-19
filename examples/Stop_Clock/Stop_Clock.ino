#include <Wire.h>
#include <I2C_RTC.h>

//static DS1307 RTC;
static DS3231 RTC;
//static PCF8563 RTC;
//static PCF8523 RTC;


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
