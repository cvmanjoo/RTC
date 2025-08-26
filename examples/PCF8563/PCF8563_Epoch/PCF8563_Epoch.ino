#include <Wire.h>
#include <I2C_RTC.h>

static PCF8563 RTC;

void setup()
{
  Serial.begin(115200);
  RTC.begin();

  //RTC.setEpoch(0);
}

void loop()
{
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
  Serial.println(" ");

  delay(1000);
}
