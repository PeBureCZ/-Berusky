void setup()
{
  Serial.begin(115200);
  pinMode(WIFI_DIODE, OUTPUT);
  digitalWrite(WIFI_DIODE, HIGH);

  timer = timerBegin(100000);
  timerStart(timer);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0); //once per 10 milliseconds
  
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);

  //nextion display init
  nexSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); // Initialize hardware for Nextion display, pin 16(rx)+17(tx) on board
  nexInit();
  b0.attachPop(b0_but, &b0);
  b1.attachPush(b1_press, &b1); //press
  b1.attachPop(b1_but, &b1); //release
  b2.attachPop(b2_but, &b2);
  b3.attachPop(b3_but, &b3);

  //init spi + mfrc522
  SPI.begin();
  mfrc522.PCD_Init();  
  
  Serial.println("\n[*] Creating AP");
  WiFi.mode(WIFI_AP); //WIFI_AP
  WiFi.softAP(ssid, password);
  Serial.print("[+] AP Created with IP Gateway ");
  Serial.println(WiFi.softAPIP());
  server.begin();
  esp_wifi_set_max_tx_power(200); //max is 78, not equal 1:1 
  
  firstTimeMemoryCheck();
  digitalWrite(WIFI_DIODE, LOW);
}



