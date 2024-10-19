// /**
//  * 
//  *      Idea:
//  *        - Phân tích chuỗi có format: $key,value1,value2,value3*
//  */

// #include "codetest_3.h"
// #include "CodeTest_danhDauChuoiBatDau.h"
#include "CodeTest_danhDauChuoiBatDau_ChuoiDanhDauKetThuc.h"

// // #define MAX_TERM_SIZE   (15)

// // char term[];
// // void setup() {
// //   Serial.begin(115200);
// //   Serial.println(F("TestCommand Start!"));
// // }

// // void loop() {
// //   if(Serial.available()){
// //     char tempChar = Serial.read();
// //     deCode(tempChar);

// //     Serial.println(key);
// //   }
// // }

// // int deCode(char paKey) {
// //   switch (paKey) {
// //     case ',':  // term terminators
// //       parity ^= (uint8_t)c;
// //     case '\r':
// //     case '\n':
// //     case '*':
      
// //       break;

// //     case '$':  // sentence begin
// //       // curTermNumber = curTermOffset = 0;
// //       // parity = 0;
// //       // curSentenceType = GPS_SENTENCE_OTHER;
// //       // isChecksumTerm = false;
// //       // sentenceHasFix = false;
// //       // return false;

// //     default:  // ordinary characters
// //       // if (curTermOffset < sizeof(term) - 1)
// //       //   term[curTermOffset++] = c;
// //       // if (!isChecksumTerm)
// //       //   parity ^= c;
// //       // return false;
// //   }

// //   return false;
// // }

// // #define MAX_TERM_SIZE (15)
// // char term[MAX_TERM_SIZE];
// // char key[MAX_TERM_SIZE];
// // char values[3][MAX_TERM_SIZE]; // Giả sử có tối đa 3 giá trị
// // int curTermOffset = 0;
// // int curValueIndex = 0;
// // int parity = 0;
// // bool isChecksumTerm = false;

// // void resetTerm() {
// //   memset(term, 0, sizeof(term));
// //   curTermOffset = 0;
// // }

// // void addTermToValues() {
// //   if (curValueIndex < 3) {
// //     strcpy(values[curValueIndex], term);
// //     curValueIndex++;
// //   }
// // }

// // int deCode(char paKey) {
// //   switch (paKey) {
// //     case ',':
// //       parity ^= (uint8_t)paKey;
// //       term[curTermOffset] = '\0';
// //       if (curValueIndex == 0) {
// //         strcpy(key, term);
// //       } else {
// //         addTermToValues();
// //       }
// //       resetTerm();
// //       break;

// //     case '\r':
// //     case '\n':
// //       break;

// //     case '*':
// //       term[curTermOffset] = '\0';
// //       addTermToValues();
// //       isChecksumTerm = true;
// //       break;

// //     case '$':
// //       curTermOffset = 0;
// //       curValueIndex = 0;
// //       parity = 0;
// //       memset(key, 0, sizeof(key));
// //       memset(values, 0, sizeof(values));
// //       resetTerm();
// //       isChecksumTerm = false;
// //       break;

// //     default:
// //       if (curTermOffset < MAX_TERM_SIZE - 1) {
// //         term[curTermOffset++] = paKey;
// //         parity ^= paKey;
// //       }
// //       break;
// //   }
// //   return false;
// // }

// // void setup() {
// //   Serial.begin(115200);
// //   Serial.println(F("TestCommand Start!"));
// // }

// // void loop() {
// //   if(Serial.available()){
// //     char tempChar = Serial.read();
// //     deCode(tempChar);

// //     Serial.println(key);
// //   }
// // }

// #define MAX_TERM_SIZE (15)
// char term[MAX_TERM_SIZE];
// char key[MAX_TERM_SIZE];
// char values[3][MAX_TERM_SIZE]; // Giả sử có tối đa 3 giá trị
// int curTermOffset = 0;
// int curValueIndex = 0;
// int parity = 0;
// bool isChecksumTerm = false;

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
//   switch (paKey) {
//     case ',':
//       parity ^= (uint8_t)paKey;
//       term[curTermOffset] = '\0';
//       if (curValueIndex == 0) {
//         strcpy(key, term);
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
//       return true; // End of sentence, return true to indicate completion
//       break;

//     case '$':
//       curTermOffset = 0;
//       curValueIndex = 0;
//       parity = 0;
//       memset(key, 0, sizeof(key));
//       memset(values, 0, sizeof(values));
//       resetTerm();
//       isChecksumTerm = false;
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