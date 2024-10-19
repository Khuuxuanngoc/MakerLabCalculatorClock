#ifndef kxnTask_RTC__H
#define kxnTask_RTC__H

#include "kxnTask.h"
#include "MKL_RTClib.h"

CREATE_TASK(kxnTask_RTC)

DateTime now;
RTC_DS1307 rtc;

void setup()
{
    if (!this->rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
    }

    if (!this->rtc.isrunning())
    {
        Serial.println("RTC is NOT running, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        this->rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // this->rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    kxnTaskManager.add(this);
    setState(0);
}

void loop()
{
    now = this->rtc.now();
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    kDelay(1000);
}

DateTime getTimeNow(){
    return this->rtc.now();
}

void setTimeNow(int phour, int pminute, int psecond){
    DateTime tempTime = getTimeNow();

    this->rtc.adjust(DateTime(tempTime.year(), tempTime.month(), tempTime.day(), phour, pminute, psecond));
}

END

#endif