#include <Wire.h>
#include <Debug.h>

uint8_t Debug::getRegister(uint8_t reg)
{
    Wire.beginTransmission(I2C_ADDR);                       
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(I2C_ADDR, 1);
    reg = Wire.read();
    return (reg);
}

void Debug::setRegister(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(I2C_ADDR);                       
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}