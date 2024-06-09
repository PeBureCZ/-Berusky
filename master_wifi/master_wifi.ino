//Esp server (deska s diodou)
#include <Nextion.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Preferences.h> //flash memory 

//const uint8_t BUILD_IN_DIODE = 2;

WiFiServer server(24);
const char *ssid = "berusky";
const char *password = "neprolomitelne";

HardwareSerial nextionSerial(2);

NexButton b0 = NexButton(0, 2, "b0");
NexTouch *nex_listen_list[] = 
{
    &b0,
    NULL
};

Preferences preferences; //FLASH MEMORY INICIALIZATION

void recieveData(WiFiClient client)
{
  uint8_t dataStr[2]; 
  client.readBytes(dataStr, 2);
  Serial.print("Received data: ");
  Serial.print(dataStr[0]);
  Serial.print("+");
  Serial.println(dataStr[1]);
}