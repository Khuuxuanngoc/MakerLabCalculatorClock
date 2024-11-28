/**
 *
 *      Idea:
 *        - Phân tích chuỗi có format: $#key1,value1,value2,value3*#
 *
 *      result:
 *          - Khi nhập chuỗi "vs$#key1,value1,value2,value3*#$key2,value12,,value32*$#key3,value13,,,value43*#sb"
 *              - Ouput:
 *                      Key: key
 *                      Value 1: value1
 *                      Value 2: value2
 *                      Value 3: value3$key2
 *                      Key: key3
 *                      Value 1: value13
 *                      Value 2:
 *                      Value 3:
 *
 *          - Khi nhập chuỗi: $GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F
 *
 *
 */

#define MAX_TERM_SIZE (15)

class kxnTachData
{
public:
  char term[MAX_TERM_SIZE];
  char key[MAX_TERM_SIZE];
  char values[3][MAX_TERM_SIZE]; // Giả sử có tối đa 3 giá trị
  int curTermOffset = 0;
  int curValueIndex = 0;
  int parity = 0;
  bool isChecksumTerm = false;
  bool keyCaptured = false;     // Thêm biến này để theo dõi việc tách key
  bool isStartSequence = false; // Biến này để theo dõi chuỗi bắt đầu

  void resetTerm()
  {
    memset(term, 0, sizeof(term));
    curTermOffset = 0;
  }

  void addTermToValues()
  {
    if (curValueIndex < 3)
    {
      strcpy(values[curValueIndex], term);
      curValueIndex++;
    }
  }

  bool deCode(char paKey)
  {
    static bool isProcessing = false;
    static int endSeqIndex = 0; // Biến này để theo dõi chuỗi kết thúc

    if (!isStartSequence)
    {
      // Kiểm tra chuỗi bắt đầu
      static int startSeqIndex = 0;
      // const char* startSeq = "$#"; // OK
      const char *startSeq = "$";
      if (paKey == startSeq[startSeqIndex])
      {
        startSeqIndex++;
        if (startSeq[startSeqIndex] == '\0')
        {
          isStartSequence = true;
          startSeqIndex = 0; // Đặt lại chỉ số chuỗi bắt đầu
          isProcessing = true;
        }
      }
      else
      {
        startSeqIndex = 0; // Đặt lại nếu ký tự không khớp
      }
      return false;
    }

    if (isProcessing)
    {
      // const char* endSeq = "*#";
      const char *endSeq = "*";
      if (paKey == endSeq[endSeqIndex])
      {
        endSeqIndex++;
        if (endSeq[endSeqIndex] == '\0')
        {
          // Chuỗi kết thúc đã được nhận diện hoàn chỉnh
          term[curTermOffset] = '\0';
          addTermToValues();
          isChecksumTerm = true;
          isStartSequence = false; // Đặt lại chuỗi bắt đầu cho lần sau
          isProcessing = false;
          endSeqIndex = 0; // Đặt lại chỉ số chuỗi kết thúc
          return true;     // End of sentence, return true to indicate completion
        }
      }
      else
      {
        endSeqIndex = 0; // Đặt lại nếu ký tự không khớp
      }

      if (endSeqIndex == 0)
      { // Chỉ xử lý ký tự khi không đang kiểm tra chuỗi kết thúc
        switch (paKey)
        {
        case ',':
          parity ^= (uint8_t)paKey;
          term[curTermOffset] = '\0';
          if (!keyCaptured)
          {
            strcpy(key, term);
            keyCaptured = true;
          }
          else
          {
            addTermToValues();
          }
          resetTerm();
          break;

        case '\r':
        case '\n':
          break;

        default:
          if (curTermOffset < MAX_TERM_SIZE - 1)
          {
            term[curTermOffset++] = paKey;
            parity ^= paKey;
          }
          break;
        }
      }
    }
    return false;
  }

  void resetForNext()
  {
    curTermOffset = 0;
    curValueIndex = 0;
    parity = 0;
    memset(key, 0, sizeof(key));
    memset(values, 0, sizeof(values));
    keyCaptured = false;
    isChecksumTerm = false;
  }
};

