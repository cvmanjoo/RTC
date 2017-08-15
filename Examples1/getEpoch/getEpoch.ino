#include <Wire.h>
#include <DS3231.h>

float temp;
int data;
time_t epoch;

void setup()
{
    Serial.begin(9600);
    RTC.begin();

    Serial.println();

    temp = RTC.getTemp();
    Serial.print("RTC Temperature : ");
    Serial.println(temp);

    Serial.print("Lost Power : ");
    if (RTC.lostPower())
        Serial.println("Yes");
    else
        Serial.println("No");   
}

void loop()
{
    epoch = RTC.getEpoch();
    
    Serial.print("2000 Epoch : ");
    Serial.println(epoch);
    Serial.println(isotime(gmtime(&epoch)));
    Serial.println(asctime(localtime(&epoch)));  
    delay(1000);
}