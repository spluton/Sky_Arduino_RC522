 #include <SPI.h>
#include <MFRC522.h>
#include <stdlib.h>
#include <SoftwareSerial.h>
//#include <stdarg.h>  // For printf
#include <stdio.h>
//#include <MemoryFree.h>
#include "checksum.h"
#include <SD.h>
// set up variables using the SD utility library functions:


const int chipSelect = A1;
MFRC522 mfrc522(10, A0); // Create MFRC522 instance
extern "C" {
#include "Skylanders.h"
}

static FILE uartout = {0} ;
Checksum crc;
File myFile, dir;
char C_Uid[10] = {0x00};


////////////////////////////////
/////
/////
///////////////////////////////
void setup() {
  pinMode(A4, INPUT);
  digitalWrite(A4, HIGH);
  pinMode(A5, OUTPUT);
  digitalWrite(A5, LOW);
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  pinMode(4, INPUT);
  digitalWrite(4, HIGH);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  pinMode(10, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);    // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  ShowReaderDetails();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
  delay(500);
  // fill in the UART file descriptor with pointer to writer.
  //fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  // The uart is the standard output device STDOUT.
  //stdout = &uartout ;
  Serial.print("\nInitializing SD card...");
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if  (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present1.");
  }
    dir = SD.open("/");
}


////////////////////////////////
/////
/////
///////////////////////////////
void loop() {
  for (byte x = 0; x < 10; x++) {
    C_Uid[x] = 0x00;
  }

  //while (!digitalRead(2)) {

    while (!digitalRead(A4)) {
      delay(100);
      while (!digitalRead(A4));
      delay(100);
      myFile.close();
      myFile =   dir.openNextFile();
      while (myFile.isDirectory())     {
        myFile.close();
        myFile =   dir.openNextFile();
      }
      if (! myFile) {
        // no more files
        //break;
        dir.rewindDirectory();

      }
      //   Serial.println(freeMemory());
      Serial.println(myFile.name());
      //entry.close();
    }


  if (digitalRead(4)) {
    writecard();
    return;
  }
  byte Buffer[18];
  byte BufCRC[2];
  byte size = sizeof(Buffer);
  MFRC522::MIFARE_Key key;
  // put your main code here, to run repeatedly:
  byte status;
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    sprintf(C_Uid, "%s%02x", C_Uid, mfrc522.uid.uidByte[i]);
  }
  Serial.println(C_Uid);
  if (mfrc522.uid.sak == 0x81) {
    Serial.println("Copy from Skylanders");
  }
  if (mfrc522.uid.sak == 0x01) {
    Serial.println("Skylanders");
  }

  if (SD.exists(C_Uid)) {
    Serial.println("File Found. Exit.");
    mfrc522.PICC_HaltA();
    return;
  }
  for (byte i = 0; i < 64; i++) {
    GetOne(C_Uid, (i / 4), key.keyByte);
    for (uint8_t c = 0; c < 6; c++)  Serial.print(key.keyByte[c], HEX);
    Serial.println();
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, i, &key, &(mfrc522.uid));
    //  Serial.println(freeMemory());
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      ErrorHalt();
      return;
    }
    mfrc522.MIFARE_Read(i, Buffer, &size);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      ErrorHalt();
      return;
    }
    Serial.print(i);
    mfrc522.PCD_CalculateCRC(Buffer, 16, BufCRC);
    if ((Buffer[16] == BufCRC[0]) && (Buffer[17] == BufCRC[1])) {
      Serial.println("_OK");
    } else {
      Serial.println("_Faild CRC. Exit. Halt");
      ErrorHalt();
      return;
    }
    if (((i + 1) % 4) == 0) {
      for (byte x = 0; x < 6; x++) {
        Buffer[x] = key.keyByte[x];
      }
    }
    myFile = SD.open(C_Uid, FILE_WRITE);
    if (myFile) {
      myFile.write(Buffer, 16);
      myFile.close();
    } else {
      Serial.println("error opening File");
    }
  }
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  //Serial.println(freeMemory());
  Serial.print("Saved ");
  Serial.println(C_Uid);
}

