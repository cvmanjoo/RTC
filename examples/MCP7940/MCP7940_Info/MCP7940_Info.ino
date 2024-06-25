#include <Wire.h>
#include <I2C_RTC.h>

static MCP7940 RTC;

int hours,minutes,seconds,day,month,year;

void setup()
{
    Serial.begin(9600);
    while (!Serial); // wait for serial port to connect. Needed for native USB

/*************************************************
    RTC.isConnected();


    
    RTC.setWeek(4);
    RTC.enableExternalBattery();
    //RTC.disableExternalBattery();
    
    Serial.println(RTC.getWeek());

    Serial.println(RTC.showRegister(0x03),BIN);




***************************************************/
    RTC.enableExternalBattery();

    Serial.println();
    Serial.println("*** RTC MCP7940 ***");

    Serial.print("Is RTC Connected? : ");
    if (RTC.isConnected())
        Serial.println("Yes");
    else
    {
        Serial.println("No. Check Connections");
        while(true);
    }

    Serial.print("Is External Battery Enabled? : ");
    if (RTC.isExternalBatteryEnabled())
        Serial.println("Yes");
    else
        Serial.println("No");

    Serial.print("Does Power Failed? : ");
    if (RTC.doesPowerFailed())
        Serial.println("Yes");
    else
        Serial.println("No");

    Serial.print("Is Clock Running? : ");
    if (RTC.isRunning())
        Serial.println("Yes");
    else
        Serial.println("No. Time may be Inaccurate");


    Serial.print("Hour Mode : ");
    if (RTC.getHourMode() == CLOCK_H12)
        Serial.println("12 Hours");
    else
        Serial.println("24 Hours");
}

void loop()
{
    //if (RTC.isRunning())
    {
        switch (RTC.getWeek())
        {
            case 1: Serial.print("SUN");
            break;
            case 2: Serial.print("MON");
            break;
            case 3: Serial.print("TUE");
            break;
            case 4: Serial.print("WED");
            break;
            case 5: Serial.print("THU");
            break;
            case 6: Serial.print("FRI");
            break;
            case 7: Serial.print("SAT");
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
}
