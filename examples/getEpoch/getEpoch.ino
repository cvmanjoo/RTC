#include <Wire.h>
#include <DS1307.h>
//#include <DS3231.h>
//#include <PCF8563.h>

time_t epoch;

void setup()
{
    Serial.begin(9600);
    RTC.begin();    
}

void loop()
{
    epoch = RTC.getEpoch();
    
    Serial.print("2000 Epoch : ");
    Serial.println(epoch);
    Serial.println(isotime(gmtime(&epoch)));
    Serial.println(asctime(localtime(&epoch)));  
    Serial.println();
    delay(1000);
}
