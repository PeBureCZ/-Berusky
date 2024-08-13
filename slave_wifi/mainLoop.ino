void loop()
{
  if (status == WL_CONNECTED)
  {
    if (client.connected())
    {
      if (client.available())
      {
        Serial.print("slave recieve...\n");  
        recieveData(client);
      }
    }
  }
  rfidCheck();
}