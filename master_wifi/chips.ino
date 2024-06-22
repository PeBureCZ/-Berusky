char isKnowChip(byte byte1, byte byte2, byte byte3, byte byte4)
{
  //read chips
  preferences.begin("my-app", false); //open preferences "my-app". False is "no read-only"

  byte groupArrayW[32];
  byte groupArrayY[32];
  byte groupArrayR[32];
  byte groupArrayG[32];
  preferences.getBytes("groupArrayW", groupArrayW, 32);
  preferences.getBytes("groupArrayY", groupArrayY, 32);
  preferences.getBytes("groupArrayR", groupArrayR, 32);
  preferences.getBytes("groupArrayG", groupArrayG, 32);
  char groupChar = '0';

  for (size_t groupIndex = 0; groupIndex < 4; groupIndex++)
  {
    byte *testedGroup;

    switch (groupIndex)
    {
      case 1:
        testedGroup = groupArrayW;
        groupChar = 'w';
        break;
      case 2:
        testedGroup = groupArrayY;
        groupChar = 'y';
        break;
      case 3:
        testedGroup = groupArrayR;
        groupChar = 'r';
        break;
      default:
        testedGroup = groupArrayG;
        groupChar = 'g';
        break;
    }

    for (size_t i = 0; i < 32; i += 4)
    {
      if(testedGroup[i] == byte1 && testedGroup[i+1] == byte2 && testedGroup[i+2] == byte3 && testedGroup[i+3] == byte4)
      {
        preferences.end();
        return groupChar;
      }
    }
  }
  groupChar = '0';
  preferences.end();
  return groupChar;
}

void rfidCheck()
{
   //waiting the card approach
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  // Select a card
  if (!mfrc522.PICC_ReadCardSerial()) return;
  //version of mfrc522 tag
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

  //check supported tag
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    //Serial.println("Tento mfrc522 tag neni podporovany (typ MIFARE Classic).");
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

  //TODO: check if chip is in any group (wgry) - re-wrote information on main menu screen

  //only test solution 
  if (mfrc522.uid.uidByte[0] == 0xA3 && mfrc522.uid.uidByte[1] == 0x69 && mfrc522.uid.uidByte[2] == 0x56 && mfrc522.uid.uidByte[3] == 0xC9) runCommand(RESET_HW);  
  
  setMenuText();

  char group = isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3]);
  Serial.println(group);
  if (group != '0')
  {
    Serial.println("known chip");   
  }
  else Serial.println("unknown chip!");

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void hexPrint(byte *buffer, byte bufferSize) 
{
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void recieveData(WiFiClient client)
{
  uint8_t dataStr[2]; 
  client.readBytes(dataStr, 2);
  Serial.print("Received data: ");
  Serial.print(dataStr[0]);
  Serial.print("+");
  Serial.println(dataStr[1]);
}
