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

  Serial.println("slave start");
  delay(250);
}

void initializeAfterSync()
{
  //inter clock
  timer = timerBegin(100000);
  timerStart(timer);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0); //once per 10 milliseconds

  digitalWrite(SYNCHRONIZED, LOW);

  messageToSend.time = 0;
  messageToSend.group = 0;
  messageToSend.index = -1;
  
  initializeRandom(); //initialize random nums
  switchLightOn();
  lightTime = getRandomNum(minLightOn, maxLightOn);
}