/**
 *  Idea:
 *      - Khi vừa cấp nguồn:
 *          - Kiểm tra Eeprom
 *              - Đọc trạng thái bộ đếm giờ counterStatus, timeStartValue
 *              - Hiển thị timeStartValue lên dòng 1 LCD (nếu counterStatus == true)
 *
 *      - Khi Click BTN_START
 *          - khi counterStatus == false
 *          - Chọn giá DV S1, S2, S3
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
 *          - Khi bộ đếm >= 10 phút
 *              - Bật đèn Led báo hiệu bắt đầu tính giờ
 *              - Hiển thị totalMinute lên dòng 3 LCD
 *              - get moneyValue = (totalMinute - 10)  * costValue[Sx]
 *              - Hiển thị moneyValue lên dòng 4 LCD
 *
 *      - Setting over Serial
 *          - Show các command
 *          - Cài đặt timeRTC
 *          - cài đặt costValue
 */

#include "kxnTask_CalculatorClock.h"

#include "MKL_OneButton.h"

MKL_OneButton btnStart = MKL_OneButton(
    A1,   // Cấu hình đây là chân Digital Input.
    true, // Nút nhấn kích hoạt LOW.
    false // Kích hoạt điện trở nội "Pull-Up".
);

MKL_OneButton btnStop = MKL_OneButton(
    A2,   // Cấu hình đây là chân Digital Input.
    true, // Nút nhấn kích hoạt LOW.
    false // Kích hoạt điện trở nội "Pull-Up".
);

kxnTask_CalculatorClock kxnTask_CalculatorClock1;

void setup()
{
    Serial.begin(115200);
    Serial.println("Start init");
    kxnTask_CalculatorClock1.setup();

    btnStart.attachLongPressStop(Btn_StartF);
    btnStart.attachClick(Btn_Start_SelectTypeCostF);
    
    btnStop.attachLongPressStop(Btn_StopF);
}

void loop()
{
    kxnTaskManager.run(millis());
    CheckSerial();
    btnStart.tick();
    btnStop.tick();
}

void CheckSerial()
{
    if (Serial.available())
    {
        String temString = Serial.readStringUntil('\n');
        kxnTask_CalculatorClock1.CheckCommand(temString);
    }
}

void Btn_StartF()
{
    if (!kxnTask_CalculatorClock1.isCalculating())
    {
        kxnTask_CalculatorClock1.StartCalculate();
    }
}

void Btn_StopF()
{
    if (kxnTask_CalculatorClock1.isCalculating())
    {
        kxnTask_CalculatorClock1.StopCalculate();
    }
}

void Btn_Start_SelectTypeCostF()
{
    if (!kxnTask_CalculatorClock1.isCalculating())
    {
        kxnTask_CalculatorClock1.ChangeTypeCost();
    }
}