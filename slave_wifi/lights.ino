void lightTick()
{
  if (lightOn)
  {
    if (lightTime == 0)
    {
      switchLightOff();
      return;
    }
  }
  else
  {
    if (messageHolder.getCount() == 255) return;
    else if (messageHolder.getCount() != -1)
    {
      //diode flickering if it wait for send message
      Serial.println("wait for send activated"); 
      if (actualTime % 1000 == 0) digitalWrite(SYNCHRONIZED, HIGH);
      else if (actualTime % 500 == 0) digitalWrite(SYNCHRONIZED, LOW);
    }
    if (lightTime == 0)
    {
      if (messageHolder.getCount() == -1) digitalWrite(SYNCHRONIZED, LOW);
      switchLightOn();
      return;
    }
  }
  lightTime--;
}

void switchLightOff()
{
  lightTime = getRandomNum(minLightOff, maxLightOff);  
  if (lightNum != -1 ) digitalWrite(lightNum, LOW);
  Serial.print("light off: ");
  Serial.println(lightNum);
  lightOn = false;
}

void switchLightOn()
{
  if (colorUsed[0] != 0 && colorUsed[1] != 0 && colorUsed[2] != 0 && colorUsed[3] != 0)  //lights need refresh
  {
    //REFRESH
    colorUsed[0] = 0;
    colorUsed[1] = 0;
    colorUsed[2] = 0;
    colorUsed[3] = 0;
  }
  int oldLightNum = lightNum;
  while(1)
  {
    int num = getRandomNum(1,100);
    if (num < 25) lightNum = B_COLOR;
    else if (num < 50) lightNum = Y_COLOR;
    else if (num < 75) lightNum = R_COLOR;
    else lightNum = G_COLOR;
    bool colorIsUsed = false;
    for (int i = 0; i < 4; i++)
    {
      if (colorUsed[i] == lightNum)
      {
        colorIsUsed = true;
        break;
      }
    }
    if (!colorIsUsed)
    {
      for (int i = 0; i < 4; i++)
      {
        if (colorUsed[i] == 0)
        {
          colorUsed[i] = lightNum;
          break;
        }
      }
      if (oldLightNum != -1 ) digitalWrite(oldLightNum, LOW);  
      Serial.print("turn on light: ");    
      Serial.println(lightNum);    
      digitalWrite(lightNum, HIGH);      
      break;
    }
    //a random light was used before, it requires a new random light
  }
  //generate new time for light ON
  lightTime = getRandomNum(minLightOn, maxLightOn);
  lightOn = true;
}

char getGroupByLight()
{
  if (lightNum == B_COLOR) return B_COLOR;
  if (lightNum == Y_COLOR) return Y_COLOR;
  if (lightNum == R_COLOR) return R_COLOR;
  if (lightNum == G_COLOR) return G_COLOR;
}

void useChipOnLight(char group, int indexOfChip)
{
  if (lightOn)
  {
    switchLightOff();
    Message message;
    message.time = static_cast<unsigned int>(actualTime/1000);
    message.group = group;
    message.index = indexOfChip;
    messageHolder.addMessage(message);
  }
  
}
