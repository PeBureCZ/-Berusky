void b0_but(void *ptr)
{
  if (lastChipUsed[0] != 0x00) addNewChip(lastChipUsed[0],lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], 'w');  
  Serial.println("b0");
}

void b1_but(void *ptr)
{
  Serial.println("b1");
  runCommand(ADMIN_PRINT);
}

void b1_press(void *ptr)
{
  Serial.println("b1_press");
}

void b2_but(void *ptr)
{
  Serial.println("b2");
  if (lastChipUsed[0] != 0x00) removeChip(lastChipUsed[0], lastChipUsed[1], lastChipUsed[2], lastChipUsed[3]);
}

void b3_but(void *ptr)
{
  Serial.println("b3");
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
  // output += '(';
  // output += 'w';
  // output += 'r';
  // output += 'g';
  // output += 'y';
  // output += ')';
  t0.setText(output.c_str());
}