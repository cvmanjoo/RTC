/* ------------------------------------------------------------
 * "THE BEERWARE LICENSE" (Revision 42):
 * <hi@cvmanjoo.in> wrote this code. As long as you retain this
 * notice, you can do whatever you want with this stuff. If we
 * meet someday, and you think this stuff is worth it, you can
 * buy me a beer in return.
 * ------------------------------------------------------------
 * Get_Set_Epoch.ino
 * Created by Manjunath CV. 
 * Released into the public domain.
 * ------------------------------------------------------------
 * Epoch Converter -> https://www.epochconverter.com/
 * Points to be noted while setting Epoch
 *   1. Epoch Should be always greater than 946684800 Which is
 *  	01-01-2000. This is a limitation of DS1307 chip itself
 *   2. Tested on ESP32 works fine, AVR based chips might not
 *  	work properly.
 * ----------------------------------------------------------*/

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

    RTC.setHourMode(CLOCK_H12);

    // RTC.setEpoch(946684801); //SAT 01-01-2000 12:00:00 AM
    // Serial.print(RTC.getEpoch());
    // Serial.print(" - ");
    // Serial.println(RTC.getDateTimeString());

    RTC.setEpoch(1728674817); //FRI 11-10-2024 07:26:57 PM
    Serial.print(RTC.getEpoch());
    Serial.print(" - ");
    Serial.println(RTC.getDateTimeString());

	RTC.setEpoch(1760486280); //TUE 14-10-2025 11:58:00 PM
    Serial.print(RTC.getEpoch());
    Serial.print(" - ");
    Serial.println(RTC.getDateTimeString());

    // RTC.setEpoch(4102444799); //THU 31-12-2099 11:59:59 PM
    // Serial.print(RTC.getEpoch());
    // Serial.print(" - ");
    // Serial.println(RTC.getDateTimeString());

    // RTC.setDateTime(__TIMESTAMP__);
    // Serial.print(RTC.getEpoch());
    // Serial.print(" - ");
    // Serial.println(RTC.getDateTimeString());
    
    RTC.startClock();
    //RTC.stopClock();

}

void loop()
{

}