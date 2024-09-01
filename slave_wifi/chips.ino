
void hexPrint(byte *buffer, byte bufferSize) 
{
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

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
  
  // WRITE INFORMATION
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

  // CHECK SUPPORTED TYPES
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    //Serial.println("Tento mfrc522 tag neni podporovany (typ MIFARE Classic).");
    return;
  }
  // write hex id
  // Serial.print("Adresa mfrc522 tagu: ");
  // hexPrint(mfrc522.uid.uidByte, mfrc522.uid.size);
  // Serial.println();


  if (isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], B_COLOR) != -1)
  {
    if (getGroupByLight() == B_COLOR) useChipOnLight(B_COLOR, isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], B_COLOR));
    //Serial.println("known chip - group b!\n");
  }
  else if (isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], Y_COLOR) != -1)
  {
    if (getGroupByLight() == Y_COLOR) useChipOnLight(Y_COLOR, isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], Y_COLOR));
    //Serial.println("known chip - group y!\n");
  }
  else if (isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], G_COLOR) != -1)
  {
    //Serial.println("known chip - group g!\n");
    if (getGroupByLight() == G_COLOR) useChipOnLight(G_COLOR, isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], G_COLOR));
  }
  else if (isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], R_COLOR) != -1)
  {
    //Serial.println("known chip - group r!\n");
    if (getGroupByLight() == R_COLOR) useChipOnLight(R_COLOR, isKnowChip(mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3], R_COLOR));
  }
  else 
  {
    //Serial.println("Detekovan neznamy mfrc522 tag!\n");
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
  case B_COLOR:
    memcpy(groupArray, groupArrayB, sizeof(groupArrayB));
    break;
  case Y_COLOR:
    memcpy(groupArray, groupArrayY, sizeof(groupArrayY));
    break;
  case G_COLOR:
    memcpy(groupArray, groupArrayG, sizeof(groupArrayG));
    break;
  case R_COLOR:
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
