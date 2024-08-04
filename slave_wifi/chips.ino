void rfidCheck()
{
  if ( !mfrc522.PICC_IsNewCardPresent()) 
  {
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

  if(mfrc522.uid.uidByte[0] == 0x80 & mfrc522.uid.uidByte[1] == 0x71 & mfrc522.uid.uidByte[2] == 0xD8 & mfrc522.uid.uidByte[3] == 0x55) 
  {
  }
  else if(mfrc522.uid.uidByte[0] == 0xF3 & mfrc522.uid.uidByte[1] == 0xA5 & mfrc522.uid.uidByte[2] == 0xBD & mfrc522.uid.uidByte[3] == 0xC8) 
  {
  }
  else 
  {
    Serial.println("Detekovan neznamy mfrc522 tag!");
    //sendData(123456, 0x01, 0x02, 0x03, 0x04, slaveID);
  }

  Serial.println();
  // ukončení komunikace a jejího zabezpečení
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}