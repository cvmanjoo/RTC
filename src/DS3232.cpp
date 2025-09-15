/*
* DS3232.cpp - Library to set & get time from RTC DS1307
* Created by Manjunath CV. August 15, 2025, 03:20 PM
* Released into the public domain.
*/


#include <Arduino.h>
#include <Wire.h>
#include <I2C_RTC.h>

#define R_SRAM 0x14 // SRAM starts from 0x14 to 0xFF

// DS3232::DS3232() {
//   Wire.begin();
// }

bool DS3232::SRAM::begin()
{
    Wire.begin();
    return (DS3232_ADDR);
}

bool DS3232::SRAM::isConnected()
{
    Wire.begin();
    Wire.beginTransmission(DS3232_ADDR);
    //return (Wire.endTransmission() == 0 ? true : false);
    return (Wire.endTransmission() == 0 ? true : false);
}

uint8_t DS3232::SRAM::read(uint8_t address)
{
    uint8_t reg_data = 0x00;
    if(address < _length )
    {
        Wire.beginTransmission(DS3232_ADDR);
        Wire.write(address + R_SRAM); // SRAM starts at 0x14
        Wire.endTransmission();
        Wire.requestFrom(DS3232_ADDR, 1);
        reg_data = Wire.read();
    }
    return reg_data;
}

void DS3232::SRAM::write(uint8_t address, uint8_t value)
{
    if (address < _length)
    {
        Wire.beginTransmission(DS3232_ADDR);
        Wire.write(address + R_SRAM); // SRAM starts at 0x14
        Wire.write(value);
        Wire.endTransmission();
    }
    
}

void DS3232::SRAM::clear()
{
    Wire.beginTransmission(DS3232_ADDR);
    Wire.write(R_SRAM); // SRAM starts at 0x14
    for (uint8_t i = 0; i < _length; ++i)
    {
        Wire.write(0x00);
    }
    Wire.endTransmission();
}
uint16_t DS3232::SRAM::getCRC16()
{
    uint16_t crc = 0xFFFF;
    for (uint8_t i = 0; i < _length; ++i)
    {
        crc = _crc16_update(crc, read(i));
    }
    return crc;
}

uint16_t DS3232::SRAM::_crc16_update(uint16_t crc, uint8_t a)
{
    int i;
    crc ^= a;
    for (i = 0; i < 8; ++i)
    {
        if (crc & 1)
            crc = (crc >> 1) ^ 0xA001;
        else
            crc = (crc >> 1);
    }
    return crc;
}


uint8_t DS3232::SRAM::length()
{
	return(_length);
}
