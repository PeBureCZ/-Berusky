
//Esp server (deska s diodou)

#include <WiFi.h>
#include <esp_wifi.h>
#include <Preferences.h> //flash memory 

//const uint8_t BUILD_IN_DIODE = 2;

WiFiServer server(24);
const char *ssid = "berusky";
const char *password = "neprolomitelne";

Preferences preferences; //FLASH MEMORY INICIALIZATION

void recieveData(WiFiClient client)
{
  uint8_t dataStr[2]; 
  client.readBytes(dataStr, 2);
  Serial.print("\nReceived data: ");
  Serial.print(dataStr[0]);
  Serial.print("+");
  Serial.print(dataStr[1]);
}