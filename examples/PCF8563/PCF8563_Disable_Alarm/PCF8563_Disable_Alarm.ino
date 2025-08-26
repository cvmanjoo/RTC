#include <Wire.h>
#include <I2C_RTC.h>

static PCF8563 RTC;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    while (!Serial);
    
    RTC.begin();

    Serial.println();

    RTC.clearAlarm();
    RTC.disableAlarm();


    Serial.println("*** RTC PCF8563 ***");
    Serial.print("Alarm Cleared and Disabled");



}

void loop() {
  // put your main code here, to run repeatedly:

}
