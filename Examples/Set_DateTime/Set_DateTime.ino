#include <Wire.h>
#include <DS1307.h>

//#include <DS3231.h>

void setup()
{
    Serial.begin(9600);
    RTC.begin();

    Serial.print("Is Clock Running: ");
    if (RTC.isRunning())
    {
        Serial.println("Yes");
        while(true)
        {
          Serial.print(RTC.getDay());
          Serial.print("-");
          Serial.print(RTC.getMonth());
          Serial.print("-");
          Serial.print(RTC.getYear());
          
          Serial.print(" ");
          
          Serial.print(RTC.getHour());
          Serial.print(":");
          Serial.print(RTC.getMinute());
          Serial.print(":");
          Serial.print(RTC.getSecond());
          Serial.println(" ");
          delay(1000);
         
        }        
    }
    else
    {
        Serial.println("No");
        RTC.setDateTime(__DATE__, __TIME__);
        Serial.println("New Time Set");
        Serial.print(__DATE__);
        Serial.print(" ");
        Serial.println(__TIME__);
        //RTC.startClock(); //for DS3231
    }
}

void loop()
{
    
}
