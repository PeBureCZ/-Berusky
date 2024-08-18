struct Message
{
  unsigned int time;
  char group;
  int index;
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

bool recieveData(WiFiClient& client)
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
    Serial.print("Received data: ");
    Serial.print("Time: ");
    Serial.print(message.time);
    Serial.print(", group: ");
    Serial.print(message.group);
    Serial.print(", index: ");
    Serial.print(message.index);

    if (message.time == 0)
    {
      sendData(client);
      Serial.println("send sync data...");
      return true;
    }
    else
    {
      Serial.println("no sync data...");
      return false;
    }

  }
  else
  {
    Serial.println("Error: Not enough bytes read.");
    return false;
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
    message.time = actualTime / 1000; //in seconds
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