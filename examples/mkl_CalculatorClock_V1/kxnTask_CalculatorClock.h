#ifndef kxnTask_CalculatorClock__H
#define kxnTask_CalculatorClock__H

#include "kxnTask.h"
#include "kxnTask_RTC.h"
#include <EEPROM.h>
#include "MKL_LiquidCrystal_I2C.h"
#include "setting_CalculatorClock.h"

typedef struct
{
    uint16_t eepromAddress;
    uint8_t counterStatus;
    uint8_t firstCheck;
    DateTime timeStartValue;
    uint32_t costValue;

} Str_ClockEeprom;

CREATE_TASK(kxnTask_CalculatorClock)

MKL_LiquidCrystal_I2C *pLCD;

kxnTask_RTC kxnTask_RTC1;

uint16_t eepromAddress;

Str_ClockEeprom clockEeprom;

void lcdSetup()
{
    if (!pLCD)
        pLCD = new MKL_LiquidCrystal_I2C(0x27, 20, 4);

    pLCD->init();
    pLCD->backlight();
}

void SaveEeprom()
{
    clockEeprom.eepromAddress = this->eepromAddress;
    // clockEeprom.timeStartValue = kxnTask_RTC1.getTimeNow();
    EEPROM.put(this->eepromAddress, clockEeprom);
}

void ReadEeprom()
{
    EEPROM.get(this->eepromAddress, clockEeprom);
    if (this->clockEeprom.firstCheck != EEPROM_CALCULATOR_CLOCK_CHECK_FIRST_VALUE)
    {
        this->clockEeprom.firstCheck = EEPROM_CALCULATOR_CLOCK_CHECK_FIRST_VALUE;
        this->clockEeprom.counterStatus = false;
        this->clockEeprom.costValue = 100;
        this->SaveEeprom();
    }
}

void StartCalculate()
{
    this->clockEeprom.timeStartValue = kxnTask_RTC1.getTimeNow();
    this->clockEeprom.counterStatus = true;
    this->SaveEeprom();

    this->ShowStartTimeAtSetup();
}

void StopCalculate()
{
    this->clockEeprom.counterStatus = false;
    this->SaveEeprom();
    this->pLCD->setCursor(0, 1);
    this->pLCD->print("Stop:");
}

void ShowStartTimeAtSetup()
{
    if (this->clockEeprom.counterStatus == true)
    {
        this->pLCD->clear();
        this->pLCD->setCursor(0, 0);
        this->pLCD->print("Start:");

        this->pLCD->setCursor(10, 0);
        this->pLCD->print(String() + this->clockEeprom.timeStartValue.hour() +
                          ":" + this->clockEeprom.timeStartValue.minute() +
                          ":" + this->clockEeprom.timeStartValue.second() + "   ");
    }
}

String DateTimeToString(DateTime paDT)
{
    return String() + paDT.hour() + ":" + paDT.minute() + ":" + paDT.second();
}

void setup()
{
    this->lcdSetup();

    this->eepromAddress = 0;

    this->ReadEeprom();

    this->ShowStartTimeAtSetup();

    kxnTask_RTC1.setup();
    kxnTaskManager.add(this);
    setState(0);
}

void loop()
{

    if (this->clockEeprom.counterStatus == true)
    {
        // Serial.println(String() + kxnTask_RTC1.getTimeNow().hour() + ":" + kxnTask_RTC1.getTimeNow().minute() + ":" + kxnTask_RTC1.getTimeNow().second());

        DateTime timeStopValue = kxnTask_RTC1.getTimeNow();

        unsigned long subSeconds = timeStopValue.unixtime() - this->clockEeprom.timeStartValue.unixtime();
        unsigned long totalMinute = subSeconds / 60;

        float moneyValue = (float)(totalMinute * this->clockEeprom.costValue / 60.0);

        this->pLCD->setCursor(10, 1);
        this->pLCD->print(this->DateTimeToString(timeStopValue) + "   ");

        this->pLCD->setCursor(0, 2);
        this->pLCD->print(String() + totalMinute + " minute   ");

        this->pLCD->setCursor(0, 3);
        this->pLCD->print(String() + "Money: " + moneyValue + "K   ");

        Serial.println(this->DateTimeToString(timeStopValue));
    }

    kDelay(1000);
}

void setTimeFromSerial(String input)
{
    int hour, minute, second;
    sscanf(input.c_str(), "settime,%d,%d,%d", &hour, &minute, &second);
    kxnTask_RTC1.setTimeNow(hour, minute, second);
    Serial.println("Thời gian đã được thiết lập!");
}

void CheckCommand(String paCmd)
{
    if (paCmd == "")
    {
        return;
    }

    if (paCmd.indexOf("start") > -1)
    {
        this->StartCalculate();
    }
    else if (paCmd.indexOf("stop") > -1)
    {
        this->StopCalculate();
    }
    else if (paCmd.indexOf("settime") > -1)
    {
        setTimeFromSerial(paCmd);
    }
}
END

#endif