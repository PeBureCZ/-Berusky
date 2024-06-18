//Esp server (deska s diodou)
#include <Nextion.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Preferences.h> //flash memory 

const uint8_t BUILD_IN_DIODE = 2;
const uint8_t TX_PIN = 17;
const uint8_t RX_PIN = 16;

WiFiServer server(24);
const char *ssid = "berusky";
const char *password = "neprolomitelne";

//HardwareSerial nextionSerial(2);

char buffer[100] = {0};

NexButton b0 = NexButton(0, 2, "b0"); //page id = 0, component id = 2, obj name = b0
NexText t0 = NexText(0, 1, "t0");

NexTouch *nex_listen_list[] = 
{
    &b0,
    //another buttons, event, etc..?
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