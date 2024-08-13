void lightTick()
{
  if (lightOn) {
    if (lightTime == 0)
    {
      switchLightOff();
      return;
    }
  } else {
    if (lightTime == 0)
    {
      switchLightOn();
      return;
    }
  }
  lightTime--;
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
  if (colorUsed[0] == 0 || colorUsed[1] == 0 || colorUsed[2] == 0 || colorUsed[3] == 0)  //some of lights was not chosen yet
  {
    int oldLightNum = lightNum;
    while(1)
    {
      int num = getRandomNum(1,100);
      if (num < 25) lightNum = W_COLOR;
      else if (num < 50) lightNum = Y_COLOR;
      else if (num < 75) lightNum = R_COLOR;
      else lightNum = G_COLOR;
      bool colorIsUsed = false;
      for (int i = 0; i < 3; i++)
      {
        if (colorUsed[i] == lightNum)
        {
          colorIsUsed = true;
          break;
        }
      }
      if (!colorIsUsed)
      {
        colorUsed[0] = lightNum;
        digitalWrite(oldLightNum, LOW);
        digitalWrite(lightNum, HIGH); 
        break;
      }
      //a random light was used before, it requires a new random light
    }
  }
  //generate new time for light ON
  lightTime = getRandomNum(minLightOn, maxLightOn);
}

void switchLightOff()
{
}

void firstLightChoose()
{
  switchLightOn();
  lightOn = true;
}

// const uint8_t W_COLOR = 14;
// const uint8_t Y_COLOR = 27; //for GROUP LIGHT, GROUP Y
// const uint8_t R_COLOR = 26; //for GROUP LIGHT, GROUP R
// const uint8_t G_COLOR = 25; //for GROUP LIGHT, GROUP G
// const uint8_t DIODE_W = 14; //for GROUP LIGHT, GROUP W
// const uint8_t DIODE_Y = 27; //for GROUP LIGHT, GROUP Y
// const uint8_t DIODE_R = 26; //for GROUP LIGHT, GROUP R
// const uint8_t DIODE_G = 25; //for GROUP LIGHT, GROUP G
//colorUsed
//lightNum
//actualRandomNum
// unsigned int minLightOn = 5;
// unsigned int maxLightOn = 20;
// unsigned int minLightOff = 5;
// unsigned int maxLightOff = 20;