//Esp server (deska s diodou)
#include <Nextion.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Preferences.h> //flash memory 
#include <MFRC522.h> //library responsible for communicating with the module mfrc522-RC522
#include <SPI.h> //library responsible for communicating of SPI bus

enum menuTypes
{
  START_MENU = 0,
  MENU1_W,
  MENU1_G,
  MENU1_R,
  MENU1_Y,
  MENU2,
  MENU3,
  GAME_RUN,
  GAME_END

};

const uint8_t BUILD_IN_DIODE = 2;
const uint8_t TX_PIN = 17;
const uint8_t RX_PIN = 16;
int actualMenu = START_MENU;

WiFiServer server(24);
const char *ssid = "berusky";
const char *password = "neprolomitelne";

//global values
byte lastChipUsed[4] = {0x00, 0x00, 0x00, 0x00};
unsigned long long actualTime = 0; //milliseconds
unsigned long long lastTime = 0; //milliseconds

//time
hw_timer_t *timer = nullptr;

//CHIP READER MFRC

const uint8_t SS_PIN = 21;
const uint8_t RST_PIN = 22;
MFRC522::MIFARE_Key key;
MFRC522::StatusCode wifiStatus;
MFRC522 mfrc522(SS_PIN, RST_PIN);

//Nextion (display)
NexButton b0 = NexButton(0, 2, "b0"); //page id = 0, component id = 2, obj name = b0
NexButton b1 = NexButton(0, 3, "b1");  //menu
NexButton b2 = NexButton(0, 4, "b2");  //remove
NexButton b3 = NexButton(0, 5, "b3"); //back
NexText t0 = NexText(0, 1, "t0");

NexTouch *nex_listen_list[] = 
{
    &b0,
    &b1,
    &b2,
    &b3,
    //another buttons, event, etc..?
    NULL
};

void IRAM_ATTR onTimer()
{
  actualTime += 10; //miliseconds
}

Preferences preferences; //FLASH MEMORY INICIALIZATION

