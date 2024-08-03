void b0_but(void *ptr) //left top RELEASE
{
  switch (actualMenu)
  {
    case START_MENU:
    {
      lastTime = actualTime;
      sendCommand("vis b1,0"); //hidde button
      b3.setText("Stop");
      b0.setText("<<");
      b2.setText(">>");
      actualMenu = GAME_RUN;
      break;
    }
    case MENU1_W:
    {
      if (lastChipUsed[0] != 0x00) addNewChip(lastChipUsed[0],lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], 'w');  
      Serial.println("add to w");
      break;
    }
    case MENU1_G:
    {
      if (lastChipUsed[0] != 0x00) addNewChip(lastChipUsed[0],lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], 'g');  
      Serial.println("add to g");
      break;
    }

    case MENU1_R:
    {
      if (lastChipUsed[0] != 0x00) addNewChip(lastChipUsed[0],lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], 'r');  
      Serial.println("add to r");
      break;
    }
    case MENU1_Y:
    {
      if (lastChipUsed[0] != 0x00) addNewChip(lastChipUsed[0],lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], 'y');  
      Serial.println("add to y");
      break;
    }
    case MENU3_MIN_ON: //click on "+" time
    {
      if (minLightOn + 1 >= maxLightOn) break;
      minLightOn++;
      String newMenuText = "minON = ";
      newMenuText += minLightOn;
      setMenuText(newMenuText);
      break;
    }
    case MENU3_MAX_ON: //click on "+" time
    {
      if (maxLightOff == 10000) break;
      maxLightOn++;
      String newMenuText = "maxON = ";
      newMenuText += maxLightOn;
      setMenuText(newMenuText);  
      break;   
    }
    case MENU3_MIN_OFF: //click on "+" time
    {
      if (minLightOff + 1 >= maxLightOff) break;
      minLightOff++; 
      String newMenuText = "minOFF = ";
      newMenuText += minLightOff;
      setMenuText(newMenuText);
      break;  
    }
    case MENU3_MAX_OFF: //click on "+" time
    {
      if (maxLightOff == 10000) break;
      maxLightOff++;   
      String newMenuText = "maxOFF = ";
      newMenuText += maxLightOff;
      setMenuText(newMenuText);
      break;
    }
    default: break;
  }
}

void b1_but(void *ptr) //left bottom RELEASE
{

  switch (actualMenu)
  {
    case START_MENU:
    {
      Serial.println("ONLY TEMPORARY!");
      runCommand(ADMIN_PRINT);
      break;
    }
    case MENU1_W:
    case MENU1_G:
    case MENU1_R:
    case MENU1_Y:
    {
      if ((MENU1_W || MENU1_G || MENU1_R || MENU1_Y) && actualTime - pressedTime > 3000)
      {
        Serial.println("holded B1 click");
        clearAllData();
        String newMenuText = "Data cleared";
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
    case MENU3_MIN_ON: //click on "+" time
    {
      if (minLightOn == 0) break;
      minLightOn--;
      String newMenuText = "minON = ";
      newMenuText += minLightOn;
      setMenuText(newMenuText);
      break;
    }
    case MENU3_MAX_ON: //click on "+" time
    {
      if (maxLightOn - 1 <= minLightOn) break;
      maxLightOn--;  
      String newMenuText = "maxON = ";
      newMenuText += maxLightOn;
      setMenuText(newMenuText);
      break;    
    }
    case MENU3_MIN_OFF: //click on "+" time
    {
      if (minLightOff == 0) break;
      minLightOff--;  
      String newMenuText = "minOFF = ";
      newMenuText += minLightOff;
      setMenuText(newMenuText);
      break;    
    }
    case MENU3_MAX_OFF: //click on "+" time
    {
      if (maxLightOff - 1 <= minLightOff ) break;
      maxLightOff--;   
      String newMenuText = "maxOFF = ";
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
      b0.setText("Add");
      b1.setText("Remove");
      b2.setText("gpW");
      b3.setText("Back");
      actualMenu = MENU1_W;
      break;
    }    
    case MENU1_W:
    {
      b2.setText("gpG");
      actualMenu = MENU1_G;
      break;
    }
    case MENU1_G:
    {
      b2.setText("gpR");
      actualMenu = MENU1_R;
      break;
    }
    case MENU1_R:
    {
      b2.setText("gpY");
      actualMenu = MENU1_Y;
      break;
    }
    case MENU1_Y:
    {
      b2.setText("gpW");
      actualMenu = MENU1_W;
      break;
    }
    case MENU3_MIN_ON:
    {
      b2.setText("maxON");
      String newMenuText = "maxON = ";
      newMenuText += maxLightOn;
      setMenuText(newMenuText);
      actualMenu = MENU3_MAX_ON;
      break;     
    }
    case MENU3_MAX_ON:
    {
      b2.setText("minOFF");
      String newMenuText = "minOFF = ";
      newMenuText += minLightOff;
      setMenuText(newMenuText);
      actualMenu = MENU3_MIN_OFF;
      break;     
    }
    case MENU3_MIN_OFF:
    {
      b2.setText("maxOFF");
      String newMenuText = "maxOFF = ";
      newMenuText += maxLightOff;
      setMenuText(newMenuText);
      actualMenu = MENU3_MAX_OFF;
      break;     
    }
    case MENU3_MAX_OFF:
    {
      b2.setText("minON");
      String newMenuText = "minON = ";
      newMenuText += minLightOn;
      setMenuText(newMenuText);
      actualMenu = MENU3_MIN_ON;
      break;     
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
      b0.setText("+");
      b1.setText("-");
      b2.setText("minON");
      String newMenuText = "minON = ";
      newMenuText += minLightOn;
      setMenuText(newMenuText);
      b3.setText("Back");
      actualMenu = MENU3_MIN_ON;
      break;
    }
    case GAME_RUN:
    {
      b3.setText("Back");
      actualMenu = GAME_END;
      break;
    }
    case GAME_END:
    {
      sendCommand("vis b1,1"); //hidde button
      b0.setText("Start");
      b2.setText("Menu1");
      b3.setText("Menu3");
      actualMenu = START_MENU;
      break;
    }
    default: //DEFF = "Back"
    {
      b0.setText("Start");
      b1.setText("Menu2");
      b2.setText("Menu1");
      b3.setText("Menu3");
      actualMenu = START_MENU;
      String newMenuText = "";
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
  
  if (lastChipUsed[0] == 0x00) output += "(empty)";
  else
  {
    for (int i = 0; i < 4; i++)
    {
      char group;
      if (i == 0) group = 'w';
      else if (i == 1) group = 'r';
      else if (i == 2) group = 'y';
      else if (i == 3) group = 'g';

      int indexOfChip = isKnowChip(lastChipUsed[0], lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], group);
      if (indexOfChip != -1)
      {
        output += "(";
        output += group;
        output += indexOfChip;
        output += ")";
        break;
      }
    }
  } 
  setMenuText(output);
}