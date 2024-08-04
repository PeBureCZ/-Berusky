void loop()
{
  wifiClient = server.available();
  if (wifiClient) 
  {
    // Wait for data from the client
    if (wifiClient.connected()) 
    {
      if (wifiClient.available()) 
      {
        recieveData(wifiClient);
        Serial.println("yes6\n");
      }
    }
    else Serial.println("yes4\n");
    sendData(wifiClient);
  }
  nexLoop(nex_listen_list); //loop for Nextion display
  rfidCheck();
  if (actualMenu == GAME_RUN && indexOfGameScreen == 0 && (actualTime % 100) < 11) printTime();
}