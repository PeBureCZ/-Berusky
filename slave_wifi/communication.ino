struct syncData
{
  unsigned int time;
  char player;
  char index;
};

void sendMessage()
{

}

void sendData(uint8_t firstData, uint8_t secondData)
{
  if (client.connect(IP , 24))
  {
    uint8_t dataStr[2];
    dataStr[0] = firstData;
    dataStr[1] = secondData;
    Serial.print("Sending data: ");
    Serial.print(dataStr[0]);
    client.write(dataStr,sizeof(dataStr));
    Serial.print(" + ");
    Serial.print(dataStr[1]);
    client.flush();
    client.stop();
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(WIFI_CONNECT, LOW);
      delay(500);
      digitalWrite(WIFI_CONNECT, HIGH);
      delay(500);
    }
    
    Serial.print("end send");
  }
  else Serial.print("no send");
}