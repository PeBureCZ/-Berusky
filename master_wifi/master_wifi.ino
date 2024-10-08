//Esp server (deska s diodou)
#include <Nextion.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Preferences.h> //flash memory 
#include <MFRC522.h> //library responsible for communicating with the module mfrc522-RC522
#include <SPI.h> //library responsible for communicating of SPI bus

bool temporalBool = true;

const char* BUT_START = "Start";
const char* BUT_PLAYERS = "Hráči";
const char* BUT_MENU = "Menu";
const char* BUT_INTER = "Interval";
const char* BUT_ADD = "Přidat";
const char* BUT_REMOVE = "Odebrat";
const char* BUT_BACK = "Zpět";
const char* GP_Y = "Žlutá";
const char* GP_R = "Červená";
const char* GP_G = "Zelená";
const char* GP_B = "Modrá";
const char* BUT_PREVIOUS = "<<";
const char* BUT_NEXT = ">>";
const char* START_TEXT = "Berušky 1.0.0";
const char* MIN_ON_BUT = "minSvit";
const char* MAX_ON_BUT = "maxSvit";
const char* MIN_OFF_BUT = "minZhas";
const char* MAX_OFF_BUT = "maxZhas";
const char* PLUS_BUT = "+";
const char* MINUS_BUT = "-";
const char* BUT_STOP = "Stop";
const char* CLEAR_INFO = "Data vymazána";
const char* SCORE = "b.";
const char* PLAYER_ADD = "Přidáno";

const uint8_t B_COLOR = 14; //VALUE = PIN ON SLAVE, HAVE TO BE SAME DUE TO COMMUNICATION
const uint8_t Y_COLOR = 27; //VALUE = PIN ON SLAVE, HAVE TO BE SAME DUE TO COMMUNICATION
const uint8_t R_COLOR = 26; //VALUE = PIN ON SLAVE, HAVE TO BE SAME DUE TO COMMUNICATION
const uint8_t G_COLOR = 25; //VALUE = PIN ON SLAVE, HAVE TO BE SAME DUE TO COMMUNICATION

enum menuTypes
{
  START_MENU = 0,
  MENU1_B,
  MENU1_G,
  MENU1_R,
  MENU1_Y,
  MENU_SCORE,
  MENU3_MIN_ON,
  MENU3_MAX_ON,
  MENU3_MIN_OFF,
  MENU3_MAX_OFF,
  GAME_RUN,
  GAME_END
};

const uint8_t WIFI_DIODE = 13;
const uint8_t TX_PIN = 17; //display
const uint8_t RX_PIN = 16; //display

int actualMenu = START_MENU;
unsigned int score[] = 
{
  //sorted in order: WGRY
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
};

int indexOfGameScreen = 0;

unsigned int minLightOn = 5;
unsigned int maxLightOn = 20;
unsigned int minLightOff = 5;
unsigned int maxLightOff = 20;

WiFiServer server(24);
const char *ssid = "berusky";
const char *password = "neprolomitelne";
WiFiClient wifiClient;

//global values
byte lastChipUsed[4] = {0x00, 0x00, 0x00, 0x00};

//time
hw_timer_t *timer = nullptr;
unsigned long long actualTime = 0; //milliseconds
unsigned long long lastTime = 0; //milliseconds - used for game time calculation (last time = start time of the new game)
unsigned long long pressedTime = 0; //milliseconds
unsigned long long lastGameTime = 0 - 1; //milliseconds (maximum size)

//CHIP READER MFRC
const uint8_t SS_PIN = 21; //rfid reader
const uint8_t RST_PIN = 22; //rfid reader
MFRC522::MIFARE_Key key; //rfid reader
MFRC522::StatusCode wifiStatus; //rfid reader
MFRC522 mfrc522(SS_PIN, RST_PIN); //rfid reader

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

