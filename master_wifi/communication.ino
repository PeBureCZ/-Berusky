struct Message
{
  unsigned int time;
  char group;
  int index;
};

struct SyncMessage
{
  unsigned int time;
  byte groupArrayB[32];
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
    Serial.print(getGroupText(message.group));
    Serial.print(", index: ");
    Serial.println(message.index);

    if (message.time == 0)
    {
      sendData(client);
      Serial.println("send sync data...\n¨¨");
      return true;
    }
    else
    {
      if (message.time <= lastGameTime/1000) // seconds <= miliseconds / 1000
      {
        //sorted in order: WGRY
        int IndexInScore = message.index;
        if (message.group == G_COLOR) IndexInScore += 8;
        else if (message.group == R_COLOR) IndexInScore += 16;
        else if (message.group == Y_COLOR) IndexInScore += 24;
        ++score[IndexInScore];
        Serial.print("add score +1 to ");
        Serial.print(message.group);
        Serial.print(" / index = ");
        Serial.print(message.index);
        if (GAME_RUN && indexOfGameScreen > 0) setGameScreen(indexOfGameScreen);
      }
      else Serial.println("game stop - no points");
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
    byte groupArrayB[32];
    byte groupArrayG[32];
    byte groupArrayR[32];
    byte groupArrayY[32];
    preferences.getBytes("groupArrayB", groupArrayB, 32);
    preferences.getBytes("groupArrayG", groupArrayG, 32);
    preferences.getBytes("groupArrayR", groupArrayR, 32);
    preferences.getBytes("groupArrayY", groupArrayY, 32);
    preferences.end(); // Ukončí práci s Preferences

    SyncMessage message;
    message.time = actualTime / 1000; //in seconds
    memcpy(message.groupArrayB, groupArrayB, sizeof(groupArrayB));
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

String getGroupText(char group)
{
  String groupText = "";
  switch(group)
  {
    case B_COLOR:
    {
      groupText = GP_B;
      break;
    }
    case Y_COLOR:
    {
      groupText = GP_Y;
      break;
    }
    case R_COLOR:
    {
      groupText = GP_R;
      break;
    }
    case G_COLOR:
    {
      groupText = GP_G;
      break;
    }
    default: groupText = "";
  }
  return groupText;
}
