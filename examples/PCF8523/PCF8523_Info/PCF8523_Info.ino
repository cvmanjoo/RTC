#include <Wire.h>
#include <I2C_RTC.h>

static PCF8523 RTC;

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  RTC.begin();

  Serial.println();
    Serial.println("*** RTC PCF8523 ***");
  Serial.print("Is Clock Running : ");
  if (RTC.isRunning())
    Serial.println("Yes");
  else
    Serial.println("No");
/*
  Serial.print("Battery Status : ");
  if (RTC.isBatteryLow())
    Serial.println("Low");
  else
    Serial.println("Good");
*/
//RTC.set    
    
}

void loop()
{

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
