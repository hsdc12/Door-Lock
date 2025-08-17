#include <Arduino.h>
#include "RFID.h"
#include <SPI.h>
#include <EEPROM.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 myRFID(SS_PIN, RST_PIN);   // Create MFRC522 instance.

#define PASSWORD_ADDR 0     // Start of password in EEPROM
#define CARD1_ADDR      (PASSWORD_ADDR + PASSWORD_SIZE)       // 16
#define CARD2_ADDR      (CARD1_ADDR + 12)                     // 28
#define CARD3_ADDR      (CARD2_ADDR + 12)                     // 40
#define CARD4_ADDR      (CARD3_ADDR + 12)                     // 52

#define CARD_SIZE       12
#define PASSWORD_SIZE   16

char scannedID[CARD_SIZE + 1];  // buffer for scanned card hex string
int len = myRFID.uid.size;

extern rgb_lcd lcd;

void initRFID() {
  noTone(2); // Stop any tone that might be playing
  SPI.begin();      // Initiate  SPI bus
  myRFID.PCD_Init();   // Initiate MFRC522
  myRFID.PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_max); // Set the antenna gain to maximum
}


void handleRFID() {

     // Wait for RFID cards to be scanned
  if ( ! myRFID.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // an RFID card has been scanned but no UID 
  if ( ! myRFID.PICC_ReadCardSerial()) 
  {

    return;
  }
  char scannedID[CARD_SIZE + 1];
  int len = myRFID.uid.size; // UID length in bytes
  for (int i = 0; i < len && i < CARD_SIZE / 2; i++) {
    sprintf(&scannedID[i * 2], "%02X", myRFID.uid.uidByte[i]);
  }
  scannedID[len * 2] = '\0';  // Null-terminate

  // Read all stored cards
  char storedCard1[CARD_SIZE + 1];
  char storedCard2[CARD_SIZE + 1];
  char storedCard3[CARD_SIZE + 1];
  char storedCard4[CARD_SIZE + 1];

  readStoredCard(1, storedCard1);
  readStoredCard(2, storedCard2);
  readStoredCard(3, storedCard3);
  readStoredCard(4, storedCard4);

 if (strcmp(scannedID, storedCard1) == 0) {
    AuthorizedAccess();
  } else if (strcmp(scannedID, storedCard2) == 0) {
    AuthorizedAccess();
  } else if (strcmp(scannedID, storedCard3) == 0) {
    AuthorizedAccess();
  } else if (strcmp(scannedID, storedCard4) == 0) {
    AuthorizedAccess();
  } 
  else   {
    lcd.setCursor(0, 0);
    lcd.print("           ");
    lcd.setCursor(0, 0);   
    lcd.print("Denied!");
    
    tone(2, 200, 600);
    delay(600); // Wait for 600 milliseconds

    lcd.setCursor(0, 0);
    lcd.print("PIN:______  ");
    lcd.setCursor(0, 0);  //clear text
  }
}

void AuthorizedAccess() {
    lcd.setCursor(0, 0);
    lcd.print("           ");
    lcd.setCursor(0, 0);   
    lcd.print("Authorized!"); //clear any text previously displayed

    digitalWrite(4, HIGH);   //unclock door function
    digitalWrite(5, LOW);    
    digitalWrite(6, HIGH);   
    tone(2, 440);              

    delay(4000); // Wait for 4 seconds
    
    digitalWrite(4, LOW);   //lock door function
    digitalWrite(5, HIGH);  
    digitalWrite(6, LOW);   
    noTone(2);

    lcd.setCursor(0, 0);
    lcd.print("PIN:______  ");
    lcd.setCursor(0, 0);   //clears the text after door lock cycle
}

void readStoredCard(int slot, char* buffer) {
  int addr = 0;
  switch (slot) {
    case 1: addr = CARD1_ADDR; break;
    case 2: addr = CARD2_ADDR; break;
    case 3: addr = CARD3_ADDR; break;
    case 4: addr = CARD4_ADDR; break;
    default: return;
  }
  for (int i = 0; i < CARD_SIZE; i++) {
    buffer[i] = EEPROM.read(addr + i);
  }
  buffer[CARD_SIZE] = '\0';  // Null-terminate
}