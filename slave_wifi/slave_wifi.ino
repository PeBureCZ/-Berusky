//ESP32 - CHYTACKA (SLAVE STATION)
#include <WiFi.h>
#include <MFRC522.h> //library responsible for communicating with the module mfrc522-RC522
#include <SPI.h> //library responsible for communicating of SPI bus
#include <esp_wifi.h> //allow esp_wifi_set_max_tx_power(20);
#include <Preferences.h> //flash memory 

Preferences preferences; //FLASH MEMORY INICIALIZATION

const uint8_t B_COLOR = 14;
const uint8_t Y_COLOR = 27;
const uint8_t R_COLOR = 26;
const uint8_t G_COLOR = 25;
const uint8_t DIODE_B = B_COLOR; //for GROUP LIGHT, GROUP W
const uint8_t DIODE_Y = Y_COLOR; //for GROUP LIGHT, GROUP Y
const uint8_t DIODE_R = R_COLOR; //for GROUP LIGHT, GROUP R
const uint8_t DIODE_G = G_COLOR; //for GROUP LIGHT, GROUP G

struct Message
{
  unsigned int time = 0;
  char group = 0;
  int index = -1;
  Message(){}
  Message(const Message& other) : time(other.time), group(other.group), index(other.index) {}
  Message& operator=(const Message& other)
  {
    if (this != &other)
    {
      time = other.time;
      group = other.group;
      index = other.index;
    }
    return *this;
  }
};

struct SyncMessage
{
  unsigned int time;
  byte groupArrayB[32];
  byte groupArrayG[32];
  byte groupArrayR[32];
  byte groupArrayY[32];
  byte minON;
  byte maxON;
  byte minOFF;
  byte maxOFF;
};

const unsigned char MEM_BLOCK_SIZE = 32; //size of the memory block that hold messages and will be saved onto flash memory

class MessageHolder
{
  private:
  int lastIndex = -1;
  Message messages[MEM_BLOCK_SIZE];

  public:
  MessageHolder(){}
  int getCount()
  {
    return lastIndex;
  }
  void addMessage(Message& message)
  {
    //if the block index is 31, then messages need to be saved in flash memory
    int blockIndex = lastIndex % MEM_BLOCK_SIZE;
    if (blockIndex >= MEM_BLOCK_SIZE-1)
    {
      int blocknNum = lastIndex / MEM_BLOCK_SIZE; //determine what block is used for save data to the flash memory
      String adress = "memoryBlock" + String(blocknNum*MEM_BLOCK_SIZE); //could be 0 
      Serial.print("created adress-out: ");
      Serial.println(adress);

      preferences.begin("my-app", false);
      preferences.putBytes(adress.c_str(), messages, sizeof(messages));
      preferences.end();
      messages[0] = message; //start from start index (rewriting)
    }
    else messages[++blockIndex] = message;   
    ++lastIndex;
  }
  bool removeLast()
  {
    if (lastIndex != -1)
    {
      --lastIndex; 
      int blockIndex = lastIndex % MEM_BLOCK_SIZE; 
      if (blockIndex == MEM_BLOCK_SIZE-1) 
      {
        //need to load new messages from flash memory
        int blocknNum = lastIndex / MEM_BLOCK_SIZE; //determine what block is used for save data to the flash memory 
        String adress = "memoryBlock" + String(blocknNum*MEM_BLOCK_SIZE); //could be 0       
        preferences.begin("my-app", true); //true for read only
        preferences.getBytes(adress.c_str(), messages, sizeof(messages));
        preferences.end();

      }
      return true;
    }
    else return false;
  }
  bool canAdd()
  {
    if (lastIndex == MEM_BLOCK_SIZE*8-1) return false;
    else return true;
  }
  Message getLast()
  {
    
    if (lastIndex != -1)
    {
      int blockIndex = lastIndex % MEM_BLOCK_SIZE;
      return messages[blockIndex];
    }
    else
    {
      Message message;
      return message;
    }
  }
};


//wifi
const char* ssid     = "berusky";
const char* password = "neprolomitelne";
IPAddress IP (192, 168, 4, 1);  //adress to connect
WiFiClient client;
int status = WL_IDLE_STATUS;

const uint8_t SYNCHRONIZED = 32; //for special control diode

const uint8_t SS_PIN = 21; //rfid reader
const uint8_t RST_PIN = 22; //rfid reader
MFRC522::MIFARE_Key key; //rfid reader
MFRC522::StatusCode wifiStatus; //rfid reader
MFRC522 mfrc522(SS_PIN, RST_PIN); //rfid reader

unsigned long long actualTime = 0; //milliseconds
unsigned long long lightTime = 0; //seconds

bool initialized = false;

#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16

//inter clock
hw_timer_t *timer = nullptr;
void IRAM_ATTR onTimer()
{
  actualTime += 10; //miliseconds
  if ((actualTime % 1000) == 0) lightTick();
}

//create a holder class for carrying messages if communication between the slave and master stations failes
MessageHolder messageHolder;

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
bool lightOn = false;
int lightNum = -1;
int colorUsed[] = {0,0,0,0};

byte groupArrayB[32] = 
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

int randomSeedNum = -1;
int pseudoRandomNums[] =
{
  72, 28, 46, 5, 94, 81, 14, 54, 32, 97,
  18, 66, 36, 24, 74, 84, 44, 59, 88, 62,
  10, 13, 19, 76, 1, 85, 48, 91, 43, 37,
  55, 34, 95, 21, 60, 27, 99, 40, 23, 92,
  7, 82, 68, 53, 41, 2, 33, 26, 12, 100,
  11, 73, 8, 57, 98, 78, 22, 45, 63, 52,
  47, 87, 4, 86, 77, 3, 31, 56, 71, 93,
  49, 30, 58, 61, 15, 42, 67, 29, 35, 17,
  79, 38, 20, 65, 96, 39, 6, 70, 50, 25,
  9, 89, 51, 90, 16, 64, 75, 80, 83, 69
};

