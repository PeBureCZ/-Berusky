void loop()
{
  status = WiFi.status();

  if (messageHolder.getCount() != 255) rfidCheck();
  else delay(50); 

  if (status == WL_CONNECTED)
  {
    if (messageHolder.getCount() != -1) sendData();
    if (client.connected())
    {
      if (client.available())
      {
        Serial.print("slave recieve...\n");  
        if (recieveData(client)) initializeAfterSync();;
      }
    }
    if (!initialized) //only once
    {
      //for synchronization to re-send sync data by master station
      sendSyncMessage();       
    }
  }  
}