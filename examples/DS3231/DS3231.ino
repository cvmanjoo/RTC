#include <Wire.h>
#include <DS3231.h>

float temp;
int data;
// DS3231 RTC;

void setup()
{
    Serial.begin(9600);
    RTC.begin();

    Serial.println();

    temp = RTC.getTemp();
    Serial.print("RTC Temperature : ");
    Serial.println(temp);

    /*Serial.print("Lost Power : ");
    if (RTC.lostPower())
    {
        Serial.println("Yes");
        RTC.setDay(31);
        RTC.setMonth(12);
        RTC.setYear(2014);
        RTC.setHour(23);
        RTC.setMinute(59);
        RTC.setSecond(46);
        
        RTC.setEpoch(0,0,0);
        RTC.StartClock();
    }
    else
        Serial.println("No");
*/
    RTC.setTime(9,39,52);
    RTC.setDate(9,12,2017);

  //RTC.setEpoch(3710945875,NTP_OFFSET,0);
  //RTC.setEpoch(0,0,0);
  
  //RTC.StartClock();

RTC.setAlarm2(40,9,9);


}

void loop()
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

    if(RTC.isAlarm2Tiggered())
    
        Serial.println("isAlarm2Tiggered : Yes ");
    
    else
        Serial.println("isAlarm2Tiggered : No ");
}