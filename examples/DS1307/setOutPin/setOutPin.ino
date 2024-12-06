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
    Serial.begin(115200);
    while (!Serial); // wait for serial port to connect. Needed for native USB port only
    RTC.begin();
    if(RTC.isConnected() == false)
    {
        Serial.println("RTC Not Connected!");
        while(true);
    }

    RTC.setOutPin(LOW);
    Serial.print("Done.");
   
}

void loop()
{
}
