#include <Wire.h>
#include <I2C_RTC.h>

static NVRAM NVRAM;

uint8_t address = 0,data = 0;

void setup()
{
    Serial.begin(9600);

    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    if(NVRAM.begin() == false)
    {
        Serial.println("RTC Not Connected!");
        while(true);
    }

    for (address = 0; address < NVRAM.length(); address ++)
    {
      NVRAM.write(address, 0xFF);
    }
    Serial.println("\nNVRAM Cleared!");
}
void loop()
{

}

