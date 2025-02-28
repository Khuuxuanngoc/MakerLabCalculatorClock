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
  uint32_t costValue[EEPROM_CALCULATOR_CLOCK_MAX_NUM_COST];
  uint8_t costValueIndex;
  uint8_t minFreeValue[EEPROM_CALCULATOR_CLOCK_MAX_NUM_COST];
  float lastFee;

} Str_ClockEeprom;

String nameService[EEPROM_CALCULATOR_CLOCK_MAX_NUM_COST] = {" TV", " HD", "*TV", "*HD"};

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
    this->clockEeprom.costValueIndex = 0;
    // for(int cf = 0; cf < EEPROM_CALCULATOR_CLOCK_MAX_NUM_COST; cf++){
    // this->clockEeprom.costValue[this->clockEeprom.costValueIndex] = 100;
    // }

    this->clockEeprom.costValue[0] = 240;
    this->clockEeprom.costValue[1] = 360;
    this->clockEeprom.costValue[2] = 240;
    this->clockEeprom.costValue[3] = 360;
    

    this->clockEeprom.minFreeValue[0] = 10;
    this->clockEeprom.minFreeValue[1] = 10;
    this->clockEeprom.minFreeValue[2] = 0;
    this->clockEeprom.minFreeValue[3] = 0;
    

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
  digitalWrite(CLOCK_LED_STATUS_PIN, CLOCK_LED_OFF);

  if(this->clockEeprom.counterStatus == true)
  {
    this->clockEeprom.counterStatus = false;
    // digitalWrite(CLOCK_LED_STATUS_PIN, CLOCK_LED_OFF);
    this->SaveEeprom();
  }
  else
  {
    // this->clockEeprom.counterStatus = false;
    // digitalWrite(CLOCK_LED_STATUS_PIN, CLOCK_LED_OFF);
    this->SaveEeprom();
    ShowTypeCost();
  }
  // this->clockEeprom.counterStatus = false;
  // digitalWrite(CLOCK_LED_STATUS_PIN, CLOCK_LED_OFF);
  // this->SaveEeprom();
  // // ShowTypeCost();

  // this->pLCD->setCursor(0, 1);
  // this->pLCD->print("Stop:");
}

void ShowTypeCost()
{
  if (this->clockEeprom.counterStatus == false)
  {
    this->pLCD->clear();


    this->pLCD->setCursor(0, 2);
    this->pLCD->print(String() + this->clockEeprom.minFreeValue[this->clockEeprom.costValueIndex] + " min   ");

    this->pLCD->setCursor(0, 3);
      this->pLCD->print(String() + "Old Fee: " + (uint16_t)this->clockEeprom.lastFee + "K   ");
  }

  this->pLCD->setCursor(17, 2);
  this->pLCD->print(F("  "));
  this->pLCD->setCursor(9, 2);
  
  // this->pLCD->print(String() + "*S" + (this->clockEeprom.costValueIndex + 1) + ":" + (this->clockEeprom.costValue[this->clockEeprom.costValueIndex]) + "K/h");
  this->pLCD->print(String() + nameService[this->clockEeprom.costValueIndex] + ":" + (this->clockEeprom.costValue[this->clockEeprom.costValueIndex]) + "K/h");
  
}

void ChangeTypeCost()
{
  if (this->clockEeprom.costValueIndex < (EEPROM_CALCULATOR_CLOCK_MAX_NUM_COST - 1))
  {
    this->clockEeprom.costValueIndex++;
  }
  else
  {
    this->clockEeprom.costValueIndex = 0;
  }

  this->ShowTypeCost();
}

