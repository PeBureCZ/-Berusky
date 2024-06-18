void loop()
{
  WiFiClient client = server.available();
  if (client) 
  {
    // Wait for data from the client
    if (client.connected()) 
    {
      if (client.available()) 
      {
        recieveData(client);
      }
      //else Serial.print("no data\n");
    }
  }
  nexLoop(nex_listen_list);
  //b0_Release(nullptr);
  delay(50);
}