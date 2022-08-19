#include <Wire.h>
#include <I2C_RTC.h>

static NVRAM NVRAM;

uint8_t address = 0,data = 0;

void setup()
{
    Serial.begin(9600);
    NVRAM.begin();
    for (address = 0; address < NVRAM.length; address++)
    {
      data = NVRAM.read(address);
      Serial.print(address, DEC);
      Serial.print(" ");
      Serial.print(address, HEX);
      Serial.print(" ");
      Serial.println(data, HEX);
      delay(250);
    }
}
void loop()
{

}
