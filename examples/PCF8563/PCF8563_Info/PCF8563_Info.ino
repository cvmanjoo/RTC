#include <Wire.h>
#include <RTC.h>

static PCF8563 RTC;

void setup()
{
  Serial.begin(9600);
  RTC.begin();

  Serial.println();
    Serial.println("*** RTC PCF8563 ***");
  Serial.print("Is Clock Running : ");
  if (RTC.isRunning())
    Serial.println("Yes");
  else
    Serial.println("No");

  Serial.print("Alarm Enabled  : ");
  if (RTC.isAlarmEnabled())
    Serial.println("Yes");
  else
    Serial.println("No");
    
  Serial.print("Alarm Triggered : ");
  if (RTC.isAlarmTriggered())
    Serial.println("Yes");
  else
    Serial.println("No");

   Serial.print("Timer Enabled  : ");
  if (RTC.isTimerEnabled())
    Serial.println("Yes");
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

  Serial.print(RTC.getHours());
  Serial.print(":");
  Serial.print(RTC.getMinutes());
  Serial.print(":");
  Serial.print(RTC.getSeconds());
  Serial.println(" ");

  delay(1000);
}
