#include <Arduino.h>
#include <Wire.h>
#include <I2C_RTC.h>

// DS3232::DS3232() {
//   Wire.begin();
// }

uint8_t DS3232::read(uint8_t address) {
//   Wire.beginTransmission(DS3232_ADDR);
//   Wire.write(address);
//   Wire.endTransmission();
//   Wire.requestFrom(DS3232_ADDR, 1);
//   return Wire.read();

    Serial.println("DS3232::read");
    return 128;
}

void DS3232::write(uint8_t address, uint8_t value)
{
//   Wire.beginTransmission(DS3232_ADDR);
//   Wire.write(address);
//   Wire.endTransmission();
//   Wire.requestFrom(DS3232_ADDR, 1);
//   return Wire.read();

    Serial.println("DS3232::write");
   
}