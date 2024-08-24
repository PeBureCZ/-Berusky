void loop()
{
  status = WiFi.status();

  if (!waitForSend) rfidCheck();
  else delay(50); 

  if (status == WL_CONNECTED)
  {
    if (waitForSend) sendData(messageToSend);
    if (client.connected())
    {
      if (client.available())
      {
        Serial.print("slave recieve...\n");  
        recieveData(client);
      }
    }
    if (!initialized) //only once
    {
      if (actualTime != 0)
      {
        initialized = true;
        initializeAfterSync();
      }
      else sendData(messageToSend); //for synchronization, "empty" values indicate first message to re-send sync data by master station
    }
  }  
}