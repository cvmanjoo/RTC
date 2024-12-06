#include <Wire.h>
#include <I2C_RTC.h>

static NVRAM NVRAM;

uint8_t address = 0, data = 0;

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial port to connect. Needed for native USB port only

    if(NVRAM.begin() == false)
    {
        Serial.println("RTC Not Connected!");
        while(true);
    }

    Serial.println("NVRAM Contents\n");

    Serial.print("    ");
    for (int i = 0; i<=15; i++)
    {
        Serial.print(" ");
        Serial.print(i ,HEX);
        Serial.print(" ");
    }
    
    Serial.println();
    Serial.print("00: ");

    for (address = 0; address < NVRAM.length(); address++)
    {
        
        data = NVRAM.read(address);
        if(data <= 0x0F)
            Serial.print("0");
        
        Serial.print(data, HEX);
        Serial.print(" ");

        if((address % 16) == 15)
        {
            Serial.println();
            Serial.print(address+1,HEX);
            Serial.print(": ");
        }
    }
    Serial.println();
}
void loop()
{

}
