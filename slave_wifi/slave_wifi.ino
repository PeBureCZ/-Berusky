//STM32 bzucak - client
#include <WiFi.h>
#include <MFRC522.h> //library responsible for communicating with the module mfrc522-RC522
#include <esp_wifi.h> //allow esp_wifi_set_max_tx_power(20);

//wifi
const char* ssid     = "berusky";
const char* password = "neprolomitelne";     
IPAddress IP (192, 168, 4, 1);    
WiFiClient client;

int status = WL_IDLE_STATUS;

bool testChecking = false;

const uint8_t SOUND_ITEM = 13;

#include <SPI.h> //library responsible for communicating of SPI bus
#define SS_PIN    21
#define RST_PIN   22
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16

MFRC522::MIFARE_Key key;
MFRC522::StatusCode wifiStatus;
MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup()
{
    Serial.begin(115200);
    pinMode(SOUND_ITEM, OUTPUT);
    digitalWrite(SOUND_ITEM, HIGH);
    SPI.begin(); // Init SPI bus
    //  // Init MFRC522
    mfrc522.PCD_Init(); 
    Serial.println("\nConnecting");
    Serial.println(get_wifi_status(status));
    WiFi.begin(ssid, password);

    esp_wifi_set_max_tx_power(200);
    Serial.print("dBm = ");
    Serial.print(WiFi.getTxPower());
    Serial.print(" / 4 \n"); 
}

void rfidCheck()
{
  if (!testChecking) 
  {
      testChecking = true;
      Serial.print("\nchecking now\n");
  }
   // Aguarda a aproximacao do cartao
   //waiting the card approach
  if ( !mfrc522.PICC_IsNewCardPresent()) 
  {
    //Serial.print("test1\n");
    return;
  }
  // Select a card
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    //Serial.print("test2\n");
    return;
  }
  
  // výpis informace o verzi mfrc522 tagu
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print("mfrc522 tag typu: ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  // kontrola podporovaných typů mfrc522 tagu
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println("Tento mfrc522 tag neni podporovany (typ MIFARE Classic).");
    return;
  }
  // výpis adresy mfrc522 tagu v hexa formátu
  Serial.print("Adresa mfrc522 tagu: ");
  vypisHex(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();

  if(mfrc522.uid.uidByte[0] == 0x80 & mfrc522.uid.uidByte[1] == 0x71 & mfrc522.uid.uidByte[2] == 0xD8 & mfrc522.uid.uidByte[3] == 0x55) 
  {
  }
  else if(mfrc522.uid.uidByte[0] == 0xF3 & mfrc522.uid.uidByte[1] == 0xA5 & mfrc522.uid.uidByte[2] == 0xBD & mfrc522.uid.uidByte[3] == 0xC8) 
  {
  }
  else 
  {
    Serial.println("Detekovan neznamy mfrc522 tag!");
    sendData(38,39);
  }

  Serial.println();
  // ukončení komunikace a jejího zabezpečení
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

// podprogram pro výpis adresy mfrc522 tagu v hexa formátu
void vypisHex(byte *buffer, byte bufferSize) 
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
 
void loop()
{
  delay(200);
  status = WiFi.status();
  if (status == WL_CONNECTED)
  {
    //sendData(4);
    rfidCheck();
  }
  else
  {
    testChecking = false;
    while(status != WL_CONNECTED)
    {
      status = WiFi.status();
      Serial.println(get_wifi_status(status));
      delay(1000);
    }
  }
  
  // if(client.connect(IP , 24)) 
  // {
  //   Serial.print("\nclient yes");   
  // }
  // else Serial.print("\nclient no");
}

void sendData(uint8_t firstData, uint8_t secondData)
{
  if (client.connect(IP , 24))
  {
    uint8_t dataStr[2];
    dataStr[0] = firstData;
    dataStr[1] = secondData;
    Serial.print("Sending data: ");
    Serial.print(dataStr[0]);
    client.write(dataStr,sizeof(dataStr));
    Serial.print(" + ");
    Serial.print(dataStr[1]);
    client.flush();
        for(int i = 0; i < 4; i++)
    {
      digitalWrite(SOUND_ITEM, LOW);
      delay(500);
      digitalWrite(SOUND_ITEM, HIGH);
      delay(500);
    }
    client.stop();
    Serial.print("end send");
  }
  else Serial.print("no send");
}


