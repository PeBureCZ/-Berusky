
//Esp server (deska s diodou)

#include <WiFi.h>
#include <esp_wifi.h>

const char *ssid = "berusky";
const char *password = "neprolomitelne";

const int PIN_DIODE = 33; //číslo PINu (platí pro ESP32 dual W+BT)

WiFiServer server(24);

void setup()
{
  pinMode(PIN_DIODE, OUTPUT); //initializace pinu č. 33 jako "výstup"
  Serial.begin(115200);
  Serial.println("\n[*] Creating AP");
  WiFi.mode(WIFI_STA); //WIFI_AP
  WiFi.softAP(ssid, password);
  Serial.print("[+] AP Created with IP Gateway ");
  Serial.println(WiFi.softAPIP());
  server.begin();
  //WiFi.setTxPower(WIFI_POWER_19_5dBm); // WIFI_POWER_19_5dBm equal 78 / WIFI_POWER_2dBm = 1
  esp_wifi_set_max_tx_power(200); //max is 78, not equal 1:1

  Serial.print("dBm = ");
  Serial.print(WiFi.getTxPower());
  Serial.print(" / 4 \n");
  
}

void loop()
{
  WiFiClient client = server.available();
  if (client) 
  {
    // Wait for data from the client
    if (client.connected()) 
    {
      if (client.available()) 
      {
        recieveData(client);
      }
      //else Serial.print("no data\n");
    }
    else Serial.print("no client2");
  }
  delay(100);
}

void recieveData(WiFiClient client)
{
  uint8_t dataStr[2]; 
  client.readBytes(dataStr, 2);
  Serial.print("[+] Received data: ");
  Serial.print(dataStr[0]);
  Serial.print("+");
  Serial.print(dataStr[1]);
  useTestDiode(6); //DELAY!
}

void useTestDiode(int cycle)
{
  for (int i = 0; i < cycle; i++)
  {
    digitalWrite(PIN_DIODE, HIGH);
    delay(700);
    digitalWrite(PIN_DIODE, LOW);
    delay(300);
  }
}