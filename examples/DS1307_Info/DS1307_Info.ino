#include <Wire.h>
#include <DS1307.h>

void setup()
{
    Serial.begin(9600);
    RTC.begin();
    
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
      switch(RTC.getMeridiem())
      {
        case HOUR_AM :
          Serial.print(" AM");
          break;
        case HOUR_PM :
          Serial.print(" PM");
          break;
      }
    }
    Serial.println("");
    Serial.print("Epoch Time : ");
    Serial.println(RTC.getEpoch());

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
    
}
