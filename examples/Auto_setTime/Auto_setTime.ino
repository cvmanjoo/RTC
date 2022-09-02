
#include <Wire.h>
#include <I2C_RTC.h>

//static DS1307 RTC;
//static DS3231 RTC;
//static PCF8523 RTC;
//static PCF8563 RTC;

int hours,minutes,seconds,day,month,year;

void setup()
{
    Serial.begin(9600);
    while (!Serial); // wait for serial port to connect. Needed for native USB
    
    RTC.begin();

    Serial.print("Is Clock Running: ");
    if (RTC.isRunning())
    {
        Serial.println("Yes");
        switch (RTC.getWeek())
        {
            case 1: Serial.print("MON");
            break;
            case 2: Serial.print("TUE");
            break;
            case 3: Serial.print("WED");
            break;
            case 4: Serial.print("THU");
            break;
            case 5: Serial.print("FRI");
            break;
            case 6: Serial.print("SAT");
            break;
            case 7: Serial.print("SUN");
            break;
        }
		Serial.print(" ");
		
		day=RTC.getDay();
		month=RTC.getMonth();
		year=RTC.getYear();

		if(day<10)
			Serial.print("0");

		Serial.print(RTC.getDay());
		Serial.print("-");
		if(month<10)
			Serial.print("0");
		Serial.print(RTC.getMonth());
		Serial.print("-");
		Serial.print(RTC.getYear());
		Serial.print(" ");
		
		hours = RTC.getHours();
		minutes = RTC.getMinutes(); 
		seconds = RTC.getSeconds();
		
		if(hours<10)
			Serial.print("0");
        Serial.print(hours);
        Serial.print(":");
		if(minutes<10)
			Serial.print("0");
        Serial.print(minutes);
        Serial.print(":");
		if(seconds<10)
			Serial.print("0");
        Serial.print(RTC.getSeconds());
        Serial.print("");
        if (RTC.getHourMode() == CLOCK_H12)
        {
            switch (RTC.getMeridiem())
            {
                case HOUR_AM:
                    Serial.print(" AM");
                    break;
                case HOUR_PM:
                    Serial.print(" PM");
                    break;
            }   
        }
        Serial.println();
        delay(1000);
    }       
    else
    {
        delay(250);

        Serial.println("No");
        Serial.println("Setting Time");
        RTC.setHourMode(CLOCK_H12); //Comment if RTC PCF8563
        //RTC.setHourMode(CLOCK_H24);  
        RTC.setDateTime(__DATE__, __TIME__);
        RTC.updateWeek();           //Update Weekdaybased on new date.    
        Serial.println("New Time Set");
        Serial.print(__DATE__);
        Serial.print(" ");
        Serial.println(__TIME__);
        RTC.startClock(); //Start the Clock;
    }
}

void loop()
{
}