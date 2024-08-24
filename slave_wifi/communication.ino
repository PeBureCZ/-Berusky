
void sendData(struct Message& message) //char group, int indexOfChip
{
  if (client.connect(IP , 24))
  {
    unsigned int timeToSend = 0;
    if (message.group !=  0) timeToSend = static_cast<unsigned int>(actualTime/1000);
    message.time = timeToSend;
    client.write(reinterpret_cast<uint8_t*>(&message), sizeof(message));
    client.flush();

    //reset value to default
    message.time = 0;
    message.group = 0;
    message.index = -1;
    waitForSend = false;
    digitalWrite(SYNCHRONIZED, LOW);
  }
  else
  {
    Serial.print("no send\n");
    waitForSend = true;
  }
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
  }
  else
  {
    Serial.println("Error: Not enough bytes read\n");
  }
}