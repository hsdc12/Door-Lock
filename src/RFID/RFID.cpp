#include <Arduino.h>
#include "RFID.h"
#include <SPI.h>



#define SS_PIN 10
#define RST_PIN 9
MFRC522 myRFID(SS_PIN, RST_PIN);   // Create MFRC522 instance.

extern rgb_lcd lcd;

void initRFID() {
  noTone(2); // Stop any tone that might be playing
  SPI.begin();      // Initiate  SPI bus
  myRFID.PCD_Init();   // Initiate MFRC522
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
  //Show UID on serial monitor

  String content= "";
  for (byte i = 0; i < myRFID.uid.size; i++) {
     content.concat(String(myRFID.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(myRFID.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "76 FE 31 02") //change here the UID of the card/cards that you want to give access
  {
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
    lcd.print("           ");
    lcd.setCursor(0, 0);   //clears the text after door lock cycle
  }
 
 else   {
    lcd.setCursor(0, 0);
    lcd.print("           ");
    lcd.setCursor(0, 0);   
    lcd.print("Denied!");
    
    tone(2, 200, 600);

    lcd.setCursor(0, 0);
    lcd.print("           ");
    lcd.setCursor(0, 0);  //clear text
  }
}