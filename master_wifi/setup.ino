void setup()
{
  pinMode(BUILD_IN_DIODE, OUTPUT);
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(BUILD_IN_DIODE, LOW);
  Serial.begin(115200);

  //nextion display init
  nexSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); // Initialize hardware for Nextion display, pin 16(rx)+17(tx) on board
  nexInit();
  b0.attachPop(b0_Release, &b0);
  
  Serial.println("\n[*] Creating AP");
  WiFi.mode(WIFI_AP); //WIFI_AP
  WiFi.softAP(ssid, password);
  Serial.print("[+] AP Created with IP Gateway ");
  Serial.println(WiFi.softAPIP());
  server.begin();
  esp_wifi_set_max_tx_power(200); //max is 78, not equal 1:1 
}

void b0_Release(void *ptr)
{
  t0.setText("testx");
  Serial.println("test event");
}