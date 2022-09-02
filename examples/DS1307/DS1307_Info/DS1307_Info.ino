#include <Wire.h>
#include <I2C_RTC.h>

static DS1307 RTC;

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  RTC.begin();

  Serial.println();
  Serial.println("*** RTC 1307 ***");
  Serial.print("Is Clock Running : ");
  if (RTC.isRunning())
    Serial.println("Yes");
  else
    Serial.println("No. Time may be Inaccurate");
  Serial.print("Hour Mode : ");
  if (RTC.getHourMode() == CLOCK_H24)
    Serial.println("24 Hours");
  else
    Serial.println("12 Hours");
  Serial.print("Is Out Pin Enabled : ");
  if (RTC.isOutPinEnabled())
    Serial.println("Yes");
  else
    Serial.println("No");

  Serial.print("Is SQWE Enabled : ");
  if (RTC.isSqweEnabled())
    Serial.println("Yes");
  else
    Serial.println("No");

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
