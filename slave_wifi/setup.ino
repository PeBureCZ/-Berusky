void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(1);; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(WIFI_CONNECT, OUTPUT);
  pinMode(SYNCHRONIZED, OUTPUT);
  digitalWrite(WIFI_CONNECT, LOW);
  digitalWrite(SYNCHRONIZED, LOW);

  SPI.begin(); // Init SPI bus
    // Init MFRC522
  mfrc522.PCD_Init(); 
  Serial.println("Connecting...\n");
  Serial.println(get_wifi_status(status));

  WiFi.mode(WIFI_STA); //WIFI_STATION
  WiFi.begin(ssid, password);

  Serial.println("connected to the server\n");

  esp_wifi_set_max_tx_power(200);

  Serial.println("slave start");
  delay(250);
  while(true)
  {
    status = WiFi.status();
    if (status == WL_CONNECTED)
    {
      unsigned int timeToSend = static_cast<unsigned int>(actualTime/1000);
      sendData(timeToSend, 0x01, 0x02, 0x03, 0x04, slaveID);
      break;
    }
  }
  digitalWrite(WIFI_CONNECT, HIGH);
  digitalWrite(SYNCHRONIZED, HIGH);
}