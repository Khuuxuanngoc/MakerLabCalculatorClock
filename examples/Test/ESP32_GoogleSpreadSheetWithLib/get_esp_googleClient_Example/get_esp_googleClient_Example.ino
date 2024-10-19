
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt
 *
 * Copyright (c) 2023 mobizt
 *
 */

// This example shows how to get the spreadsheet.

#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif

// For SD/SD_MMC mounting helper
#include <GS_SDHelper.h>

#define WIFI_SSID "nk"
#define WIFI_PASSWORD "30121992"

// For how to create Service Account and how to use the library, go to https://github.com/mobizt/ESP-Google-Sheet-Client

#define PROJECT_ID "espsigner-438305"

// Service Account's client email
#define CLIENT_EMAIL "kxnesp32name@espsigner-438305.iam.gserviceaccount.com"

// Service Account's private key
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDT+RdgGrY/uewq\nz8rc42ae6umnxLeQrzPuIb4xlqYjIenkLlO3b1yQaut9wFhSbfkbkJ0A+DjOeYHH\nR9y1Mf1/CjATERPnB5QA3LdVb9qWl8V2YqvYBfzPgG8OcwcOW/s/dgk6jgMQ6mEE\nEj0RV2j5+nMSWUoPjW87dOYwTqPfAwbnXCcjF2tSu7dRQKAHONNxa9Pl4oWrrIq5\nYm9DftHDNxCUK6TlM4ioIPf0lrdQfUjbCmAcgXDtWNeBzBOY4pH6c57Fm7uyh5Ca\nbjdqTBSWWsZ9epLWalYXRI+pN8u96yG2EWwjXVszm8jyLW/BWP8akAkUkM4C+LsV\nHR7D7qD5AgMBAAECggEAFpPMivSRZpR6nH09J92TMbhG6pMD88XLf9DJgZ3ghHOx\nZjfOfCZg+zNeMYrOBXqX6rkSlfa6UzijK2sCrax9Sf2XbQJ3xpbaq2RW5SNYqotQ\nEYnVMJlDqgTNn2H+VYfTyRi9Byb22eOC8ZRxO3J+WIjV3pWS0zj+6NvsxCauItut\nkjYoLY9VkQrGJZ8gIYvkLeZOK1Hv3/+vLOjpWm1n5aVyOLPp2bMv2f7tTVK1Mgqw\nV8Y5X3gD6R/p+o4gQtwJ/EGqzk0DR1VkkSpIQz8wlsETqYR8vwwqT+Gg3IyacVYB\n6xbhduc8nfcf95CKsRkiw3Sturz7YZLla6rnVTLC0QKBgQD44eVpxTMYMl3u5yMS\nh0YD9s00xnDRlaUV0SrHghSiP8td3PdBC1NZ1cCXpzBpMR9CqI7RmBFhbZJf/LUJ\nuF92PxmdDZxtaBfAn7lRpId6S31XPNDuDMcLAQYPkYPg+13EMZ41kMQJsDXH+urz\ngjJXkjeDliJjiqIyyZ//O6wt8QKBgQDaCPnnlanvRrJfz0RLXYBZ3Pip2sgPQFzf\nr3AZ35Q62luwqAtH5YS3Js8HoOuXqwzLg+IdjIrRnrUOezwKW9+QsYpLlPPYx0Ks\ntdeuXtRmzck1DA6hXKZbLjs1iCuV3H8ZTxP7X+TaVTimPZY7EkgjdMOMZ8p8nyzP\nohQaE+u7iQKBgQD2mZ1ed+5wgzdfh9XQfgh51QjuydDvWX8BZwckN1eKi/tNPawf\n3sDvukPh/96P3ARFYA5iznIqPw9R3Grk/rJXz53pZ5sfLIycZ6aPpp42JqZC0heS\nR0bROGpw8pXhTi9SPnesS4xyrQ4c4Bn8vXx0GIS49QGN78dW20CU/ONVgQKBgGdM\nA2G1ZUooW0/K+ZcfGDT63JBUR536J0YObvTDGQ387521L4sDcSn4Z/M8SQGVFz3A\nILtRH1+KaksPFXXu6u85PcTrL8/PL4Z1rFY/NS/zZNUN/kxygZoHQCPvfH7GdSy/\naz8XwmuyHmGcIfWghq7NneuCLMt3+hnwtsdYEsHJAoGAJauu4vtAIcm3zARNqKxE\nOmnbk96Jq3rk4vRXCYyZkMSMj5Xs5lmwxAD/LsZFlfPZJltbd8zjPs3tadCprxt2\nosodjLoxyj7rpSDRLxNjxjCLIArAZdJcfshbYtsL1x0Thzi63sDy0rjDpuU5DmKB\nfJeOjwyV8TT0CyGQ2EAz/NQ=\n-----END PRIVATE KEY-----\n";


