/**
 * 
 *      Idea:
 *        - Phân tích chuỗi có format: $#key1,value1,value2,value3*
 * 
 *      result:
 *          - Khi nhập chuỗi "bofvs$#key1,value1,value2,value3*$key2,value12,,value32*$#key3,value13,,,value43*sb h"
 *              - Ouput:
 *                      Key: key
 *                      Value 1: value1
 *                      Value 2: value2
 *                      Value 3: value3
 *                      Key: key3
 *                      Value 1: value13
 *                      Value 2: 
 *                      Value 3: 
 * 
 *
 *                       
 */

// #define MAX_TERM_SIZE (15)
// char term[MAX_TERM_SIZE];
// char key[MAX_TERM_SIZE];
// char values[3][MAX_TERM_SIZE]; // Giả sử có tối đa 3 giá trị
// int curTermOffset = 0;
// int curValueIndex = 0;
// int parity = 0;
// bool isChecksumTerm = false;
// bool keyCaptured = false; // Thêm biến này để theo dõi việc tách key
// bool isStartSequence = false; // Biến này để theo dõi chuỗi bắt đầu

// void resetTerm() {
//   memset(term, 0, sizeof(term));
//   curTermOffset = 0;
// }

// void addTermToValues() {
//   if (curValueIndex < 3) {
//     strcpy(values[curValueIndex], term);
//     curValueIndex++;
//   }
// }

// bool deCode(char paKey) {
//   if (!isStartSequence) {
//     // Kiểm tra chuỗi bắt đầu
//     static int startSeqIndex = 0;
//     const char* startSeq = "$#";
//     if (paKey == startSeq[startSeqIndex]) {
//       startSeqIndex++;
//       if (startSeq[startSeqIndex] == '\0') {
//         isStartSequence = true;
//         startSeqIndex = 0; // Đặt lại chỉ số chuỗi bắt đầu
//       }
//     } else {
//       startSeqIndex = 0; // Đặt lại nếu ký tự không khớp
//     }
//     return false;
//   }

//   switch (paKey) {
//     case ',':
//       parity ^= (uint8_t)paKey;
//       term[curTermOffset] = '\0';
//       if (!keyCaptured) {
//         strcpy(key, term);
//         keyCaptured = true;
//       } else {
//         addTermToValues();
//       }
//       resetTerm();
//       break;

//     case '\r':
//     case '\n':
//       break;

//     case '*':
//       term[curTermOffset] = '\0';
//       addTermToValues();
//       isChecksumTerm = true;
//       isStartSequence = false; // Đặt lại chuỗi bắt đầu cho lần sau
//       return true; // End of sentence, return true to indicate completion
//       break;

//     default:
//       if (curTermOffset < MAX_TERM_SIZE - 1) {
//         term[curTermOffset++] = paKey;
//         parity ^= paKey;
//       }
//       break;
//   }
//   return false;
// }

// void setup() {
//   Serial.begin(115200);
//   Serial.println(F("TestCommand Start!"));
// }

// void loop() {
//   if(Serial.available()) {
//     char tempChar = Serial.read();
//     if(deCode(tempChar)) {
//       Serial.print("Key: ");
//       Serial.println(key);
//       for (int i = 0; i < curValueIndex; ++i) {
//         Serial.print("Value ");
//         Serial.print(i + 1);
//         Serial.print(": ");
//         Serial.println(values[i]);
//       }
//     }
//   }
// }

////////////////////////////////////////////////////////////

#define MAX_TERM_SIZE (15)
char term[MAX_TERM_SIZE];
char key[MAX_TERM_SIZE];
char values[3][MAX_TERM_SIZE]; // Giả sử có tối đa 3 giá trị
int curTermOffset = 0;
int curValueIndex = 0;
int parity = 0;
bool isChecksumTerm = false;
bool keyCaptured = false; // Thêm biến này để theo dõi việc tách key
bool isStartSequence = false; // Biến này để theo dõi chuỗi bắt đầu

void resetTerm() {
  memset(term, 0, sizeof(term));
  curTermOffset = 0;
}

void addTermToValues() {
  if (curValueIndex < 3) {
    strcpy(values[curValueIndex], term);
    curValueIndex++;
  }
}

bool deCode(char paKey) {
  static bool isProcessing = false;

  if (!isStartSequence) {
    // Kiểm tra chuỗi bắt đầu
    static int startSeqIndex = 0;
    const char* startSeq = "$#";
    if (paKey == startSeq[startSeqIndex]) {
      startSeqIndex++;
      if (startSeq[startSeqIndex] == '\0') {
        isStartSequence = true;
        startSeqIndex = 0; // Đặt lại chỉ số chuỗi bắt đầu
        isProcessing = true;
      }
    } else {
      startSeqIndex = 0; // Đặt lại nếu ký tự không khớp
    }
    return false;
  }

  if (isProcessing) {
    switch (paKey) {
      case ',':
        parity ^= (uint8_t)paKey;
        term[curTermOffset] = '\0';
        if (!keyCaptured) {
          strcpy(key, term);
          keyCaptured = true;
        } else {
          addTermToValues();
        }
        resetTerm();
        break;

      case '\r':
      case '\n':
        break;

      case '*':
        term[curTermOffset] = '\0';
        addTermToValues();
        isChecksumTerm = true;
        isStartSequence = false; // Đặt lại chuỗi bắt đầu cho lần sau
        isProcessing = false;
        return true; // End of sentence, return true to indicate completion
        break;

      default:
        if (curTermOffset < MAX_TERM_SIZE - 1) {
          term[curTermOffset++] = paKey;
          parity ^= paKey;
        }
        break;
    }
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("TestCommand Start!"));
}

void loop() {
  if(Serial.available()) {
    char tempChar = Serial.read();
    if(deCode(tempChar)) {
      Serial.print("Key: ");
      Serial.println(key);
      for (int i = 0; i < curValueIndex; ++i) {
        Serial.print("Value ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(values[i]);
      }
      // Reset lại để xử lý lệnh tiếp theo
      curTermOffset = 0;
      curValueIndex = 0;
      parity = 0;
      memset(key, 0, sizeof(key));
      memset(values, 0, sizeof(values));
      keyCaptured = false;
      isChecksumTerm = false;
    }
  }
}

