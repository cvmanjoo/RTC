#include <Wire.h>
#include <I2C_RTC.h>

static DS3232::SRAM RAM;

uint8_t address = 0, data = 0;

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial port to connect. Needed for native USB port only

	RAM.begin();

	if(RAM.isConnected())
    {
        Serial.println("RTC Not Connected!");
        while(true);
    }

    Serial.println("DS3232 Contents\n");

    Serial.print("    ");
    for (int i = 0; i<=15; i++)
    {
        Serial.print(" ");
        Serial.print(i ,HEX);
        Serial.print(" ");
    }
    
    Serial.println();
    Serial.print("00: ");

    for (address = 0; address <  RAM.length(); address++)
    {
        
        data = RAM.read(address);
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
