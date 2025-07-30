#include "main.h"
#include <SPI.h>
#include <MFRC522.h>
#include <rgb_lcd.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 myRFID(SS_PIN, RST_PIN);   // Create MFRC522 instance.

extern rgb_lcd lcd;

void setup() {

  SPI.begin();      // Initiate  SPI bus
  myRFID.PCD_Init();   // Initiate MFRC522

  lcd.begin(16, 2);
  lcd.setRGB(0, 128, 255); 
  lcd.clear();
  TimeDateTempsetup();
  setupLock();  // Initialize the lock system
}


void loop() 
{
  TimeDateTempvoid();


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

    unlockdoor();
    delay(4000); // Wait for 4 seconds
    lockdoor();

    lcd.setCursor(0, 0);
    lcd.print("           ");
    lcd.setCursor(0, 0);   //clears the text after door lock cycle
  }
 
 else   {
    lcd.setCursor(0, 0);
    lcd.print("           ");
    lcd.setCursor(0, 0);   
    lcd.print("Denied!");
    
    tone(2, 200); // Beep start
    delay(3000); // Wait for 3 seconds
    noTone(2); // Stop beeping

    lcd.setCursor(0, 0);
    lcd.print("           ");
    lcd.setCursor(0, 0);  //clear text
  }

}
