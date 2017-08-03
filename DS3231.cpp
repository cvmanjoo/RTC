/* 
 * DS3231.cpp - Library to set & get time from RTC DS3231
 * Created by Manjunath CV. July 08, 2017, 02:18 AM
 * Released into the public domain.
 */

#include <Wire.h>
#include "DS3231.h"


void DS3231::begin()
{
    Wire.begin(); // join i2c bus
}

/*----------------------------------------------------------------------------------------------
getSecond

+---------+--------+-------+-------+-------+-------+-------+-------+--------+----------+-------+
| ADDRESS | BIT 7  | BIT 6 | BIT 5 | BIT 4 | BIT 3 | BIT 2 | BIT 1 | BIT 0  | FUNCTION | RANGE |
+---------+--------+-------+-------+-------+-------+-------+-------+--------+----------+-------+
|   0x00  |    0   |       10 Seconds      |             Seconds            |  Seconds | 00-59 |
+---------+--------+-----------------------+--------------------------------+----------+-------+

----------------------------------------------------------------------------------------------*/
uint8_t DS3231::getSecond()
{
    uint8_t data;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x00);  // Second Register
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();

    return (bcd2bin(data));
}

void DS3231::setSecond(uint8_t data)
{
    //uint8_t data;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x00);  // Second Register

    Wire.write(bin2bcd(data));

    Wire.endTransmission();
}


/*-----------------------------------------------------------
getMinute
-----------------------------------------------------------*/
uint8_t DS3231::getMinute()
{
    uint8_t data;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x01);  // Minute Register
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();

    return (bcd2bin(data));
}

void DS3231::setMinute(uint8_t data)
{

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x01);  // Minute Register

    Wire.write(bin2bcd(data));

    Wire.endTransmission();
}

/*-----------------------------------------------------------
getHour
-----------------------------------------------------------*/
uint8_t DS3231::getHour()
{
    uint8_t data;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x02);  // Hour Register
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();

    return (bcd2bin(data));
}

void  DS3231::setHour(uint8_t data)
{
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x02);  // Hour Register

    Wire.write(bin2bcd(data));

    Wire.endTransmission();
}


/*-----------------------------------------------------------
getWeek
-----------------------------------------------------------*/
uint8_t DS3231::getWeek()
{
    uint8_t data;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x03);  // Hour Register
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();

    //return (bcd2bin(data));
    return (data);
}

/*-----------------------------------------------------------
getDay
-----------------------------------------------------------*/
      
uint8_t DS3231::getDay()
{
    uint8_t data;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x04);  // Day Register
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();

    return (bcd2bin(data));
}

void DS3231::setDay(uint8_t data)
{


    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x04);  // Day Register
    
    Wire.write(bin2bcd(data));

    Wire.endTransmission();
}

/*-----------------------------------------------------------
getMonth
-----------------------------------------------------------*/
uint8_t DS3231::getMonth()
{
    uint8_t data;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();
    
    //data = data & 0b00001111;

    return (bcd2bin(data));
}

void DS3231::setMonth(uint8_t data)
{
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register
 
    Wire.write(bin2bcd(data));

    Wire.endTransmission();
}

/*-----------------------------------------------------------
getYear
-----------------------------------------------------------*/
uint16_t DS3231::getYear()
{
    uint8_t data;
    uint16_t year;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x06);  // Year Register
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();
    year = bcd2bin(data)+2000;
    return (year);
}

void DS3231::setYear(uint16_t year)
{

    uint8_t data;
    
    data = year - 2000;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x06);  // Year Register

    Wire.write(bin2bcd(data));

    Wire.endTransmission();
}


/*-----------------------------------------------------------
setTime
-----------------------------------------------------------*/

void DS3231::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x00);  // Year Register

    Wire.write(bin2bcd(second));
    Wire.write(bin2bcd(minute));
    Wire.write(bin2bcd(hour));

    Wire.endTransmission();
}


/*-----------------------------------------------------------
setDate
-----------------------------------------------------------*/
void DS3231::setDate(uint8_t day, uint8_t month, uint16_t year)
{
    if (year > 2000)
    {
        year = year - 2000;
    }

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x04);  // Year Register

    Wire.write(bin2bcd(day));
    Wire.write(bin2bcd(month));
    Wire.write(bin2bcd(year));

    Wire.endTransmission();
}

/*-----------------------------------------------------------
enableAlaram()
-----------------------------------------------------------*/
uint8_t DS3231::enableAlaram()
{
    uint8_t reg;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x0E);  // Hour Register
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(DS3231_ADDR, 1);
    reg = Wire.read();

    //return (bcd2bin(data));
    //return (data);

    bitWrite(reg,2,1); // Write bit INTCN to 1 to enable INT/SQW pin 

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x0E);  // Month Register
 
    Wire.write(bin2bcd(reg));

    Wire.endTransmission();


}



/*-----------------------------------------------------------
DS3231 Exclusive Functions
-----------------------------------------------------------*/
bool DS3231::lostPower(void)
{
    uint8_t data;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x0F); 
    Wire.endTransmission();
    delay(10);


    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();

    return (data >> 7);
}

void DS3231::StartClock(void)
{
    uint8_t data;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x0F);

    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();

    //data =  data & 0x7F; 

    bitWrite(data,7,0); // Write  EOSC Register to 0 to start the clock. 

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x0F);  // Month Register
 
    Wire.write(bin2bcd(data));

    Wire.endTransmission();


}


float DS3231::getTemp()
{
    float rv;
    uint8_t temp_msb, temp_lsb;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x11); 
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(DS3231_ADDR, 2);
    temp_msb = Wire.read();
    temp_lsb = Wire.read() >> 6;

    rv = (0.25 * temp_lsb) + temp_msb;

    return(rv);
}

static uint8_t DS3231::bcd2bin (uint8_t val) 
{ 
    return val - 6 * (val >> 4); 
}
static uint8_t DS3231::bin2bcd (uint8_t val) 
{ 
    return val + 6 * (val / 10); 
}