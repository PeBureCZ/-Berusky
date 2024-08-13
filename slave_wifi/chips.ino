void rfidCheck()
{
  if ( !mfrc522.PICC_IsNewCardPresent()) 
  {
    //Serial.println("test1\n");
    return;
  }
  // Select a card
  if ( !mfrc522.PICC_ReadCardSerial()) 
  {
    //Serial.print("test2\n");
    return;
  }

  Serial.println("chip readed");
  
  // WRITE INFORMATION
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print("mfrc522 tag typu: ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  // CHECK SUPPORTED TYPES
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println("Tento mfrc522 tag neni podporovany (typ MIFARE Classic).");
    return;
  }
  // write hex id
  Serial.print("Adresa mfrc522 tagu: ");
  hexPrint(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();

  // if(mfrc522.uid.uidByte[0] == 0x80 & mfrc522.uid.uidByte[1] == 0x71 & mfrc522.uid.uidByte[2] == 0xD8 & mfrc522.uid.uidByte[3] == 0x55) 
  // {
  // }
  // else if(mfrc522.uid.uidByte[0] == 0xF3 & mfrc522.uid.uidByte[1] == 0xA5 & mfrc522.uid.uidByte[2] == 0xBD & mfrc522.uid.uidByte[3] == 0xC8) 
  // {
  // }
  if (isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], 'w') != -1)
  {
    Serial.println("known chip - group w!\n");
  }
  else if (isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], 'y') != -1)
  {
    Serial.println("known chip - group y!\n");
  }
  else if (isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], 'g') != -1)
  {
    Serial.println("known chip - group g!\n");
  }
  else if (isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], 'r') != -1)
  {
    Serial.println("known chip - group r!\n");
  }
  else 
  {
    Serial.println("Detekovan neznamy mfrc522 tag!\n");
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

int isKnowChip(byte byte1, byte byte2, byte byte3, byte byte4, char group)
{
  //read chips
  byte groupArray[32];
  switch (group)
  {
  case 'w':
    memcpy(groupArray, groupArrayW, sizeof(groupArrayW));
    break;
  case 'y':
    memcpy(groupArray, groupArrayY, sizeof(groupArrayY));
    break;
  case 'g':
    memcpy(groupArray, groupArrayG, sizeof(groupArrayG));
    break;
  case 'r':
    memcpy(groupArray, groupArrayR, sizeof(groupArrayR));
    break;
  default:
    break;
  }

  for (size_t i = 0; i < 32; i += 4)
  {
    if(groupArray[i] == byte1 && groupArray[i+1] == byte2 && groupArray[i+2] == byte3 && groupArray[i+3] == byte4)
    {
      return i / 4;
    }
  }
  return -1;
}