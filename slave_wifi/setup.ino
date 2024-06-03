void setup()
{
    Serial.begin(115200);
    pinMode(SOUND_ITEM, OUTPUT);
    digitalWrite(SOUND_ITEM, HIGH);
    pinMode(BUILD_IN_DIODE, OUTPUT);
    digitalWrite(BUILD_IN_DIODE, HIGH);
    SPI.begin(); // Init SPI bus
     // Init MFRC522
    mfrc522.PCD_Init(); 
    Serial.println("\nConnecting");
    Serial.println(get_wifi_status(status));
    WiFi.begin(ssid, password);

    esp_wifi_set_max_tx_power(200);
    Serial.println("slave start");
}