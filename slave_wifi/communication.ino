
bool sendSyncMessage()
{
  if (client.connect(IP , 24))
  {
    Message message; //create only deffault message (with index == -1, etc.) The master station knows that the slave station is not synchronized
    client.write(reinterpret_cast<uint8_t*>(&message), sizeof(message));
    client.flush();
    digitalWrite(SYNCHRONIZED, LOW);
    return true;
  }
  else return false;
}

void sendData() //char group, int indexOfChip
{
  if (client.connect(IP , 24))
  {
    Message message;
    message.time = messageHolder.getLast().time;
    message.group = messageHolder.getLast().group;
    message.index = messageHolder.getLast().index;
    client.write(reinterpret_cast<uint8_t*>(&message), sizeof(message));
    client.flush();
    messageHolder.removeLast();
    digitalWrite(SYNCHRONIZED, LOW);
  }
  else Serial.print("no send\n");
}

bool recieveData(WiFiClient& client)
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
    memcpy(groupArrayB, message.groupArrayB, sizeof(message.groupArrayB));
    memcpy(groupArrayG, message.groupArrayG, sizeof(message.groupArrayG));
    memcpy(groupArrayR, message.groupArrayR, sizeof(message.groupArrayR));
    memcpy(groupArrayY, message.groupArrayY, sizeof(message.groupArrayY));

    minLightOn = message.minON;
    maxLightOn = message.maxON;
    minLightOff = message.minOFF;
    maxLightOff = message.maxOFF;

    actualTime = message.time * 1000; //transfer to milliseconds

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
    Serial.println(message.maxOFF);

    initialized = true;
    return true;
  }
  else
  {
    Serial.println("Error: Not enough bytes read\n");
    return false;
  }
}