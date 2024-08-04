void loop()
{
  delay(50);
  //status = WiFi.status();
  // if (status == WL_CONNECTED)
  // {
  //   //sendData(4);
  //   Serial.println("yes\n");
  //   rfidCheck();
  // }
  // else
  // {
  //   // while(status != WL_CONNECTED)
  //   // {
  //   //   delay(1000);
  //   //   status = WiFi.status();
  //   //   digitalWrite(BUILD_IN_DIODE, HIGH);
  //   //   delay(1000);
  //   //   Serial.println(get_wifi_status(status));
  //   //   digitalWrite(BUILD_IN_DIODE, LOW); 
  //   // }
  //   //Serial.println("unconected");
  // }
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
}