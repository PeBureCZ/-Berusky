void b0_Release(void *ptr)
{
  if (lastChipUsed[0] != 0x00) addNewChip(lastChipUsed[0],lastChipUsed[1],lastChipUsed[2],lastChipUsed[3], 'w');  
  runCommand(ADMIN_PRINT);
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