class kxnTachData_T2
{
public:
  char term[MAX_TERM_SIZE];
  char key[MAX_TERM_SIZE];
  char values[3][MAX_TERM_SIZE]; // Giả sử có tối đa 3 giá trị
  int curTermOffset = 0;
  int curValueIndex = 0;
  int parity = 0;
  bool isChecksumTerm = false;
  bool keyCaptured = false;     // Thêm biến này để theo dõi việc tách key
  bool isStartSequence = false; // Biến này để theo dõi chuỗi bắt đầu

  void resetTerm()
  {
    memset(term, 0, sizeof(term));
    curTermOffset = 0;
  }

  void addTermToValues()
  {
    if (curValueIndex < 3)
    {
      strcpy(values[curValueIndex], term);
      curValueIndex++;
    }
  }

  bool deCode(char paKey)
  {
    static bool isProcessing = false;
    static int endSeqIndex = 0; // Biến này để theo dõi chuỗi kết thúc

    if (!isStartSequence)
    {
      // Kiểm tra chuỗi bắt đầu
      static int startSeqIndex = 0;
      const char* startSeq = "$#"; // OK
      // const char *startSeq = "$";
      if (paKey == startSeq[startSeqIndex])
      {
        startSeqIndex++;
        if (startSeq[startSeqIndex] == '\0')
        {
          isStartSequence = true;
          startSeqIndex = 0; // Đặt lại chỉ số chuỗi bắt đầu
          isProcessing = true;
        }
      }
      else
      {
        startSeqIndex = 0; // Đặt lại nếu ký tự không khớp
      }
      return false;
    }

    if (isProcessing)
    {
      const char* endSeq = "*#";
      // const char *endSeq = "*";
      if (paKey == endSeq[endSeqIndex])
      {
        endSeqIndex++;
        if (endSeq[endSeqIndex] == '\0')
        {
          // Chuỗi kết thúc đã được nhận diện hoàn chỉnh
          term[curTermOffset] = '\0';
          addTermToValues();
          isChecksumTerm = true;
          isStartSequence = false; // Đặt lại chuỗi bắt đầu cho lần sau
          isProcessing = false;
          endSeqIndex = 0; // Đặt lại chỉ số chuỗi kết thúc
          return true;     // End of sentence, return true to indicate completion
        }
      }
      else
      {
        endSeqIndex = 0; // Đặt lại nếu ký tự không khớp
      }

      if (endSeqIndex == 0)
      { // Chỉ xử lý ký tự khi không đang kiểm tra chuỗi kết thúc
        switch (paKey)
        {
        case ',':
          parity ^= (uint8_t)paKey;
          term[curTermOffset] = '\0';
          if (!keyCaptured)
          {
            strcpy(key, term);
            keyCaptured = true;
          }
          else
          {
            addTermToValues();
          }
          resetTerm();
          break;

        case '\r':
        case '\n':
          break;

        default:
          if (curTermOffset < MAX_TERM_SIZE - 1)
          {
            term[curTermOffset++] = paKey;
            parity ^= paKey;
          }
          break;
        }
      }
    }
    return false;
  }

  void resetForNext()
  {
    curTermOffset = 0;
    curValueIndex = 0;
    parity = 0;
    memset(key, 0, sizeof(key));
    memset(values, 0, sizeof(values));
    keyCaptured = false;
    isChecksumTerm = false;
  }
};
#include "chuoiStart_Stop_Macro.h"

kxnTachData kxnTachData1;

kxnTachData_T2  kxnTachData_T2_1;
// chuoiStart_Stop_Macro kxnTachData_T2_1;
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
//   static bool isProcessing = false;
//   static int endSeqIndex = 0; // Biến này để theo dõi chuỗi kết thúc

