//ESP32 - CHYTACKA (SLAVE STATION)
#include <WiFi.h>
#include <MFRC522.h> //library responsible for communicating with the module mfrc522-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#include <esp_wifi.h> //allow esp_wifi_set_max_tx_power(20);


//wifi
const char* ssid     = "berusky";
const char* password = "neprolomitelne";
   
IPAddress IP (192, 168, 4, 1);  //adress to connect

//each slave station must have unique ID for correct communication!
uint8_t slaveID = 0; //must not be bigger than 32!!!

WiFiClient client;

int status = WL_IDLE_STATUS;

const uint8_t WIFI_CONNECT = 13;
const uint8_t SYNCHRONIZED = 12;

const uint8_t SS_PIN = 21;
const uint8_t RST_PIN = 22;
MFRC522::MIFARE_Key key;
MFRC522::StatusCode wifiStatus;
MFRC522 mfrc522(SS_PIN, RST_PIN);

unsigned long long actualTime = 0; //milliseconds

#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16

// podprogram pro výpis adresy mfrc522 tagu v hexa formátu
void hexPrint(byte *buffer, byte bufferSize) 
{
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

String get_wifi_status(int status)
{
    switch(status)
    {
        case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
        case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
        case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
        case WL_CONNECTED:
        return "WL_CONNECTED";
        case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    }
}

unsigned int minLightOn = 5;
unsigned int maxLightOn = 20;
unsigned int minLightOff = 5;
unsigned int maxLightOff = 20;

byte groupArrayW[32] = 
{
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,         
  0x00, 0x00, 0x00, 0x00, 
};

byte groupArrayG[32] = 
{
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,         
  0x00, 0x00, 0x00, 0x00, 
};

byte groupArrayR[32] = 
{
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,         
  0x00, 0x00, 0x00, 0x00,  
};

byte groupArrayY[32] = 
{
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,         
  0x00, 0x00, 0x00, 0x00, 
};

