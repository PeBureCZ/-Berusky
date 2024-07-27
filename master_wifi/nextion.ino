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
    {
      if (lastChipUsed[0] != 0x00) removeChip(lastChipUsed[0], lastChipUsed[1], lastChipUsed[2], lastChipUsed[3]);
      else Serial.println("nothing to delete");
      Serial.println("chip removed");
      break;
    }
    default: {Serial.println("nothing");}
  }
}

void b1_press(void *ptr) //left bottom PRESS ONLY
{
  //Serial.println("b1_press");
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
    default: break;
  }
}

void b3_but(void *ptr)  //right bottom RELEASE
{
  switch (actualMenu)
  {
    case GAME_RUN:
      b3.setText("Back");
      actualMenu = GAME_END;
      break;
    case GAME_END:
      sendCommand("vis b1,1"); //hidde button
      b0.setText("Start");
      b2.setText("Menu1");
      b3.setText("Menu3");
      actualMenu = START_MENU;
      break;
    case MENU1_G:
    case MENU1_R:
    case MENU1_Y:
    case MENU1_W:
    {
      b0.setText("Start");
      b1.setText("Menu2");
      b2.setText("Menu1");
      b3.setText("Menu3");
      actualMenu = START_MENU;
      break;
    }
    default:
    {
      
    }
  }
}

void setMenuText()
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
  else if (isKnowChip(lastChipUsed[0], lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], 'w')) output += "(w)";
  else if (isKnowChip(lastChipUsed[0], lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], 'r')) output += "(r)";
  else if (isKnowChip(lastChipUsed[0], lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], 'y')) output += "(y)";
  else if (isKnowChip(lastChipUsed[0], lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], 'g')) output += "(g)";
  
  t0.setText(output.c_str());
}