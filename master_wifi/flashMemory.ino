void firstTimeMemoryCheck()
{
  preferences.begin("my-app", false); //open preferences "my-app". False is "no read-only"
  if (preferences.getInt("statusStart", -1) != -1)
  {
    Serial.println("memory used");
  }
  else
  {
    preferences.putInt("statusStart", 1);
    preferences.end(); //have to end use preferences
    Serial.println("memory created");
    createMemory();
  }
}

void createMemory()
{
  preferences.begin("my-app", false);
  //GROUPs
  // 8 chips saved in array (1st chip = index 0-3, 8th chip = index 29-32)
  byte groupArrayW[32] = 
  {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,         
    0x00, 0x00, 0x00, 0x00, 
  };
  preferences.putBytes("groupArrayW", groupArrayW, 32);

    byte groupArrayG[32] = 
  {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,         
    0x00, 0x00, 0x00, 0x00, 
  };
  preferences.putBytes("groupArrayG", groupArrayG, 32);

  byte groupArrayR[32] = 
  {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,         
    0x00, 0x00, 0x00, 0x00,  
  };
  preferences.putBytes("groupArrayR", groupArrayR, 32);

  byte groupArrayY[32] = 
  {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,         
    0x00, 0x00, 0x00, 0x00, 
  };
  preferences.putBytes("groupArrayY", groupArrayY, 32);
  preferences.end(); // Ukončí práci s Preferences
}

void clearAllData()
{
  preferences.begin("my-app", false); //open preferences "my-app". False is "no read-only"
  preferences.clear();
  Serial.println("all data removed!");
  preferences.end(); //have to end use preferences
  createMemory(); //rebuild data to default values
  actualTime = 0;
  lastChipUsed[0] = 0x00;
  lastChipUsed[1] = 0x00;
  lastChipUsed[2] = 0x00;
  lastChipUsed[3] = 0x00;
}

void resetMFRC()
{
  Serial.println("RC522 was reset!");
  mfrc522.PCD_Reset();
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init();  
}

void addNewChip(byte byte1, byte byte2, byte byte3, byte byte4, char group)
{
  if (group == '\0') return;
  preferences.begin("my-app", false); //open preferences "my-app". False is "no read-only"
  byte groupArray[32];

  //Pull data (saved chips)
  switch(group)
  {
    case 'w': 
    {
      preferences.getBytes("groupArrayW", groupArray, 32);
      break;
    }
    case 'g': 
    {
      preferences.getBytes("groupArrayG", groupArray, 32);
      break;
    }
    case 'y': 
    {
      preferences.getBytes("groupArrayY", groupArray, 32);
      break;
    }
    case 'r': 
    {
      preferences.getBytes("groupArrayR", groupArray, 32);
      break;
    };
    default:
    {};
  }
  int indexOfChip = 0;
  bool fullyOccupied = true;
  for (int i = 0; i < 32; i += 4)
  {
    bool freePosition = true;
    for (int j = 0; j < 4; j++)
    {
      if (groupArray[i+j] != 0x00)
      {
        freePosition = false;
        break;
      }
    }
    if (freePosition)
    {
      fullyOccupied = false;
      break;
    }
    indexOfChip++;
  }

  if (fullyOccupied) return;

  //Push new data
  groupArray[(indexOfChip)*4] = byte1;
  groupArray[(indexOfChip)*4+1] = byte2;
  groupArray[(indexOfChip)*4+2] = byte3;
  groupArray[(indexOfChip)*4+3] = byte4;

  //Save data
  switch(group)
  {
    case 'w': 
    {
      preferences.putBytes("groupArrayW", groupArray, 32);
      break;
    }
    case 'g':
    {
      preferences.putBytes("groupArrayG", groupArray, 32);
      break;
    }
    case 'y':
    {
      preferences.putBytes("groupArrayY", groupArray, 32);
      break;
    }
    case 'r':
    {
      preferences.putBytes("groupArrayR", groupArray, 32);
      break;
    };
    default:
    {
    };
  }
  preferences.end(); //have to end use preferences
}
