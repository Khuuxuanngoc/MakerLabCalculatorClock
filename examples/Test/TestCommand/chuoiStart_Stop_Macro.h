#ifndef chuoiStart_Stop_Macro__H
#define chuoiStart_Stop_Macro__H
#include "Arduino.h"

#ifndef KXN_DATA_DECODE

#define KXN_DATA_DECODE(name,value)   name##_deCode(value)
#define KXN_DATA_RESET(name)   name##_resetForNext()

#define KXN_DATA_KEY(name)   name##_key
#define KXN_DATA_VALUE(name, index)   name##_values[index]
#define KXN_DATA_CUR_INDEX(name)          name##_curValueIndex

#endif

#define chuoiStart_Stop_Macro_MAX_TERM_SIZE    15
#define chuoiStart_Stop_Macro_MAX_VALUE_SIZE    3

bool chuoiStart_Stop_Macro_deCode(char paKey);

// class chuoiStart_Stop_Macro {
// public:
    char chuoiStart_Stop_Macro_term[chuoiStart_Stop_Macro_MAX_TERM_SIZE];
    char chuoiStart_Stop_Macro_key[chuoiStart_Stop_Macro_MAX_TERM_SIZE];
    char chuoiStart_Stop_Macro_values[chuoiStart_Stop_Macro_MAX_VALUE_SIZE][chuoiStart_Stop_Macro_MAX_TERM_SIZE];  // Giả sử có tối đa 3 giá trị
    int chuoiStart_Stop_Macro_curTermOffset = 0;
    int chuoiStart_Stop_Macro_curValueIndex = 0;
    int chuoiStart_Stop_Macro_parity = 0;
    bool chuoiStart_Stop_Macro_isChecksumTerm = false;
    bool chuoiStart_Stop_Macro_keyCaptured = false;      // Thêm biến này để theo dõi việc tách key
    bool chuoiStart_Stop_Macro_isStartSequence = false;  // Biến này để theo dõi chuỗi bắt đầu

    void chuoiStart_Stop_Macro_resetTerm() {
        memset(chuoiStart_Stop_Macro_term, 0, sizeof(chuoiStart_Stop_Macro_term));
        chuoiStart_Stop_Macro_curTermOffset = 0;
    }

    void chuoiStart_Stop_Macro_addTermToValues() {
        if (chuoiStart_Stop_Macro_curValueIndex < chuoiStart_Stop_Macro_MAX_VALUE_SIZE) {
            strcpy(chuoiStart_Stop_Macro_values[chuoiStart_Stop_Macro_curValueIndex], chuoiStart_Stop_Macro_term);
            chuoiStart_Stop_Macro_curValueIndex++;
        }
    }

    bool chuoiStart_Stop_Macro_deCode(char paKey) {
        static bool isProcessing = false;
        static int endSeqIndex = 0;  // Biến này để theo dõi chuỗi kết thúc
        if (!chuoiStart_Stop_Macro_isStartSequence) {
            // Kiểm tra chuỗi bắt đầu
            static int startSeqIndex = 0;
            const char* startSeq = "$#";  // OK
            // const char* startSeq = "$";
            if (paKey == startSeq[startSeqIndex]) {
                startSeqIndex++;
                if (startSeq[startSeqIndex] == '\0') {
                    chuoiStart_Stop_Macro_isStartSequence = true;
                    startSeqIndex = 0;  // Đặt lại chỉ số chuỗi bắt đầu
                    isProcessing = true;
                }
            } else {
                startSeqIndex = 0;  // Đặt lại nếu ký tự không khớp
            }
            return false;
        }
        if (isProcessing) {
            const char* endSeq = "*#";
            // const char* endSeq = "*";
            if (paKey == endSeq[endSeqIndex]) {
                endSeqIndex++;
                if (endSeq[endSeqIndex] == '\0') {
                    // Chuỗi kết thúc đã được nhận diện hoàn chỉnh
                    chuoiStart_Stop_Macro_term[chuoiStart_Stop_Macro_curTermOffset] = '\0';
                    chuoiStart_Stop_Macro_addTermToValues();
                    chuoiStart_Stop_Macro_isChecksumTerm = true;
                    chuoiStart_Stop_Macro_isStartSequence = false;  // Đặt lại chuỗi bắt đầu cho lần sau
                    isProcessing = false;
                    endSeqIndex = 0;  // Đặt lại chỉ số chuỗi kết thúc
                    return true;  // End of sentence, return true to indicate completion
                }
            } else {
                endSeqIndex = 0;  // Đặt lại nếu ký tự không khớp
            }
            if (endSeqIndex == 0) {  // Chỉ xử lý ký tự khi không đang kiểm tra chuỗi kết thúc
                switch (paKey) {
                case ',':
                    chuoiStart_Stop_Macro_parity ^= (uint8_t)paKey;
                    chuoiStart_Stop_Macro_term[chuoiStart_Stop_Macro_curTermOffset] = '\0';
                    if (!chuoiStart_Stop_Macro_keyCaptured) {
                        strcpy(chuoiStart_Stop_Macro_key, chuoiStart_Stop_Macro_term);
                        chuoiStart_Stop_Macro_keyCaptured = true;
                    } else {
                        chuoiStart_Stop_Macro_addTermToValues();
                    }
                    chuoiStart_Stop_Macro_resetTerm();
                    break;
                case '\r':
                case '\n':
                    break;
                default:
                    if (chuoiStart_Stop_Macro_curTermOffset < chuoiStart_Stop_Macro_MAX_TERM_SIZE - 1) {
                        chuoiStart_Stop_Macro_term[chuoiStart_Stop_Macro_curTermOffset++] = paKey;
                        chuoiStart_Stop_Macro_parity ^= paKey;
                    }
                    break;
                }
            }
        }
        return false;
    }

    void chuoiStart_Stop_Macro_resetForNext() {
        chuoiStart_Stop_Macro_curTermOffset = 0;
        chuoiStart_Stop_Macro_curValueIndex = 0;
        chuoiStart_Stop_Macro_parity = 0;
        memset(chuoiStart_Stop_Macro_key, 0, sizeof(chuoiStart_Stop_Macro_key));
        memset(chuoiStart_Stop_Macro_values, 0, sizeof(chuoiStart_Stop_Macro_values));
        chuoiStart_Stop_Macro_keyCaptured = 0;
        chuoiStart_Stop_Macro_isChecksumTerm = 0;
    }
// };

#endif