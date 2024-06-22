void loop()
{
  digitalWrite(BUILD_IN_DIODE, LOW);
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
  nexLoop(nex_listen_list); //loop for Nextion display
  rfidCheck();
  delay(100);
}