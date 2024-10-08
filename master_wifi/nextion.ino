void b0_but(void *ptr) //left top RELEASE
{
  switch (actualMenu)
  {
    case START_MENU:
    {
      lastTime = actualTime;
      sendCommand("vis b1,0"); //hidde button
      b3.setText(BUT_STOP);
      b0.setText(BUT_PREVIOUS);
      b2.setText(BUT_NEXT);
      indexOfGameScreen = 0;
      lastGameTime = 0 - 1; //maximum size of variable value due to accept points from catch stations
      resetScore();
      actualMenu = GAME_RUN;
      break;
    }
    case GAME_RUN:
    {
      --indexOfGameScreen;
      if (indexOfGameScreen < 0) indexOfGameScreen = 0; //to not rewrite timer clock
      setGameScreen(indexOfGameScreen);
      break;
    }
    case MENU1_B:
    {
      if (lastChipUsed[0] != 0x00) addNewChip(lastChipUsed[0],lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], B_COLOR);  
      Serial.println("add to b");
      break;
    }
    case MENU1_G:
    {
      if (lastChipUsed[0] != 0x00) addNewChip(lastChipUsed[0],lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], G_COLOR);  
      Serial.println("add to g");
      break;
    }

    case MENU1_R:
    {
      if (lastChipUsed[0] != 0x00) addNewChip(lastChipUsed[0],lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], R_COLOR);  
      Serial.println("add to r");
      break;
    }
    case MENU1_Y:
    {
      if (lastChipUsed[0] != 0x00) addNewChip(lastChipUsed[0],lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], Y_COLOR);  
      Serial.println("add to y");
      break;
    }
    case MENU3_MIN_ON: //click on "+" time
    {
      if (minLightOn + 1 >= maxLightOn) break;
      minLightOn++;
      String newMenuText = MIN_ON_BUT;
      newMenuText += " = ";
      newMenuText += minLightOn;
      setMenuText(newMenuText);
      break;
    }
    case MENU3_MAX_ON: //click on "+" time
    {
      if (maxLightOff == 10000) break;
      maxLightOn++;
      String newMenuText = MAX_ON_BUT;
      newMenuText += " = ";
      newMenuText += maxLightOn;
      setMenuText(newMenuText);  
      break;   
    }
    case MENU3_MIN_OFF: //click on "+" time
    {
      if (minLightOff + 1 >= maxLightOff) break;
      minLightOff++; 
      String newMenuText = MIN_OFF_BUT;
      newMenuText += " = ";
      newMenuText += minLightOff;
      setMenuText(newMenuText);
      break;  
    }
    case MENU3_MAX_OFF: //click on "+" time
    {
      if (maxLightOff == 10000) break;
      maxLightOff++;   
      String newMenuText = MAX_OFF_BUT;
      newMenuText += " = ";
      newMenuText += maxLightOff;
      setMenuText(newMenuText);
      break;
    }
    case MENU_SCORE: //click on "MENU" on basic screen
    {
      if (indexOfGameScreen == 1) break;
      setGameScreen(--indexOfGameScreen);
      break;
    }
    default: break;
  }
}