#define KXN_spreadsheetId     "13nJreQu01atSvm8adQbSB-ADX0ouvAeq5cdGlEmrEr8"

/**
const char rootCACert[] PROGMEM = "-----BEGIN CERTIFICATE-----\n"
                                  "MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw\n"
                                  "CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n"
                                  "MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n"
                                  "MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n"
                                  "Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA\n"
                                  "A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo\n"
                                  "27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w\n"
                                  "Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw\n"
                                  "TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl\n"
                                  "qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH\n"
                                  "szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8\n"
                                  "Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk\n"
                                  "MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92\n"
                                  "wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p\n"
                                  "aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN\n"
                                  "VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID\n"
                                  "AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n"
                                  "FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb\n"
                                  "C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe\n"
                                  "QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy\n"
                                  "h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4\n"
                                  "7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J\n"
                                  "ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef\n"
                                  "MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/\n"
                                  "Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT\n"
                                  "6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ\n"
                                  "0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm\n"
                                  "2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb\n"
                                  "bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n"
                                  "-----END CERTIFICATE-----\n";
*/

bool taskComplete = false;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

void tokenStatusCallback(TokenInfo info);

void setup()
{

    Serial.begin(115200);
    Serial.println();
    Serial.println();

    GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

#if defined(ESP32) || defined(ESP8266)
    WiFi.setAutoReconnect(true);
#endif

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    multi.addAP(WIFI_SSID, WIFI_PASSWORD);
    multi.run();
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

    Serial.print("Connecting to Wi-Fi");
    unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        if (millis() - ms > 10000)
            break;
#endif
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    // In case SD/SD_MMC storage file access, mount the SD/SD_MMC card.
    // SD_Card_Mounting(); // See src/GS_SDHelper.h

    // GSheet.setCert(rootCACert); // or GSheet.setCertFile("path/to/certificate/file.pem", esp_google_sheet_file_storage_type_flash /* or esp_google_sheet_file_storage_type_sd */);

    // Set the callback for Google API access token generation status (for debug only)
    GSheet.setTokenCallback(tokenStatusCallback);

    // The WiFi credentials are required for Pico W
    // due to it does not have reconnect feature.
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    GSheet.clearAP();
    GSheet.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

    // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
    GSheet.setPrerefreshSeconds(10 * 60);

    // Begin the access token generation for Google API authentication
    GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);

    // Or begin with the Service Account JSON file
    // GSheet.begin("path/to/serviceaccount/json/file", esp_google_sheet_file_storage_type_flash /* or esp_google_sheet_file_storage_type_sd */);
}

void loop()
{
    // Call ready() repeatedly in loop for authentication checking and processing
    bool ready = GSheet.ready();

    if (ready && !taskComplete)
    {
        // For basic FirebaseJson usage example, see examples/FirebaseJson/Create_Edit_Parse/Create_Edit_Parse.ino

        FirebaseJson response;
        // Instead of using FirebaseJson for response, you can use String for response to the functions
        // especially in low memory device that deserializing large JSON response may be failed as in ESP8266

        Serial.println("Get spreadsheet...");
        Serial.println("------------------------");

        // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets/get

        bool success = GSheet.get(&response /* returned response */, KXN_spreadsheetId /* spreadsheet Id to request */);
        response.toString(Serial, true);
        Serial.println();

        Serial.println("\nGet spreadsheet by data filter...");
        Serial.println("----------------------------------");

        FirebaseJsonArray dataFiltersArr;

        FirebaseJson dataFilters1;
        dataFilters1.add("a1Range", "Sheet1!A1:A3");
        dataFiltersArr.add(dataFilters1);

        // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets/getByDataFilter

        success = GSheet.getByDataFilter(&response /* returned response */, KXN_spreadsheetId /* spreadsheet Id to request */, &dataFiltersArr /* array of data range to request by filter */, "true" /* include grid data */);

        if (success)
            response.toString(Serial, true);
        else
            Serial.println(GSheet.errorReason());
        Serial.println();

#if defined(ESP32) || defined(ESP8266)
        Serial.println(ESP.getFreeHeap());
#elif defined(PICO_RP2040)
        Serial.println(rp2040.getFreeHeap());
#endif

        taskComplete = true;
    }
}

void tokenStatusCallback(TokenInfo info)
{
    if (info.status == token_status_error)
    {
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
        GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
    }
    else
    {
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    }
}