#include <Wire.h>
#include <I2C_RTC.h>

static DS3231 RTC;

void setup()
{
  Serial.begin(9600);
  RTC.begin();
  
  DateTime A1, A2;
  
  Serial.println();
  Serial.println("*** RTC DS3231 ***");
  Serial.print("Is Clock Running : ");
  if (RTC.isRunning())
    Serial.println("Yes");
  else
    Serial.println("No. Time may be Inaccurate");
  Serial.print("Hour Mode :");
  if (RTC.getHourMode() == CLOCK_H12)
    Serial.println("12 Hours");
  else
    Serial.println("24 Hours");

  //Alarms1

  A1 = RTC.getAlarm1();

  Serial.print("Alarm1 Time : ");
  Serial.print(A1.hours);
  Serial.print(":");
  Serial.print(A1.minutes);
  Serial.print(":");
  Serial.print(A1.seconds);
  if (RTC.getHourMode() == CLOCK_H12)
  {
    switch (RTC.getMeridiem())
    {
      case HOUR_AM :
        Serial.print(" AM");
        break;
      case HOUR_PM :
        Serial.print(" PM");
        break;
    }
  }
  Serial.println();

  //Alarms Trigger Status

  Serial.print("Alarm1 Tiggered : ");
  if (RTC.isAlarm1Tiggered())
    Serial.println("Yes");
  else
    Serial.println("No");

  Serial.print("Alarm2 Tiggered : ");
  if (RTC.isAlarm2Tiggered())
    Serial.println("Yes");
  else
    Serial.println("No");
  
  Serial.print("INT/SQW Pin Mode : ");
  if (RTC.getINTPinMode())
    Serial.println("Alarm");
  else
    Serial.println("SQW");

  //Aging Offset Value
  Serial.print("Aging Offset : "); 
  Serial.print(RTC.getAgingOffset());
  Serial.println();
  
  Serial.print("RTC Temperature : "); 
  Serial.print(RTC.getTemp());
  Serial.print("'C");
  Serial.println();
  Serial.println();
}

void loop()
{

  switch (RTC.getWeek())
  {
    case 1:
      Serial.print("SUN");
      break;
    case 2:
      Serial.print("MON");
      break;
    case 3:
      Serial.print("TUE");
      break;
    case 4:
      Serial.print("WED");
      break;
    case 5:
      Serial.print("THU");
      break;
    case 6:
      Serial.print("FRI");
      break;
    case 7:
      Serial.print("SAT");
      break;
  }
  Serial.print(" ");
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
  if (RTC.getHourMode() == CLOCK_H12)
  {
    switch (RTC.getMeridiem())
    {
      case HOUR_AM :
        Serial.print(" AM");
        break;
      case HOUR_PM :
        Serial.print(" PM");
        break;
    }
  }
  Serial.println();
  delay(1000);
}