void b1_but(void *ptr) //left bottom RELEASE
{
  runCommand(ADMIN_PRINT);
  switch (actualMenu)
  {
    case START_MENU:
    {
      sendCommand("vis b1,0"); //hidde button
      b3.setText(BUT_BACK);
      b0.setText(BUT_PREVIOUS);
      b2.setText(BUT_NEXT);
      indexOfGameScreen = 1;
      actualMenu = MENU_SCORE;
      setGameScreen(1);
      break;
    }
    case MENU1_B:
    case MENU1_G:
    case MENU1_R:
    case MENU1_Y:
    {
      if ((MENU1_B || MENU1_G || MENU1_R || MENU1_Y) && actualTime - pressedTime > 3000)
      {
        Serial.println("holded B1 click");
        clearAllData();
        String newMenuText = CLEAR_INFO;
        setMenuText(newMenuText);
      }
      else
      {
        Serial.println("normal B1 click");
        if (lastChipUsed[0] != 0x00) removeChip(lastChipUsed[0], lastChipUsed[1], lastChipUsed[2], lastChipUsed[3]);
        else Serial.println("nothing to delete");
        Serial.println("chip removed");
      }
      break;
    }
    case MENU3_MIN_ON: //click on "-" time
    {
      if (minLightOn == 0) break;
      minLightOn--;
      String newMenuText = MIN_ON_BUT;
      newMenuText += " = ";
      newMenuText += minLightOn;
      setMenuText(newMenuText);
      break;
    }
    case MENU3_MAX_ON: //click on "-" time
    {
      if (maxLightOn - 1 <= minLightOn) break;
      maxLightOn--;  
      String newMenuText = MAX_ON_BUT;
      newMenuText += " = ";
      newMenuText += maxLightOn;
      setMenuText(newMenuText);
      break;    
    }
    case MENU3_MIN_OFF: //click on "-" time
    {
      if (minLightOff == 0) break;
      minLightOff--;  
      String newMenuText = MIN_OFF_BUT;
      newMenuText += " = ";
      newMenuText += minLightOff;
      setMenuText(newMenuText);
      break;    
    }
    case MENU3_MAX_OFF: //click on "-" time
    {
      if (maxLightOff - 1 <= minLightOff ) break;
      maxLightOff--;   
      String newMenuText = MAX_OFF_BUT;
      newMenuText += " = ";
      newMenuText += maxLightOff;
      setMenuText(newMenuText);
      break; 
    }
    default: {Serial.println("nothing");}
  }
}

void b1_press(void *ptr) //left bottom PRESS ONLY
{
  pressedTime = actualTime; //start to check button hold time in Menu1
}

void b2_but(void *ptr) //right top RELEASE
{
  // Serial.println("b2");
  switch (actualMenu)
  {
    case START_MENU:
    {
      b0.setText(BUT_ADD);
      b1.setText(BUT_REMOVE);
      b2.setText(GP_B);
      b3.setText(BUT_BACK);
      actualMenu = MENU1_B;
      break;
    }
    case GAME_RUN:
    {
      if (indexOfGameScreen == 32) break;
      setGameScreen(++indexOfGameScreen);
      break;
    }
    case MENU1_B:
    {
      b2.setText(GP_G);
      actualMenu = MENU1_G;
      break;
    }
    case MENU1_G:
    {
      b2.setText(GP_R);
      actualMenu = MENU1_R;
      break;
    }
    case MENU1_R:
    {
      b2.setText(GP_Y);
      actualMenu = MENU1_Y;
      break;
    }
    case MENU1_Y:
    {
      b2.setText(GP_B);
      actualMenu = MENU1_B;
      break;
    }
    case MENU3_MIN_ON:
    {
      b2.setText(MAX_ON_BUT);
      String newMenuText = MAX_ON_BUT;
      newMenuText += " = ";
      newMenuText += maxLightOn;
      setMenuText(newMenuText);
      actualMenu = MENU3_MAX_ON;
      break;     
    }
    case MENU3_MAX_ON:
    {
      b2.setText(MIN_OFF_BUT);
      String newMenuText = MIN_OFF_BUT;
      newMenuText += " = ";
      newMenuText += minLightOff;
      setMenuText(newMenuText);
      actualMenu = MENU3_MIN_OFF;
      break;     
    }
    case MENU3_MIN_OFF:
    {
      b2.setText(MAX_OFF_BUT);
      String newMenuText = MAX_OFF_BUT;
      newMenuText += " = ";
      newMenuText += maxLightOff;
      setMenuText(newMenuText);
      actualMenu = MENU3_MAX_OFF;
      break;     
    }
    case MENU3_MAX_OFF:
    {
      b2.setText(MIN_ON_BUT);
      String newMenuText = MIN_ON_BUT;
      newMenuText += " = ";
      newMenuText += minLightOn;
      setMenuText(newMenuText);
      actualMenu = MENU3_MIN_ON;
      break;     
    }
    case MENU_SCORE: //click on "MENU" on basic screen
    {
      if (indexOfGameScreen == 32) break;
      setGameScreen(++indexOfGameScreen);;
    }
    default: break;
  }
}

