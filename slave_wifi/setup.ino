void setup()
{
  Serial.begin(115200);

  pinMode(WIFI_CONNECT, OUTPUT);
  pinMode(SYNCHRONIZED, OUTPUT);
  digitalWrite(WIFI_CONNECT, LOW);
  digitalWrite(SYNCHRONIZED, LOW);

  SPI.begin(); // Init SPI bus
    // Init MFRC522
  mfrc522.PCD_Init(); 
  Serial.println("\nConnecting");
  Serial.println(get_wifi_status(status));
  WiFi.mode(WIFI_STA); //WIFI_STATION
  WiFi.begin(ssid, password);

  esp_wifi_set_max_tx_power(200);

  Serial.println("slave start");
  delay(250);
  while(true)
  {
    status = WiFi.status();
    if (status == WL_CONNECTED)
    {
      sendData(4,1);
      break;
    }
  }
  digitalWrite(WIFI_CONNECT, HIGH);
  digitalWrite(SYNCHRONIZED, HIGH);
}