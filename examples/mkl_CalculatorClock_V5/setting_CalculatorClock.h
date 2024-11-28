#ifndef setting_CalculatorClock__H
#define setting_CalculatorClock__H

// Pin declare
#define CALCULATOR_CLOCK_PIN_BTN_START        (11)
#define CALCULATOR_CLOCK_PIN_BTN_STOP        (12)
#define CLOCK_LED_STATUS_PIN                            (10)


// #define CLOCK_MINUTE_FREE                               (10)
#define CLOCK_MINUTE_FREE                               (this->clockEeprom.minFreeValue[this->clockEeprom.costValueIndex])

#define CLOCK_LED_OFF                                   (0)
#define CLOCK_LED_ON                                    !CLOCK_LED_OFF

#define EEPROM_CALCULATOR_CLOCK_CHECK_FIRST_VALUE       110
#define EEPROM_CALCULATOR_CLOCK_MAX_NUM_COST            10


#endif