
int getChipIndex(byte byte1, byte byte2, byte byte3, byte byte4, char group)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

bool isKnowChip(byte byte1, byte byte2, byte byte3, byte byte4)
{
  if (isKnowChip(byte1, byte2, byte3, byte4, 'w') != -1) return true;
  else if (isKnowChip(byte1, byte2, byte3, byte4, 'r')!= -1) return true;
  else if (isKnowChip(byte1, byte2, byte3, byte4, 'g')!= -1) return true;
  else if (isKnowChip(byte1, byte2, byte3, byte4, 'y')!= -1) return true;
  return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

int isKnowChip(byte byte1, byte byte2, byte byte3, byte byte4, char group)
{
  //read chips
  preferences.begin("my-app", false); //open preferences "my-app". False is "no read-only"
  byte groupArray[32];
  switch (group)
    {
    case 'w':
      preferences.getBytes("groupArrayW", groupArray, 32);
      break;
    case 'y':
      preferences.getBytes("groupArrayY", groupArray, 32);
      break;
    case 'g':
      preferences.getBytes("groupArrayG", groupArray, 32);
      break;
    case 'r':
      preferences.getBytes("groupArrayR", groupArray, 32);
      break;
    default:
      break;
    }

    for (size_t i = 0; i < 32; i += 4)
    {
      if(groupArray[i] == byte1 && groupArray[i+1] == byte2 && groupArray[i+2] == byte3 && groupArray[i+3] == byte4)
      {
        preferences.end();
        return i / 4;
      }
    }
  preferences.end();
  return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void rfidCheck()
{
  
   //waiting the card approach
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  // Select a card
  if (!mfrc522.PICC_ReadCardSerial()) return;
  //version of mfrc522 tag
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println("test");
  //check supported tag
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    //not supported type of chip (MIFARE Classic);
    return;
  }
  //print HEX chip ID
  Serial.print("chip ID: ");
  hexPrint(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  
  lastChipUsed[0] = mfrc522.uid.uidByte[0];
  lastChipUsed[1] = mfrc522.uid.uidByte[1];
  lastChipUsed[2] = mfrc522.uid.uidByte[2];
  lastChipUsed[3] = mfrc522.uid.uidByte[3];
 
  setMenuToChip();

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void hexPrint(byte *buffer, byte bufferSize) 
{
  for (byte i = 0; i < bufferSize; i++) 
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void addNewChip(byte byte1, byte byte2, byte byte3, byte byte4, char group)
{
  if (group == '\0') return;
  if (isKnowChip(byte1, byte2, byte3, byte4, group) != -1) return;
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
      Serial.println("test2");
      fullyOccupied = false;
      break;
    }
    indexOfChip++;
  }

  if (fullyOccupied) return;
  Serial.println(lastChipUsed[0]);
  lastChipUsed[0] = 0x00;
  lastChipUsed[1] = 0x00;
  lastChipUsed[2] = 0x00;
  lastChipUsed[3] = 0x00;
  setMenuToChip();

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void removeChip(byte byte1, byte byte2, byte byte3, byte byte4, char group)
{
  byte groupArray[32];
  preferences.begin("my-app", false); //open preferences "my-app". False is "no read-only"
  switch (group)
  {
    case 'w':
      preferences.getBytes("groupArrayW", groupArray, 32);
      break;
    case 'g':
      preferences.getBytes("groupArrayG", groupArray, 32);
      break;
    case 'y':
      preferences.getBytes("groupArrayY", groupArray, 32);
      break;
    case 'r':
      preferences.getBytes("groupArrayR", groupArray, 32);
      break;
    default:
      break;
  }
  int numOfChip = 0;
  for (numOfChip; numOfChip < 32; numOfChip += 4)
  {
    if (groupArray[numOfChip] == byte1 && groupArray[numOfChip+1] == byte2 && groupArray[numOfChip+2] == byte3 && groupArray[numOfChip+3] == byte4)
    {
        groupArray[numOfChip] = 0x00;
        groupArray[numOfChip+1] = 0x00;
        groupArray[numOfChip+2] = 0x00;
        groupArray[numOfChip+3] = 0x00;
        switch (group)
        {
          case 'w':
            preferences.putBytes("groupArrayW", groupArray, 32);
            break;
          case 'g':
            preferences.putBytes("groupArrayG", groupArray, 32);
            break;
          case 'y':
            preferences.putBytes("groupArrayY", groupArray, 32);
            break;
          case 'r':
            preferences.putBytes("groupArrayR", groupArray, 32);
            break;
          default:
            break;
        }
    }
  }
  preferences.end();
  setMenuToChip();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void removeChip(byte byte1, byte byte2, byte byte3, byte byte4)
{
  removeChip(byte1, byte2, byte3, byte4, 'w');
  removeChip(byte1, byte2, byte3, byte4, 'r');
  removeChip(byte1, byte2, byte3, byte4, 'g');
  removeChip(byte1, byte2, byte3, byte4, 'y');
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
