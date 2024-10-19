// /**
//  * 
//  * ID: AKfycbynjsNHRsC4iDvggHIEGwz2l3i1ii25ZPK-bkBiBiEJHqbaMbLXynJTDuMeiQuwg8QM
//  * 
//  * URL: https://script.google.com/macros/s/AKfycbynjsNHRsC4iDvggHIEGwz2l3i1ii25ZPK-bkBiBiEJHqbaMbLXynJTDuMeiQuwg8QM/exec
//  * 
//  */

// //----------------------------------------Including the libraries.
// #include "WiFi.h"
// #include <HTTPClient.h>
// #include "MKL_DHT.h"
// //----------------------------------------

// // Defining LED PINs on the ESP32 Board.
// #define On_Board_LED_PIN  2

// // Defines the DHT11 PIN and DHT sensor type.
// #define DHTPIN  4
// #define DHTTYPE DHT11

// // Defines the PIN for the Switches.
// #define Switch_1_PIN   13
// #define Switch_2_PIN   12

// //----------------------------------------SSID and PASSWORD of your WiFi network.
// const char* ssid = "REPLACE_WITH_YOUR_SSID";  //--> Your wifi name
// const char* password = "REPLACE_WITH_YOUR_PASSWORD"; //--> Your wifi password
// //----------------------------------------

// // Google script Web_App_URL.
// String Web_App_URL = "REPLACE_WITH_YOUR_WEB_APP_URL";

// String Switch_1_State = "";
// String Switch_2_State = "";

// String Status_Read_Sensor = "";
// float Temp;
// int Humd;

// // Initialize DHT as dht11.
// MKL_DHT dht11(DHTPIN, DHTTYPE);

// //________________________________________________________________________________Getting_DHT11_Sensor_Data()
// // Subroutine for getting temperature and humidity data from the DHT11 sensor.
// void Getting_DHT11_Sensor_Data() {
//   // Reading temperature or humidity takes about 250 milliseconds!
//   // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
//   Humd = dht11.readHumidity();
//   // Read temperature as Celsius (the default)
//   Temp = dht11.readTemperature();

//   // Check if any reads failed and exit early (to try again).
//   if (isnan(Humd) || isnan(Temp)) {
//     Serial.println();
//     Serial.println(F("Failed to read from DHT sensor!"));
//     Serial.println();

//     Status_Read_Sensor = "Failed";
//     Temp = 0.00;
//     Humd = 0;
//   } else {
//     Status_Read_Sensor = "Success";
//   }

//   Serial.println();
//   Serial.println("-------------");
//   Serial.print(F("Status_Read_Sensor : "));
//   Serial.print(Status_Read_Sensor);
//   Serial.print(F(" | Humidity : "));
//   Serial.print(Humd);
//   Serial.print(F("% | Temperature : "));
//   Serial.print(Temp);
//   Serial.println(F("°C"));
//   Serial.println("-------------");
// }
// //________________________________________________________________________________

// //________________________________________________________________________________Read_Switches_State()
// // Subroutine to get the state of the Switches.
// void Read_Switches_State() {
//   if (digitalRead(Switch_1_PIN) == LOW)  Switch_1_State = "On";
//   if (digitalRead(Switch_1_PIN) == HIGH) Switch_1_State = "Off";

//   if (digitalRead(Switch_2_PIN) == LOW)  Switch_2_State = "On";
//   if (digitalRead(Switch_2_PIN) == HIGH) Switch_2_State = "Off";

//   Serial.println();
//   Serial.println("-------------");
//   Serial.print(F("Switch_1 : "));
//   Serial.print(Switch_1_State);
//   Serial.print(F(" | Switch_2 : "));
//   Serial.println(Switch_2_State);
//   Serial.println("-------------");
// }
// //________________________________________________________________________________

// //________________________________________________________________________________VOID SETUP()
// void setup() {
//   // put your setup code here, to run once:

