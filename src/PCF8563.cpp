/*
* PCF8563.cpp - Library to set & get time from RTC PCF8563
* Created by Manjunath CV. March 23, 2019, 03:02 AM
* Released into the public domain.
*/

#include <time.h>
#include <Arduino.h>
#include <Wire.h>
#include <PCF8563.h>

bool PCF8563::begin()
{
    Wire.begin(); // join i2c bus
    Wire.beginTransmission (PCF8563_ADDR);
    return (Wire.endTransmission() == 0 ?  true : false);
}


bool PCF8563::isRunning(void)
{
    uint8_t data;
    bool flag;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data = Wire.read();

   flag = bitRead(data,5);

    return (!flag);
}

void PCF8563::startClock(void) //Not Updated
{
    uint8_t data;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data = Wire.read();

    bitClear(data, 5);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x00);  // Seconds Register
    Wire.write(data);
    Wire.endTransmission();
}

void PCF8563::stopClock(void) //Not Updated
{
  uint8_t data;

  Wire.beginTransmission(PCF8563_ADDR);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom(PCF8563_ADDR, 1);
  data = Wire.read();

  bitSet(data, 5);

  Wire.beginTransmission(PCF8563_ADDR);
  Wire.write(0x00);  // Seconds Register
  Wire.write(data);
  Wire.endTransmission();
}

/*-----------------------------------------------------------
get & set Second
-----------------------------------------------------------*/
uint8_t PCF8563::getSeconds()
{
    uint8_t second;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x02);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    second = Wire.read();
    return (bcd2bin(second));

}

void PCF8563::setSeconds(uint8_t second)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x02);  // Second Register
    Wire.write(bin2bcd(second));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getMinutes
-----------------------------------------------------------*/
uint8_t PCF8563::getMinutes()
{
    uint8_t minute;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x03);  // Minute Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 1);
    minute = Wire.read();
    return (bcd2bin(minute));
}

void PCF8563::setMinutes(uint8_t minute)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x03);  // Minute Register
    Wire.write(bin2bcd(minute));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getHours
-----------------------------------------------------------*/
uint8_t PCF8563::getHours()
{
    uint8_t hour;
    bool flag;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x04);  // Hour Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 1);
    hour = Wire.read();
        return (bcd2bin(hour));
}

void  PCF8563::setHours(uint8_t hour)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x04);  // Hour Register
     Wire.write(bin2bcd(hour));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getWeek
-----------------------------------------------------------*/
uint8_t PCF8563::getWeek()
{
    uint8_t week;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x06);  // Week Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 1);
    week = Wire.read();
    return week;
}

void PCF8563::setWeek(uint8_t week)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x06);  // Minute Register
    Wire.write(week);
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getDay
-----------------------------------------------------------*/
uint8_t PCF8563::getDay()
{
    uint8_t day;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x05);  // Day Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 1);
    day = Wire.read();
    return (bcd2bin(day));
}

void PCF8563::setDay(uint8_t day)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x05);  // Day Register
    Wire.write(bin2bcd(day));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getMonth()
-----------------------------------------------------------*/
uint8_t PCF8563::getMonth()
{
    uint8_t month;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x07);  // Month Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 1);
    month = Wire.read();
    return (bcd2bin(month));
}
/*-----------------------------------------------------------
setMonth()
-----------------------------------------------------------*/

void PCF8563::setMonth(uint8_t month)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x07);  // Month Register
    Wire.write(bin2bcd(month));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getYear (Completed)
-----------------------------------------------------------*/
uint16_t PCF8563::getYear()
{
    uint16_t year;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x08);  // Year Register
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    year = Wire.read();
    return (bcd2bin(year)+2000);
}

void PCF8563::setYear(uint16_t year)
{
    year = year % 100; //Converting to 2 Digit

    /*Writing 2 Digit year to Year Register(0x06)*/
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x08);  // Year Register to write year
    Wire.write(bin2bcd(year));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
setTime
-----------------------------------------------------------*/

void PCF8563::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x02);  // Year Register
    Wire.write(bin2bcd(second));
    Wire.write(bin2bcd(minute));
    Wire.write(bin2bcd(hour));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