////////////////////////////////
/////
/////
///////////////////////////////
void ShowReaderDetails() {
  // Get the MFRC522 software version
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("MFRC522 Software Version: 0x"));
  Serial.print(v, HEX);
  if (v == 0x91)
    Serial.print(F(" = v1.0"));
  else if (v == 0x92)
    Serial.print(F(" = v2.0"));
  else
    Serial.print(F(" (unknown)"));
  Serial.println("");
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
  }
}


////////////////////////////////
/////
/////
///////////////////////////////
void ErrorHalt() {
  SD.remove(C_Uid);
  Serial.print("Removed ");
  Serial.println(C_Uid);
  delay(3000);
  mfrc522.PCD_StopCrypto1();
  mfrc522.PICC_HaltA();
}


////////////////////////////////
/////
/////
///////////////////////////////
void writecard(void) {
  byte Buffer[32];
  byte BufCRC[2];
  int keytyp = 0;
  uint16_t crcblock = 0;
  byte size = 18 * sizeof(byte);
  MFRC522::MIFARE_Key key;

  // put your main code here, to run repeatedly:
  byte status;
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())  return;
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    sprintf(C_Uid, "%s%02x", C_Uid, mfrc522.uid.uidByte[i]);
  }
  Serial.println(C_Uid);

  keytyp = detect();
  if (keytyp == 3) {
    resetcard();
    return;
  }
  mfrc522.MIFARE_Read(0, Buffer, &size);
  mfrc522.PCD_CalculateCRC(Buffer, 16, BufCRC);
  for (byte i = 0; i < 6; i++) {
    Serial.print(key.keyByte[i], HEX);
  }
  if ((Buffer[16] == BufCRC[0]) && (Buffer[17] == BufCRC[1])) {
    Serial.println("_OK _ CRC ");
  }
  Serial.println(myFile.name());
  //myFile = SD.open("FIRE", FILE_READ); //Open File fuer flash
  if (myFile) {
    myFile.seek(16);
    for (byte i = 16; i < 31; i++) {
      Buffer[i] = myFile.read();
    }
  } else {
    Serial.println("error opening File");
    return;
  }
  crcblock = crc.ComputeCcittCrc16(Buffer, 30);
  Buffer[31] = (crcblock >> 8) & 0xff;
  Buffer[30] = crcblock & 0xff;

  for (byte i = 0; i < 16; i++) {
    Buffer[i] = Buffer[i + 16];
  }

  for (byte i = 16; i < 32; i++) {
    Buffer[i] = 0x00;
  }

  for (byte i = 0; i < 32; i++) {
    Serial.print(Buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  status = mfrc522.MIFARE_Write(1, &Buffer[0], 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write block 1 failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    mfrc522.PCD_StopCrypto1();
  }
  for (byte i = 3; i < 64; i++) {
    delay(10);
    SendAuthKey(i, keytyp);
    if ((((i + 1) % 4) == 0) & ( keytyp != 3))  {
      for (byte x = 0; x < 6; x++) {
        GetOne(C_Uid, (i / 4), key.keyByte);
        Buffer[x] = key.keyByte[x];
      }

      if (i == 3) {
        Buffer[6] = 0x0f;
        Buffer[7] = 0x0f;
        Buffer[8] = 0x0f;
        Buffer[9] = 0x69;
      } else {
        Buffer[6] = 0x7f;
        Buffer[7] = 0x0f;
        Buffer[8] = 0x08;
        Buffer[9] = 0x69;
      }

      for (byte x = 0; x < 6; x++) {
        Buffer[x + 10] = 0x00;

      }
      status = mfrc522.MIFARE_Write(i, &Buffer[0], 16);

      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        mfrc522.PCD_StopCrypto1();
        break;
      }
    }
    if (((i + 1) % 4) != 0) {
      status = mfrc522.MIFARE_Write(i, &Buffer[16], 16);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        mfrc522.PCD_StopCrypto1();
        break;
      }
    }


  }

  Serial.print(" DONE ");

  mfrc522.PCD_StopCrypto1();
  delay(1999);
  return;
}

