#include <Wire.h>
#include <I2C_RTC.h>

static DS1307 RTC;

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial port to connect. Needed for native USB port only
    RTC.begin();
    Serial.println();

    if(RTC.isConnected() == false)
    {
        Serial.println("RTC Not Connected!");
        while(true);
    }

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
    Serial.print(RTC.getDateTimeString());
    Serial.print(", Unix Time: ");
    Serial.println(RTC.getEpoch());
    delay(5000);
}
