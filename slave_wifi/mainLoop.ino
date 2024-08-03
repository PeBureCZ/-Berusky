void loop()
{
  delay(100);
  status = WiFi.status();
  if (status == WL_CONNECTED)
  {
    //sendData(4);
    rfidCheck();
  }
  else
  {
    // while(status != WL_CONNECTED)
    // {
    //   delay(1000);
    //   status = WiFi.status();
    //   digitalWrite(BUILD_IN_DIODE, HIGH);
    //   delay(1000);
    //   Serial.println(get_wifi_status(status));
    //   digitalWrite(BUILD_IN_DIODE, LOW); 
    // }
    Serial.println("unconected");
  }
  
  // if(client.connect(IP , 24)) 
  // {
  //   Serial.print("\nclient yes");   
  // }
  // else Serial.print("\nclient no");
}