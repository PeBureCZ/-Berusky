void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(1); // wait for serial port to connect. Needed for native USB port only
  }

  //controler diodes
  pinMode(SYNCHRONIZED, OUTPUT);
  digitalWrite(SYNCHRONIZED, HIGH);

  //game group lights
  pinMode(DIODE_B, OUTPUT);
  pinMode(DIODE_Y, OUTPUT);
  pinMode(DIODE_R, OUTPUT);
  pinMode(DIODE_G, OUTPUT);
  digitalWrite(DIODE_B, LOW);
  digitalWrite(DIODE_Y, LOW);
  digitalWrite(DIODE_R, LOW);
  digitalWrite(DIODE_G, LOW);

  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  Serial.println("Connecting...\n");
  Serial.println(get_wifi_status(status));

  WiFi.mode(WIFI_STA); //WIFI_STATION
  WiFi.begin(ssid, password);

  esp_wifi_set_max_tx_power(200);

  //memory management
  preferences.begin("my-app", false);
  preferences.clear(); //can hold a previous game
  Message memoryBlock[MEM_BLOCK_SIZE];
  String basic = "memoryBlock";
  String adress = basic + String(0);
  preferences.putBytes(adress.c_str(), memoryBlock, sizeof(memoryBlock)); //0
  adress = basic + String(MEM_BLOCK_SIZE);
  preferences.putBytes(adress.c_str(), memoryBlock, sizeof(memoryBlock)); //1
  adress = basic + String(MEM_BLOCK_SIZE*2);
  preferences.putBytes(adress.c_str(), memoryBlock, sizeof(memoryBlock)); //2
  adress = basic + String(MEM_BLOCK_SIZE*3);
  preferences.putBytes(adress.c_str(), memoryBlock, sizeof(memoryBlock)); //3
  adress = basic + String(MEM_BLOCK_SIZE*4);
  preferences.putBytes(adress.c_str(), memoryBlock, sizeof(memoryBlock)); //4
  adress = basic + String(MEM_BLOCK_SIZE*5);
  preferences.putBytes(adress.c_str(), memoryBlock, sizeof(memoryBlock)); //5
  adress = basic + String(MEM_BLOCK_SIZE*6);
  preferences.putBytes(adress.c_str(), memoryBlock, sizeof(memoryBlock)); //6
  adress = basic + String(MEM_BLOCK_SIZE*7);
  preferences.putBytes(adress.c_str(), memoryBlock, sizeof(memoryBlock)); //7
  adress = basic + String(MEM_BLOCK_SIZE*8);
  preferences.putBytes(adress.c_str(), memoryBlock, sizeof(memoryBlock)); //8
  preferences.end(); // Ukončí práci s Preferences

  delay(250);
  Serial.println("slave start");
}

void initializeAfterSync()
{
  //inter clock
  timer = timerBegin(100000);
  timerStart(timer);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0); //once per 10 milliseconds

  digitalWrite(SYNCHRONIZED, LOW);
  
  initializeRandom(); //initialize random nums
  switchLightOn();
  lightTime = getRandomNum(minLightOn, maxLightOn);
}