void b3_but(void *ptr)  //right bottom RELEASE
{
  switch (actualMenu)
  {
    case START_MENU:
    {
      b0.setText(PLUS_BUT);
      b1.setText(MINUS_BUT);
      b2.setText(MIN_ON_BUT);
      String newMenuText = MIN_ON_BUT;
      newMenuText += " = ";
      newMenuText += minLightOn;
      setMenuText(newMenuText);
      b3.setText(BUT_BACK);
      actualMenu = MENU3_MIN_ON;
      break;
    }
    case GAME_RUN:
    {
      b3.setText(BUT_BACK);
      actualMenu = GAME_END;
      lastGameTime = actualTime; //set the time of game end to compare times from catch stations
      break;
    }
    case GAME_END:
    {
      sendCommand("vis b1,1"); //hidde button
      b0.setText(BUT_START);
      b2.setText(BUT_PLAYERS);
      b3.setText(BUT_INTER);
      actualMenu = START_MENU;
      String newMenuText = "";
      setMenuText(newMenuText);   
      break;
    }
    default: //DEFF = "Back"
    {
      b0.setText(BUT_START);
      b1.setText(BUT_MENU);
      b2.setText(BUT_PLAYERS);
      b3.setText(BUT_INTER);
      actualMenu = START_MENU;
      sendCommand("vis b1,1"); //unhidde button
      String newMenuText = START_TEXT;
      setMenuText(newMenuText);
      saveData();
      break;     
    }
  }
}

void setMenuText(String &text)
{
  t0.setText(text.c_str());
}

void setMenuToChip()
{
  String output = "";
  output += String(lastChipUsed[0], HEX);
  output += ",";
  output += String(lastChipUsed[1], HEX);
  output += ",";
  output += String(lastChipUsed[2], HEX);
  output += ",";
  output += String(lastChipUsed[3], HEX);
  output += "\n";
  
  if (lastChipUsed[0] == 0x00) output = PLAYER_ADD;
  else
  {
    for (int i = 0; i < 4; i++)
    {
      char group = 0;
      if (i == 0) group = B_COLOR; 
      else if (i == 1) group = R_COLOR;
      else if (i == 2) group = Y_COLOR;
      else if (i == 3) group = G_COLOR;

      int indexOfChip = isKnowChip(lastChipUsed[0], lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], group);
      if (indexOfChip != -1)
      {
        output += "(";
        output += getGroupText(group);
        output += indexOfChip+1; //transfer from index of chip to num of chip
        output += ")";
        break;
      }
    }
  } 
  setMenuText(output);
}

void setGameScreen(int newMenu)
{
  indexOfGameScreen = newMenu;
  if (indexOfGameScreen < 1 || indexOfGameScreen > 32) return;

  //copy score array
  unsigned int scoreCopy[32]; 
  for (int i = 0; i < 32; i++) scoreCopy[i] = score[i];

  unsigned int player[] = 
  {
     //sorted in order: WGRY
    1,2,3,4,5,6,7,8,
    9,10,11,12,13,14,15,16,
    17,18,19,20,21,22,23,24,
    25,26,27,28,29,30,31,32
  };

  //sort
  for (int i = 1; i < 32; i++)
  {
    unsigned int keyScore = scoreCopy[i];
    unsigned int keyPlayer = player[i];
    int j = i - 1;

    // Move elements of arr[0..i-1], that are greater than key, to one position ahead of their current position
    while (j >= 0 && scoreCopy[j] < keyScore)
    {
      scoreCopy[j + 1] = scoreCopy[j];
      player[j + 1] = player[j];
      j = j - 1;
    }
    scoreCopy[j + 1] = keyScore;
    player[j + 1] = keyPlayer;
  }

  String output = String(indexOfGameScreen);
  output+= ": ";
  int actualPlayer = player[indexOfGameScreen-1];
  if (actualPlayer < 9)
  {
    output+= GP_B;
    output+= String(actualPlayer);
  }
  else if (actualPlayer < 17) 
  {
    output+= GP_G;
    output+= String(actualPlayer-8);
  }
  else if (actualPlayer < 25) 
  {
    output+= GP_R;
    output+= String(actualPlayer-16);
  }
  else 
  {
    output+= GP_Y;
    output+= String(actualPlayer-24);
  }
  output+= "=";
  output += String(score[actualPlayer-1]);
  output += SCORE;
  setMenuText(output);
}