void ShowStartTimeAtSetup()
{
  if (this->clockEeprom.counterStatus == true)
  {
    this->pLCD->clear();
    this->pLCD->setCursor(1, 0);
    this->pLCD->print("Start:");

    this->pLCD->setCursor(1, 1);
    this->pLCD->print("Stop:");

    // this->pLCD->setCursor(11, 2);
    // this->pLCD->print(String() + (this->clockEeprom.costValue[this->clockEeprom.costValueIndex]/60.0) + "K/min");

    // this->pLCD->setCursor(10, 0);
    // this->pLCD->print(String() + this->clockEeprom.timeStartValue.hour() + ":" + this->clockEeprom.timeStartValue.minute() + ":" + this->clockEeprom.timeStartValue.second() + "   ");
    this->pLCD->setCursor(10, 0); // show Start time
    this->pLCD->print(F("00:00:00"));

    digitalWrite(CLOCK_LED_STATUS_PIN, CLOCK_LED_OFF);
  }
  else
  {
    this->ShowTypeCost();
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

  pinMode(CLOCK_LED_STATUS_PIN, OUTPUT);

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

    float moneyValue = 0.0;

    // this->pLCD->setCursor(10, 1);
    // this->pLCD->print(this->DateTimeToString(timeStopValue) + "   ");

    // this->pLCD->setCursor(9, 2);
    // this->pLCD->print(String() + "*S" + (this->clockEeprom.costValueIndex + 1) + ":" + (this->clockEeprom.costValue[this->clockEeprom.costValueIndex]) + "K/h");
    this->ShowTypeCost();

    // this->pLCD->setCursor(0, 2);
    // this->pLCD->print(String() + totalMinute + " min   ");

    if (totalMinute >= CLOCK_MINUTE_FREE)
    {
      unsigned long totalMinuteFee = totalMinute - CLOCK_MINUTE_FREE;
      digitalWrite(CLOCK_LED_STATUS_PIN, CLOCK_LED_ON);

      // calculate a date which is 0 days, 0 hours, CLOCK_MINUTE_FREE minutes, and 0 seconds into the future
      DateTime timeStartShow = (this->clockEeprom.timeStartValue + TimeSpan(0,0,CLOCK_MINUTE_FREE,0));
      // calculate a date which is 7 days, 12 hours, 30 minutes, and 6 seconds into the future
      // DateTime future (now + TimeSpan(7,12,30,6));

      this->pLCD->setCursor(10, 0); // show Start time
      this->pLCD->print(String() + timeStartShow.hour() + ":" + timeStartShow.minute() + ":" + timeStartShow.second() + "   ");

      this->pLCD->setCursor(10, 1);
      this->pLCD->print(this->DateTimeToString(timeStopValue) + "   ");

      this->pLCD->setCursor(0, 2);
      // this->pLCD->print(String() + totalMinute + " min   ");
      this->pLCD->print(String() + totalMinuteFee + " min   ");

      // this->pLCD->setCursor(0, 2);
      // this->pLCD->print(String() + totalMinuteFee + " min   ");

      // this->pLCD->setCursor(9, 2);
      // this->pLCD->print(String() + "*S" + (this->clockEeprom.costValueIndex + 1) +":" + (this->clockEeprom.costValue[this->clockEeprom.costValueIndex]) + "K/h");

      // this->pLCD->setCursor(11, 2);
      // this->pLCD->print(String() + (this->clockEeprom.costValue[this->clockEeprom.costValueIndex]/60.0) + "K/min");
      moneyValue = (float)(totalMinuteFee * this->clockEeprom.costValue[this->clockEeprom.costValueIndex] / 60.0);
      this->pLCD->setCursor(10, 3);
      this->pLCD->print(F("          "));
      this->pLCD->setCursor(0, 3);
      this->pLCD->print(String() + ">>> Fee: " + (uint16_t)moneyValue + "K   ");
      this->clockEeprom.lastFee = moneyValue;
    }
    else
    {
      this->pLCD->setCursor(10, 1);
      this->pLCD->print(F("00:00:00"));

      this->pLCD->setCursor(0, 2);
      this->pLCD->print(F("       "));

      this->pLCD->setCursor(0, 3);
      this->pLCD->print(String() + ">>> **FREE** " + (CLOCK_MINUTE_FREE - totalMinute) + "min  ");
    }

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

void setCostToEeprom(uint8_t paIndex, uint32_t paCost)
{
  if (paIndex < EEPROM_CALCULATOR_CLOCK_MAX_NUM_COST)
  {
    this->clockEeprom.costValue[paIndex] = paCost;
    this->SaveEeprom();
  }
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

uint8_t isCalculating()
{
  return this->clockEeprom.counterStatus == true;
}
END

#endif