//   Serial.begin(115200);
//   Serial.println();
//   delay(1000);

//   pinMode(Switch_1_PIN, INPUT_PULLUP);
//   pinMode(Switch_2_PIN, INPUT_PULLUP);

//   pinMode(On_Board_LED_PIN, OUTPUT);

//   //----------------------------------------Set Wifi to STA mode
//   Serial.println();
//   Serial.println("-------------");
//   Serial.println("WIFI mode : STA");
//   WiFi.mode(WIFI_STA);
//   Serial.println("-------------");
//   //---------------------------------------- 

//   //----------------------------------------Connect to Wi-Fi (STA).
//   Serial.println();
//   Serial.println("------------");
//   Serial.print("Connecting to ");
//   Serial.println(ssid);
//   WiFi.begin(ssid, password);

//   //:::::::::::::::::: The process of connecting ESP32 with WiFi Hotspot / WiFi Router.
//   // The process timeout of connecting ESP32 with WiFi Hotspot / WiFi Router is 20 seconds.
//   // If within 20 seconds the ESP32 has not been successfully connected to WiFi, the ESP32 will restart.
//   // I made this condition because on my ESP32, there are times when it seems like it can't connect to WiFi, so it needs to be restarted to be able to connect to WiFi.

//   int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
//   connecting_process_timed_out = connecting_process_timed_out * 2;
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print(".");
//     digitalWrite(On_Board_LED_PIN, HIGH);
//     delay(250);
//     digitalWrite(On_Board_LED_PIN, LOW);
//     delay(250);
//     if (connecting_process_timed_out > 0) connecting_process_timed_out--;
//     if (connecting_process_timed_out == 0) {
//       delay(1000);
//       ESP.restart();
//     }
//   }

//   digitalWrite(On_Board_LED_PIN, LOW);
  
//   Serial.println();
//   Serial.println("WiFi connected");
//   Serial.println("------------");
//   //::::::::::::::::::
//   //----------------------------------------

//   delay(100);

//   Serial.println();
//   Serial.println("DHT11 Begin");
//   Serial.println();
//   delay(1000);
  
//   dht11.begin();

//   delay(2000);
// }

//  void loop() {
//   // Lấy dữ liệu từ cảm biến và công tắc
//   Getting_DHT11_Sensor_Data();
//   Read_Switches_State();

//   if (WiFi.status() == WL_CONNECTED) {
//     digitalWrite(On_Board_LED_PIN, HIGH);

//     // Tạo URL với tham số sheet_id và sheet_name
//     String Send_Data_URL = Web_App_URL + "?sts=write";
//     Send_Data_URL += "&sheet_id=YOUR_SHEET_ID";  // Thay YOUR_SHEET_ID bằng ID trang tính bạn muốn
//     Send_Data_URL += "&sheet_name=YOUR_SHEET_NAME";  // Thay YOUR_SHEET_NAME bằng tên trang tính bạn muốn
//     Send_Data_URL += "&srs=" + Status_Read_Sensor;
//     Send_Data_URL += "&temp=" + String(Temp);
//     Send_Data_URL += "&humd=" + String(Humd);
//     Send_Data_URL += "&swtc1=" + Switch_1_State;
//     Send_Data_URL += "&swtc2=" + Switch_2_State;

//     Serial.println();
//     Serial.println("-------------");
//     Serial.println("Send data to Google Spreadsheet...");
//     Serial.print("URL : ");
//     Serial.println(Send_Data_URL);

//     // Gửi yêu cầu HTTP
//     HTTPClient http;
//     http.begin(Send_Data_URL.c_str());
//     int httpCode = http.GET(); 
//     if (httpCode > 0) {
//       String payload = http.getString();
//       Serial.println("Payload : " + payload);    
//     }
//     http.end();

//     digitalWrite(On_Board_LED_PIN, LOW);
//     Serial.println("-------------");
//   }

//   delay(10000);  // Delay 10 giây
// }
