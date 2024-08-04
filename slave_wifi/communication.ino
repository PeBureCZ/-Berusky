struct Message
{
  unsigned int time;
  byte byte1;
  byte byte2;
  byte byte3;
  byte byte4;
  uint8_t slaveID;
};

struct SyncMessage
{
  unsigned int time;
  byte groupArrayW[32];
  byte groupArrayG[32];
  byte groupArrayR[32];
  byte groupArrayY[32];
  byte minON;
  byte maxON;
  byte minOFF;
  byte maxOFF;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void sendData(unsigned int currentTime, byte byte1, byte byte2, byte byte3, byte byte4, uint8_t stationID)
{
  if (client.connect(IP , 24))
  {
    Message message;
    message.time = currentTime;
    message.byte1 = byte1;
    message.byte2 = byte2;
    message.byte3 = byte3; 
    message.byte4 = byte4;
    message.slaveID = stationID;

    Serial.print("Sending data...");

    client.write(reinterpret_cast<uint8_t*>(&message), sizeof(message));

    client.flush();
    // client.stop();
    
    // for(int i = 0; i < 4; i++)
    // {
    //   digitalWrite(WIFI_CONNECT, LOW);
    //   delay(500);
    //   digitalWrite(WIFI_CONNECT, HIGH);
    //   delay(500);
    // }
    
    Serial.print("end send\n");
  }
  else Serial.print("no send\n");
}

void recieveData(WiFiClient& client)
{
  Serial.print("\nNew Mclient connected from ");
  Serial.print(client.remoteIP());
  Serial.print(":");
  Serial.println(client.remotePort());
  SyncMessage message;
  // Read the bytes into the message struct
  size_t bytesRead = client.readBytes(reinterpret_cast<byte*>(&message), sizeof(message));
  
  if (bytesRead == sizeof(message))
  {
    memcpy(groupArrayW, message.groupArrayW, sizeof(message.groupArrayW));
    memcpy(groupArrayG, message.groupArrayG, sizeof(message.groupArrayG));
    memcpy(groupArrayR, message.groupArrayR, sizeof(message.groupArrayR));
    memcpy(groupArrayY, message.groupArrayY, sizeof(message.groupArrayY));

    minLightOn = message.minON;
    maxLightOn = message.maxON;
    minLightOff = message.minOFF;
    maxLightOff = message.maxOFF;

    Serial.print("Received data: ");
    Serial.print("Time: ");
    Serial.print(message.time);
    Serial.print(", minON: ");
    Serial.print(message.minON);
    Serial.print(", maxON: ");
    Serial.print(message.maxON);
    Serial.print(", minOFF: ");
    Serial.print(message.minOFF);
    Serial.print(", maxOFF: ");
    Serial.print(message.maxOFF);
  }
  else
  {
    Serial.println("Error: Not enough bytes read\n");
  }
}