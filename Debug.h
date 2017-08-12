#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDR 0x68

class Debug
{
  public:
  uint8_t getRegister(uint8_t reg);
  void setRegister(uint8_t reg, uint8_t data);
};

//Instantiate class DS3231 as RTC

static Debug Debug;

#endif /* DEBUG_H */