//   if (!isStartSequence) {
//     // Kiểm tra chuỗi bắt đầu
//     static int startSeqIndex = 0;
//     // const char* startSeq = "$#"; // OK
//     const char* startSeq = "$";
//     if (paKey == startSeq[startSeqIndex]) {
//       startSeqIndex++;
//       if (startSeq[startSeqIndex] == '\0') {
//         isStartSequence = true;
//         startSeqIndex = 0; // Đặt lại chỉ số chuỗi bắt đầu
//         isProcessing = true;
//       }
//     } else {
//       startSeqIndex = 0; // Đặt lại nếu ký tự không khớp
//     }
//     return false;
//   }

//   if (isProcessing) {
//     // const char* endSeq = "*#";
//     const char* endSeq = "*";
//     if (paKey == endSeq[endSeqIndex]) {
//       endSeqIndex++;
//       if (endSeq[endSeqIndex] == '\0') {
//         // Chuỗi kết thúc đã được nhận diện hoàn chỉnh
//         term[curTermOffset] = '\0';
//         addTermToValues();
//         isChecksumTerm = true;
//         isStartSequence = false; // Đặt lại chuỗi bắt đầu cho lần sau
//         isProcessing = false;
//         endSeqIndex = 0; // Đặt lại chỉ số chuỗi kết thúc
//         return true; // End of sentence, return true to indicate completion
//       }
//     } else {
//       endSeqIndex = 0; // Đặt lại nếu ký tự không khớp
//     }

//     if (endSeqIndex == 0) { // Chỉ xử lý ký tự khi không đang kiểm tra chuỗi kết thúc
//       switch (paKey) {
//         case ',':
//           parity ^= (uint8_t)paKey;
//           term[curTermOffset] = '\0';
//           if (!keyCaptured) {
//             strcpy(key, term);
//             keyCaptured = true;
//           } else {
//             addTermToValues();
//           }
//           resetTerm();
//           break;

//         case '\r':
//         case '\n':
//           break;

//         default:
//           if (curTermOffset < MAX_TERM_SIZE - 1) {
//             term[curTermOffset++] = paKey;
//             parity ^= paKey;
//           }
//           break;
//       }
//     }
//   }
//   return false;
// }

void setup()
{
  Serial.begin(115200);
  Serial.println(F("TestCommand Start!"));
}

void loop()
{
  if (Serial.available())
  {
    char tempChar = Serial.read();
    if (kxnTachData1.deCode(tempChar))
    {
      Serial.print("Key: ");
      Serial.println(kxnTachData1.key);
      for (int i = 0; i < kxnTachData1.curValueIndex; ++i)
      {
        Serial.print("Value ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(kxnTachData1.values[i]);
      }
      // Reset lại để xử lý lệnh tiếp theo
      kxnTachData1.resetForNext();
      // curTermOffset = 0;
      // curValueIndex = 0;
      // parity = 0;
      // memset(key, 0, sizeof(key));
      // memset(values, 0, sizeof(values));
      // keyCaptured = false;
      // isChecksumTerm = false;
    }

    if (kxnTachData_T2_1.deCode(tempChar))
    {
      Serial.print("Key: ");
      Serial.println(kxnTachData_T2_1.key);
      for (int i = 0; i < kxnTachData_T2_1.curValueIndex; ++i)
      {
        Serial.print("Value ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(kxnTachData_T2_1.values[i]);
      }
      // Reset lại để xử lý lệnh tiếp theo
      kxnTachData_T2_1.resetForNext();
      
    }

    if (KXN_DATA_DECODE(chuoiStart_Stop_Macro, tempChar))
    {
      Serial.print("Key: ");
      Serial.println(KXN_DATA_KEY(chuoiStart_Stop_Macro));
      for (int i = 0; i < KXN_DATA_CUR_INDEX(chuoiStart_Stop_Macro); ++i)
      {
        Serial.print("Value ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(KXN_DATA_VALUE(chuoiStart_Stop_Macro,i));
      }
      // Reset lại để xử lý lệnh tiếp theo
      // kxnTachData_T2_1.resetForNext();
      KXN_DATA_RESET(chuoiStart_Stop_Macro);
      
    }
  }
}
