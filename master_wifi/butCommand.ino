enum Commands
{
  START_GAME = 0,
  END_GAME,
  RESET_GAME,
  RESET_HW,
  ADD_CHIP,
  REMOVE_CHIP,
  BACK,
  SHOW_MENU,
  ADMIN_PRINT,
};

void resetScore()
{ 
  for (int i = 0; i < 32; i++) score[i] = 0;
  Serial.println("reset score");
}

bool runCommand(int command)
{
  switch (command)
  {
    case START_GAME:
    {
      break;
    }    
    case END_GAME:
    {
      break;
    }    
    case RESET_GAME:
    {
      break;
    }
    case ADMIN_PRINT:
    {
      adminCommand();
      break;
    }
    case RESET_HW:
    {
      clearAllData();
      break;
    }
    default: {return false;}
  }
  return true;
}

void adminCommand()
{
  String output = "ADMIN PRINT\n";

  preferences.begin("my-app", true); //open preferences "my-app". True is "read-only"

  byte groupArrayB[32];
  byte groupArrayR[32]; 
  byte groupArrayY[32]; 
  byte groupArrayG[32]; 
  preferences.getBytes("groupArrayB", groupArrayB, 32);
  preferences.getBytes("groupArrayR", groupArrayR, 32);
  preferences.getBytes("groupArrayY", groupArrayY, 32);
  preferences.getBytes("groupArrayG", groupArrayG, 32);

  output += "chipy b: ";
  for (int i = 0; i < 32; i++) 
  {
    output += String(groupArrayB[i], HEX);
    output += ", ";
  }
  output += " \n";

  output += "chipy r: ";
  for (int i = 0; i < 32; i++) 
  {
    output += String(groupArrayR[i], HEX);
    output += ", ";
  }
  output += " \n";

  output += "chipy y: ";
  for (int i = 0; i < 32; i++) 
  {
    output += String(groupArrayY[i], HEX);
    output += ", ";
  }
  output += " \n";

  output += "chipy g: ";
  for (int i = 0; i < 32; i++) 
  {
    output += String(groupArrayG[i], HEX);
    output += ", ";
  }
  output += " \n";

  Serial.println(output);

  preferences.end(); //have to end use preferences
}