void setup()
{
  //pinMode(BUILD_IN_DIODE, OUTPUT);
  //digitalWrite(BUILD_IN_DIODE, LOW);
  Serial.begin(115200);
  Serial.println("\n[*] Creating AP");
  WiFi.mode(WIFI_AP); //WIFI_AP
  WiFi.softAP(ssid, password);
  Serial.print("[+] AP Created with IP Gateway ");
  Serial.println(WiFi.softAPIP());
  server.begin();
  esp_wifi_set_max_tx_power(200); //max is 78, not equal 1:1 
}
