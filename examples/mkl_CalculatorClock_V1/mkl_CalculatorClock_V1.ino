/**
 *  Idea:
 *      - Khi vừa cấp nguồn:
 *          - Kiểm tra Eeprom
 *              - Đọc trạng thái bộ đếm giờ counterStatus, timeStartValue
 *              - Hiển thị timeStartValue lên dòng 1 LCD (nếu counterStatus == true)
 * 
 *      - Khi LongPress BTN_START
 *          - Get time (timeStartValue) từ RTC
 *          - Cập nhật Eeprom
 *              - Lưu timeStartValue vào Eeprom
 *              - Bắt đầu bộ đếm giờ counterStatus = true;
 *          - Hiển thị timeStartValue lên dòng 1 LCD
 * 
 *      - Khi LongPress BTN_STOP
 *          - Ngừng bộ đếm giờ counterStatus = false;
 *          - Update Eeprom
 *          - 
 * 
 *      - Khi bộ đếm giờ hoạt động (counterStatus == true):
 *          - Get time (timeStopValue) từ RTC
 *          - get totalMinute = timeStopValue - timeStartValue
 *          - Hiển thị timeStopValue lên dòng 2 LCD
 *          - Hiển thị totalMinute lên dòng 3 LCD
 *          - get moneyValue = totalMinute * costValue
 *          - Hiển thị moneyValue lên dòng 4 LCD
 * 
 *      - Setting over Serial
 *          - Show các command
 *          - Cài đặt timeRTC
 *          - cài đặt costValue
 */

#include "kxnTask_CalculatorClock.h"

#include "MKL_OneButton.h"

MKL_OneButton btnStart = MKL_OneButton(
    A1, // Cấu hình đây là chân Digital Input.
    true,       // Nút nhấn kích hoạt LOW.
    false       // Kích hoạt điện trở nội "Pull-Up".
);

MKL_OneButton btnStop = MKL_OneButton(
    A2, // Cấu hình đây là chân Digital Input.
    true,       // Nút nhấn kích hoạt LOW.
    false       // Kích hoạt điện trở nội "Pull-Up".
);

kxnTask_CalculatorClock kxnTask_CalculatorClock1;

void setup(){
    Serial.begin(115200);
    Serial.println("Start init");
    kxnTask_CalculatorClock1.setup();

    btnStart.attachLongPressStop(Btn_StartF);
    btnStop.attachLongPressStop(Btn_StopF);
}

void loop(){
    kxnTaskManager.run(millis());
    CheckSerial();
    btnStart.tick();
    btnStop.tick();
}

void CheckSerial(){
    if(Serial.available()){
        String temString = Serial.readStringUntil('\n');
        kxnTask_CalculatorClock1.CheckCommand(temString);
    }
}

void Btn_StartF(){
    kxnTask_CalculatorClock1.StartCalculate();
}

void Btn_StopF(){
    kxnTask_CalculatorClock1.StopCalculate();
}