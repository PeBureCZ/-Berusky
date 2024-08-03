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
    }
  }
  nexLoop(nex_listen_list); //loop for Nextion display
  rfidCheck();
  if (actualMenu == GAME_RUN && indexOfGameScreen == 0 && (actualTime % 100) < 11) printTime();
}