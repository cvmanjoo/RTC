#include <Wire.h>
#include <I2C_RTC.h>

static NVRAM NVRAM;

uint8_t address = 0,data = 0;

void setup()
{
    Serial.begin(9600);
    NVRAM.begin();

   char Str1[10] = "Manjunath";
   char Str2[10] = "";
   NVRAM.read(18,Str2,9);

   Serial.println(Str2);


    uint8_t writeData[4] = { 0xBE, 0xEF, 0x01, 0x02 };

    NVRAM.write(18,writeData,4);

  uint8_t readData[4] = {0};
  NVRAM.read(18,readData,4);
  Serial.println(readData[0], HEX);
  Serial.println(readData[1], HEX);
  Serial.println(readData[2], HEX);
  Serial.println(readData[3], HEX);



}
void loop()
{

}
