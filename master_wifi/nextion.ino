void b0_but(void *ptr) //left top (Start + )
{
  switch (actualMenu)
  {
    case START_MENU:
      lastTime = actualTime;
      sendCommand("vis b0,0"); //hidde button
      sendCommand("vis b1,0"); //hidde button
      sendCommand("vis b2,0"); //hidde button
      b3.setText("Stop");
      actualMenu = GAME_RUN;
      break;
    case MENU1:
      if (lastChipUsed[0] != 0x00) addNewChip(lastChipUsed[0],lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], 'w');  
      Serial.println("b0-menu1");
      break;
    default: break;
  }
}

void b1_but(void *ptr) //left bottom
{
  Serial.println("b1");
  runCommand(ADMIN_PRINT);
}

void b1_press(void *ptr) //left bottom
{
  Serial.println("b1_press");
}

void b2_but(void *ptr) //right top (menu1 + )
{
  Serial.println("b2");
  if (lastChipUsed[0] != 0x00) removeChip(lastChipUsed[0], lastChipUsed[1], lastChipUsed[2], lastChipUsed[3]);
}

void b3_but(void *ptr)  //right bottom
{
  switch (actualMenu)
  {
    case GAME_RUN:
      b3.setText("Back");
      actualMenu = GAME_END;
      break;
    case GAME_END:
      sendCommand("vis b0,1"); //hidde button
      sendCommand("vis b1,1"); //hidde button
      sendCommand("vis b2,1"); //hidde button
      b3.setText("Menu3");
      actualMenu = START_MENU;
      break;
    default:  break;
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