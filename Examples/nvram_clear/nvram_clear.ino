#include <Wire.h>
#include <DS1307.h>

uint8_t address = 0,data = 0;

void setup()
{
    Serial.begin(9600);
    NVRAM.begin();
    for (address = 0; address < NVRAM.length; address ++)
    {
      NVRAM.write(address, 0x00);
      Serial.print(address, DEC);
      Serial.print(" ");
      Serial.print(address, HEX);
      Serial.println(" 0x00");
      delay(250);
    }
}
void loop()
{
   
}