setDate (Should be Optimised)
-----------------------------------------------------------*/
void PCF8563::setDate(uint8_t day, uint8_t month, uint16_t year)
{
    year = year % 100; //Converting to 2 Digit

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x05);
    Wire.write(bin2bcd(day));
    Wire.write(0);      //Weekday Resets to Zero!
    Wire.write(bin2bcd(month));
    Wire.write(bin2bcd(year));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
setDateTime()
Taken from https://github.com/adafruit/RTClib/
-----------------------------------------------------------*/

void PCF8563::setDateTime(char* date, char* time)
{
    uint8_t day, month, hour, minute, second;
    uint16_t year;
// sample input: date = "Dec 26 2009", time = "12:34:56"
    year = atoi(date + 9);
    setYear(year);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (date[0]) {
        case 'J': month = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7); break;
        case 'F': month = 2; break;
        case 'A': month = date[2] == 'r' ? 4 : 8; break;
        case 'M': month = date[2] == 'r' ? 3 : 5; break;
        case 'S': month = 9; break;
        case 'O': month = 10; break;
        case 'N': month = 11; break;
        case 'D': month = 12; break;
    }
    setMonth(month);
    day = atoi(date + 4);
    setDay(day);
    hour = atoi(time);
    setHours(hour);
    minute = atoi(time + 3);
    setMinutes(minute);
    second = atoi(time + 6);
    setSeconds(second);
}

/*-----------------------------------------------------------
Alarm Functions
-----------------------------------------------------------*/

/*-----------------------------------------------------------
void disableAlarm()
-----------------------------------------------------------*/

void PCF8563::disableAlarm()
{
    uint8_t minutes,hours,day,weekday;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);  // Alarm Hour Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 4);

    minutes = Wire.read();
    hours = Wire.read();
    day = Wire.read();
    weekday = Wire.read();

    bitSet(minutes,7);
    bitSet(hours,7);
    bitSet(day,7);
    bitSet(weekday,7);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);  // Month Register
    Wire.write(minutes);
    Wire.write(hours);
    Wire.write(day);
    Wire.write(weekday);
    Wire.endTransmission();
}


/*-----------------------------------------------------------
setAlarmHours()
-----------------------------------------------------------*/

void PCF8563::setAlarmHours(uint8_t hours)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0A);  // Month Register
    Wire.write(bin2bcd(hours));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
setAlarmMinutes()
-----------------------------------------------------------*/

void PCF8563::setAlarmMinutes(uint8_t hours)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);  // Month Register
    Wire.write(bin2bcd(hours));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
//getAlarmHours()
-----------------------------------------------------------*/
uint8_t PCF8563::getAlarmHours()
{
    uint8_t hours;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0A);  // Alarm Hour Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 1);
    hours = Wire.read();
    bitClear(hours,7);
    return (bcd2bin(hours));
}

/*-----------------------------------------------------------
//getAlarmMinutes()
-----------------------------------------------------------*/
uint8_t PCF8563::getAlarmMinutes()
{
    uint8_t minutes;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);  // Month Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 1);
    minutes = Wire.read();
    bitClear(minutes,7);
    return (bcd2bin(minutes));
}

/*-----------------------------------------------------------
void clearAlarm();
-----------------------------------------------------------*/

void PCF8563::clearAlarm(void)
{
    uint8_t data;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data = Wire.read();
    bitClear(data,3);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.write(data);
    Wire.endTransmission();
}

/*-----------------------------------------------------------
bool isAlarmTriggered();
-----------------------------------------------------------*/

bool PCF8563::isAlarmTriggered(void)
{
    uint8_t data;
    bool flag;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data= Wire.read();
    //Serial.print("Debug : data -- ");
    //Serial.println(data,BIN);
    flag = bitRead(data,3);

    return (flag);
}


/* Helpers */

uint8_t PCF8563::bcd2bin (uint8_t val)
{
    return val - 6 * (val >> 4);
}
uint8_t PCF8563::bin2bcd (uint8_t val)
{
    return val + 6 * (val / 10);
}