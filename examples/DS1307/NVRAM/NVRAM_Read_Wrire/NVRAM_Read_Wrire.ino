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


    NVRAM.write(37,0xAB);
	
	Serial.print("Function Read : ");
    Serial.println(NVRAM.read(37),HEX);
	Serial.print("Operatot Read : ");
    Serial.println(NVRAM[37],HEX);

}
void loop()
{

}
