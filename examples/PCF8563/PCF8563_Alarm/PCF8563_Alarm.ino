#include <Wire.h>
#include <I2C_RTC.h>

const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;



static PCF8563 RTC;


void setup()
{
    Serial.begin(115200);
    while (!Serial);
    
    RTC.begin();

    pinMode(ledPin, OUTPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, LOW);

    Serial.println();
    Serial.println("*** RTC PCF8563 ***");

    RTC.clearAlarm();
   RTC.disableAlarm();

    RTC.enableAlarmInterrupt();

    RTC.setAlarmHours(0);

    RTC.setAlarmMinutes(58);

    Serial.print(RTC.getAlarmHours());
    Serial.print("h ");
    Serial.print(RTC.getAlarmMinutes());    
    Serial.println("m ");

    /*Serial.println(RTC.getAlarmMinutes());

    Serial.print(RTC.getAlarmDay());
    Serial.print("  -  ");
    Serial.println(RTC.getAlarmWeek());
  */

}
void loop()
{
    Serial.print("Is Clock Running : ");
    if (RTC.isRunning())
    {
        Serial.println("Yes");
        displayTime();
    }
    else
    {
        Serial.println("No");
        while(true);
    }

    Serial.print("Alarm Enabled  : ");
    if (RTC.isAlarmEnabled())

        Serial.println("Yes");
    else
        Serial.println("No");
        
    Serial.print("Alarm Triggered : ");
    if (RTC.isAlarmTriggered())
        Serial.println("Yes");
    else
        Serial.println("No");

    delay(5000);
  
}

void blink() {
 // digitalWrite(13,HIGH);
  Serial.println("Interrupt!");
  RTC.clearAlarm();
}

void displayTime()
{
 switch (RTC.getWeek())
  {
    case 1:
      Serial.print("SUN");
      break;
    case 2:
      Serial.print("MON");
      break;
    case 3:
      Serial.print("TUE");
      break;
    case 4:
      Serial.print("WED");
      break;
    case 5:
      Serial.print("THU");
      break;
    case 6:
      Serial.print("FRI");
      break;
    case 7:
      Serial.print("SAT");
      break;
  }
  Serial.print(" ");
  Serial.print(RTC.getDay());
  Serial.print("-");
  Serial.print(RTC.getMonth());
  Serial.print("-");
  Serial.print(RTC.getYear());

  Serial.print(" ");

  Serial.print(RTC.getHours());
  Serial.print(":");
  Serial.print(RTC.getMinutes());
  Serial.print(":");
  Serial.print(RTC.getSeconds());
  if (RTC.getHourMode() == CLOCK_H12)
  {
    switch (RTC.getMeridiem())
    {
      case HOUR_AM :
        Serial.print(" AM");
        break;
      case HOUR_PM :
        Serial.print(" PM");
        break;
    }
  }
  Serial.println();
}
