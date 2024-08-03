//STM32 bzucak - client
#include <WiFi.h>
#include <MFRC522.h> //library responsible for communicating with the module mfrc522-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#include <esp_wifi.h> //allow esp_wifi_set_max_tx_power(20);


//wifi
const char* ssid     = "berusky";
const char* password = "neprolomitelne";     
IPAddress IP (192, 168, 4, 1);    
WiFiClient client;

int status = WL_IDLE_STATUS;

const uint8_t WIFI_CONNECT = 13;
const uint8_t SYNCHRONIZED = 12;

const uint8_t SS_PIN = 21;
const uint8_t RST_PIN = 22;
MFRC522::MIFARE_Key key;
MFRC522::StatusCode wifiStatus;
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16

void rfidCheck()
{
  if ( !mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select a card
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    //Serial.print("test2\n");
    return;
  }
  
  // WRITE INFORMATION
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print("mfrc522 tag typu: ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  // CHECK SUPPORTED TYPES
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println("Tento mfrc522 tag neni podporovany (typ MIFARE Classic).");
    return;
  }
  // write hex id
  Serial.print("Adresa mfrc522 tagu: ");
  hexPrint(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();

  if(mfrc522.uid.uidByte[0] == 0x80 & mfrc522.uid.uidByte[1] == 0x71 & mfrc522.uid.uidByte[2] == 0xD8 & mfrc522.uid.uidByte[3] == 0x55) 
  {
  }
  else if(mfrc522.uid.uidByte[0] == 0xF3 & mfrc522.uid.uidByte[1] == 0xA5 & mfrc522.uid.uidByte[2] == 0xBD & mfrc522.uid.uidByte[3] == 0xC8) 
  {
  }
  else 
  {
    Serial.println("Detekovan neznamy mfrc522 tag! + send data");
    sendData(38,39);
  }

  Serial.println();
  // ukončení komunikace a jejího zabezpečení
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

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