////////////////////////////////
/////
/////
///////////////////////////////
int SendAuthKey(int i, int typ ) {
  MFRC522::MIFARE_Key key;
  int status = 0;
  //Serial.println(i);
  //Serial.println(typ);
  switch (typ) {
    case 1:
      key.keyByte[0] = 0x00;
      key.keyByte[1] = 0x00;
      key.keyByte[2] = 0x00;
      key.keyByte[3] = 0x00;
      key.keyByte[4] = 0x00;
      key.keyByte[5] = 0x00;

      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, i, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate(00 for write) failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        mfrc522.PCD_StopCrypto1();
        return 0;
      } else return 1;
    case 2:
      key.keyByte[0] = 0xff;
      key.keyByte[1] = 0xff;
      key.keyByte[2] = 0xff;
      key.keyByte[3] = 0xff;
      key.keyByte[4] = 0xff;
      key.keyByte[5] = 0xff;

      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, i, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate(ff for write) failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        mfrc522.PCD_StopCrypto1();
      } else return 2;
    case 3:

      GetOne(C_Uid, (i / 4), key.keyByte);

      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, i, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate(SK for write) failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        mfrc522.PCD_StopCrypto1();
      } else return 3;
    default:
      return false;
  }
}


////////////////////////////////
/////
/////
///////////////////////////////
int detect(void) {
  MFRC522::MIFARE_Key key;
  int status = 0;
  mfrc522.PCD_StopCrypto1();
  while ( ! mfrc522.PICC_IsNewCardPresent());
  while (! mfrc522.PICC_ReadCardSerial());
  key.keyByte[0] = 0x00;
  key.keyByte[1] = 0x00;
  key.keyByte[2] = 0x00;
  key.keyByte[3] = 0x00;
  key.keyByte[4] = 0x00;
  key.keyByte[5] = 0x00;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 0, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate(00) failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    //    mfrc522.PCD_StopCrypto1();
  } else return 1;
  mfrc522.PCD_StopCrypto1();
  while ( ! mfrc522.PICC_IsNewCardPresent());
  while (! mfrc522.PICC_ReadCardSerial());
  key.keyByte[0] = 0xff;
  key.keyByte[1] = 0xff;
  key.keyByte[2] = 0xff;
  key.keyByte[3] = 0xff;
  key.keyByte[4] = 0xff;
  key.keyByte[5] = 0xff;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 0, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate(FF) failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    //  mfrc522.PCD_StopCrypto1();
  } else return 2;
  mfrc522.PCD_StopCrypto1();
  while ( !mfrc522.PICC_IsNewCardPresent());
  while (!mfrc522.PICC_ReadCardSerial());
  key.keyByte[0] = 0x4b;
  key.keyByte[1] = 0x0b;
  key.keyByte[2] = 0x20;
  key.keyByte[3] = 0x10;
  key.keyByte[4] = 0x7c;
  key.keyByte[5] = 0xcb;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 0, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate(SK) failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  } else return 3;
}


////////////////////////////////
/////
/////
///////////////////////////////
int resetcard() {

  int status = 0;
  byte Buffer[16] = {0x00,  0x00  ,  0x00  ,  0x00  ,  0x00  ,  0x00  ,  0xff  ,  0x07  ,  0x80  ,  0x69  ,  0xff  ,  0xff  ,  0xff  ,  0xff  ,  0xff ,  0xff};
  while ( ! mfrc522.PICC_IsNewCardPresent())mfrc522.PCD_StopCrypto1();
  while (! mfrc522.PICC_ReadCardSerial());

  if (mfrc522.MIFARE_OpenUidBackdoor(true)) {
    Serial.println("Reset card start");
    for (short i = 3; i < 1024; i = i + 4) {
      //   Serial.println(i);
      status = mfrc522.MIFARE_Write(i, &Buffer[0], 16);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        mfrc522.PCD_StopCrypto1();
        break;

      }
    }
    Serial.println("Done");
  }
  mfrc522.PCD_StopCrypto1();
  mfrc522.PICC_HaltA();

}
