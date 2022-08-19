#include <Wire.h>
#include <I2C_RTC.h>

static DS1307 RTC;

void setup()
{
  Serial.begin(9600);
  RTC.begin();

  RTC.setHourMode(CLOCK_H12);
  //RTC.setHourMode(CLOCK_H24);

  if (RTC.getHourMode() == CLOCK_H12)
  {
     RTC.setMeridiem(HOUR_PM);
  }
/*
  RTC.setDay(13);
  RTC.setMonth(05);
  RTC.setYear(2020);

  RTC.setHours(9);
  RTC.setMinutes(47);
  RTC.setSeconds(56);
*/
  RTC.setWeek(1);

  RTC.setDate(22,07,21);
  RTC.setTime(23,00,00);
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
  Serial.print(" ");

  if (RTC.getHourMode() == CLOCK_H12)
  {
    switch (RTC.getMeridiem()) {
      case HOUR_AM :
        Serial.print(" AM");
        break;
      case HOUR_PM :
        Serial.print(" PM");
        break;
    }
  }
  Serial.println("");
  delay(1000);
}
