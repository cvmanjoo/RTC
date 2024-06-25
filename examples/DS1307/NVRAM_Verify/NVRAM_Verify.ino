#include <Wire.h>
#include <I2C_RTC.h>

static NVRAM NVRAM;

uint8_t address = 0,data = 0;

uint32_t crc = ~0L;

const uint32_t crc_table[16] = {
0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

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

    for (int index = 0 ; index < NVRAM.length()  ; ++index)
    {
        crc = crc_table[(crc ^ NVRAM[index]) & 0x0f] ^ (crc >> 4);
        crc = crc_table[(crc ^ (NVRAM[index] >> 4)) & 0x0f] ^ (crc >> 4);
        crc = ~crc;
    }

    if(crc == 3419760290)
        Serial.println("NVRAM is Empty");
    else
        Serial.println("NVRAM is Not Empty");
}
void loop()
{

}
