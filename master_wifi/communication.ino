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

void recieveData(WiFiClient& client)
{
  Serial.print("\nNew client connected from ");
  Serial.print(client.remoteIP());
  Serial.print(":");
  Serial.println(client.remotePort());
  Message message;

  // Read the bytes into the message struct
  size_t bytesRead = client.readBytes(reinterpret_cast<byte*>(&message), sizeof(message));
  
  if (bytesRead == sizeof(message))
  {
    uint8_t thisSlaveID = message.slaveID;
    Serial.print("Received data: ");
    Serial.print("Time: ");
    Serial.print(message.time);
    Serial.print(", Byte1: ");
    Serial.print(message.byte1);
    Serial.print(", Byte2: ");
    Serial.print(message.byte2);
    Serial.print(", Byte3: ");
    Serial.print(message.byte3);
    Serial.print(", Byte4: ");
    Serial.print(message.byte4);
    Serial.print(", stationID: ");
    Serial.println(thisSlaveID);
    // if (message.time == 0) // if time is noct sync -> send sync data
    // {
    //   slavesID[thisSlaveID] = thisSlaveID; //set available stations
    //   //slavesIP[thisSlaveID] = client.remoteIP();
    //   Message message;
    //   message.time = actualTime;
    //   message.byte1 = 0x05;
    //   message.byte2 = 0x06;
    //   message.byte3 = 0x07; 
    //   message.byte4 = 0x08;
    //   message.slaveID = 255;

    //   Serial.print("master sending data...\n");
     
    //   Serial.print("master end send");
    // }
  }
  else
  {
    Serial.println("Error: Not enough bytes read.");
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void sendData(WiFiClient client)
{
  if (client)
  {
    preferences.begin("my-app", false);
    byte groupArrayW[32];
    byte groupArrayG[32];
    byte groupArrayR[32];
    byte groupArrayY[32];
    preferences.getBytes("groupArrayW", groupArrayW, 32);
    preferences.getBytes("groupArrayG", groupArrayG, 32);
    preferences.getBytes("groupArrayR", groupArrayR, 32);
    preferences.getBytes("groupArrayY", groupArrayY, 32);
    preferences.end(); // Ukončí práci s Preferences

    SyncMessage message;
    message.time = actualTime;
    memcpy(message.groupArrayW, groupArrayW, sizeof(groupArrayW));
    memcpy(message.groupArrayG, groupArrayG, sizeof(groupArrayG));
    memcpy(message.groupArrayR, groupArrayR, sizeof(groupArrayR));
    memcpy(message.groupArrayY, groupArrayY, sizeof(groupArrayY));
    message.minON = minLightOn;
    message.maxON = maxLightOn;
    message.minOFF = minLightOff;
    message.maxOFF = maxLightOff;

    Serial.print("Sending data...\n");

    client.write(reinterpret_cast<uint8_t*>(&message), sizeof(message));
    client.flush();
    
    Serial.print("end send\n");
  }
  else Serial.print("no send\n");
}