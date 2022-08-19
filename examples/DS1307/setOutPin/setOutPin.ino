#include <Wire.h>
#include <I2C_RTC.h>

/*
    LOW       0
    HIGH      1
    SQW001Hz  1Hz
    SQW04kHz  4.096kHz
    SQW08kHz  8.192kHz
    SQW32kHz  32.768kHz 
*/

static DS1307 RTC;

void setup()
{
    Serial.begin(9600);
    RTC.begin();

    RTC.setOutPin(SQW001Hz);
    Serial.print("Done.");
   
}

void loop()
{
}
