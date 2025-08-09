#include <Arduino.h>
#include <Keypad.h>
#include "keypadHandler.h"
#include <rgb_lcd.h>
#include <EEPROM.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <MFRC522.h>

extern bool firstRun;
extern MFRC522 myRFID;

// Define the keymap (4x3 for standard keypad)
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Use analog pins A2, 3, 7, A0 for rows
byte rowPins[ROWS] = {A2, 3, 7, A0};
// Use analog pins A1, A3 and digital pin 8 for columns
byte colPins[COLS] = {A1, A3, 8};
// Create the Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


const byte CODE_LENGTH  = 6;
char PASSCODE[CODE_LENGTH + 1];
char inputBuffer[CODE_LENGTH + 1];  // extra byte for the string terminator
byte inputPos = 0;
const char ADMIN_PASSCODE[CODE_LENGTH + 1] = "654321"; // Default passcode

char timeInput[6];  // HHMM + null + spare
int inputIndex = 0;
bool timeReady = false;

char dateInput[8];  // DDMMYY + null + spare
int secondinputIndex = 0;
bool dateReady = false;

#define PASSWORD_ADDR 0     // Start of password in EEPROM
#define CARD1_ADDR      (PASSWORD_ADDR + PASSWORD_SIZE)       // 16
#define CARD2_ADDR      (CARD1_ADDR + 12)                     // 28
#define CARD3_ADDR      (CARD2_ADDR + 12)                     // 40
#define CARD4_ADDR      (CARD3_ADDR + 12)                     // 52

#define CARD_SIZE       12
#define PASSWORD_SIZE   16


extern rgb_lcd lcd;
extern RTC_DS3231 rtc;


void initKeypad() {
  for (byte i = 0; i < CODE_LENGTH; i++) {
    PASSCODE[i] = char( EEPROM.read(PASSWORD_ADDR + i));  // Initialize the passcode with zeros
  }
  PASSCODE[CODE_LENGTH] = '\0';  // Null-terminate the string

  bool valid = true;
  for (byte i = 0; i < CODE_LENGTH; i++) {
    if (PASSCODE[i] < '0' || PASSCODE[i] > '9') {
      valid = false;  // Ensure all characters are digits
      break;
    }
  
  if (!valid){
    strcpy(PASSCODE, "123456");  // Default passcode if EEPROM is invalid
    for (byte i = 0; i < CODE_LENGTH; i++) {
      EEPROM.update(PASSWORD_ADDR + i, PASSCODE[i]);  // Save the default passcode to EEPROM
    }
  }
}
}

char getKeyPressed() {
  return keypad.getKey();

}


void checkCode() {
  if(strcmp(inputBuffer, ADMIN_PASSCODE) == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);   
    lcd.print(">> Admin Mode <<");  
    tone(2, 440, 200);
    delay(200); // Wait for 0.3 seconds
    tone(2, 660, 700);
    delay(2200); // Wait for 2.5 seconds
    enterAdminMode();
    return;
  }

  if (strcmp(inputBuffer, PASSCODE) == 0) {
    lcd.setCursor(0, 0);
    lcd.print("           ");
    lcd.setCursor(0, 0);   
    lcd.print("Authorized!");

    digitalWrite(4, HIGH);   //unlock door()
    digitalWrite(5, LOW);    
    digitalWrite(6, HIGH);   
    tone(2, 440);

    delay(4000);
   
    digitalWrite(4, LOW);   //lock door()
    digitalWrite(5, HIGH);  
    digitalWrite(6, LOW);   
    noTone(2);  

  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("           ");
    lcd.setCursor(0, 0);   
    lcd.print("Denied!");
    tone(2, 200, 600);
    delay(2000);
  }
}

void keypadloop() {
  char k = getKeyPressed();
  if (!k) return;                   // no key pressed

  tone(2, 586, 100); // Short beep for key press

  if (k == '*') {                   // Reset input
    inputPos = 0;
    lcd.clear();
    return;
  }
  if (k == '#') {
    inputBuffer[inputPos] = '\0';   // terminate the C-string
    checkCode();
    inputPos = 0;
    lcd.clear();
    return;
  }
   if (inputPos < CODE_LENGTH && isDigit(k)) {
    inputBuffer[inputPos++] = k;
    lcd.setCursor(inputPos - 1, 0);
    lcd.print('*');
  }
}


void enterAdminMode() {
  adminMenu();
  while (true) {
    char k = getKeyPressed();
    if (k) {
      tone(2, 586, 100);
    }
    if (k == '1') {
      adminPassword();
      return;
    } else if (k == '2') {
      adminCard();
      return;
    } else if (k == '3') {
      adminExit();
      break;
    } else if (k == '4') {
      adminReset();
      return;
    } else if (k == '5') {
      adminTime();
      return;
    } else if (k == '6') {
      adminDate();
      return;
    }
  }
}

void adminMenu() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.write(byte(1));  
  lcd.print("PIN ");
  lcd.write(byte(2));  
  lcd.print("CARD ");
  lcd.write(byte(3));  
  lcd.print("EXIT");

  lcd.setCursor(0, 1);
  lcd.write(byte(4));  
  lcd.print("RST ");
  lcd.write(byte(5));  
  lcd.print("TIME ");
  lcd.write(byte(6));  
  lcd.print("DATE");
}

void adminPassword() {
  char newPin[CODE_LENGTH + 1];
  byte pos = 0;
  lcd.clear();
  lcd.print("New PIN:");

  // block until 6 digits are collected
  while (pos < CODE_LENGTH) {
    char k = getKeyPressed();
    if (k) {
      tone(2, 586, 100); // Short beep for key press
    }
    if (k == '*') {                   // Reset input
      pos = 0;
      lcd.setCursor(0, 0);
      lcd.print("New PIN:      ");
      lcd.setCursor(7, 0);
    }
    if (k && isDigit(k)) {
      newPin[pos++] = k;
      lcd.setCursor(pos + 7, 0);
      lcd.print('*');
  }
}
 while (pos == CODE_LENGTH) {
     char k = getKeyPressed();
      if (k) {
        tone(2, 586, 100); // Short beep for key press
      }
      if (k == '*') {                   // Reset input
        enterAdminMode(); // Restart the admin mode input
        return;
      }
      if (k == '#') {
        newPin[CODE_LENGTH] = '\0';
        break;  // Exit the loop when the user confirms the new PIN
      }
 }
  strcpy(PASSCODE, newPin);
  for (byte i = 0; i < CODE_LENGTH; i++) {
    EEPROM.update(PASSWORD_ADDR + i, PASSCODE[i]);
  }

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("PIN Updated!");
  tone(2, 660, 200);
  delay(200); // Wait for 0.3 seconds
  tone(2, 880, 700);
  delay(2200); // Wait for 2.5 seconds
  enterAdminMode();  //return to admin mode
}



void adminExit() {
    lcd.clear();
    lcd.setCursor(0, 0);   
    lcd.print("+Exiting Admin +");  
    lcd.setCursor(0, 1);   
    lcd.print("+     Mode     +");  
    tone(2, 440, 200);
    delay(200); // Wait for 0.3 seconds
    tone(2, 660, 700);
    delay(2200); // Wait for 2.5 seconds
    return;  // Exit admin mode
}


void adminReset() {
    lcd.clear();
    lcd.setCursor(0, 0);   
    lcd.print("+ Device Reset +");  
    lcd.setCursor(0, 1);   
    lcd.print("+  Completed!  +");  
    tone(2, 440, 200);
    delay(200); // Wait for 0.3 seconds
    tone(2, 660, 700);
    delay(2200); // Wait for 2.5 seconds
    for (int i = CARD1_ADDR; i < CARD4_ADDR + CARD_SIZE; i++) {
      EEPROM.update(i, 0);
    }
    rtc.adjust(DateTime(0000, 0, 0, 0, 0,0 ));

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("CHINGDOM INC");
    delay(2500);
    lcd.clear();

    adminResetInit();
    return;  // Exit admin mode
}

void adminTime() {
  lcd.clear();
  inputIndex = 0;
  timeReady = false;
  memset(timeInput, 0, sizeof(timeInput));   //reset time input
  int X = 1; 

  int newHour = 0, newMinute = 0;

  while (X == 1) {
    char k   = getKeyPressed();
    DateTime now = rtc.now();

    char timeBuf[6];
    sprintf(timeBuf, "%02d:%02d", now.hour(), now.minute());

    //    start as placeholder "--:--"
    char changedBuf[6] = "__:__";
    if (!timeReady) {
      // fill in any typed digits
      for (int i = 0; i < inputIndex && i < 4; i++) {
        // place first two digits at [0],[1], next two at [3],[4]
        if (i < 2)         changedBuf[i]     = timeInput[i];
        else               changedBuf[i + 1] = timeInput[i];
      }
    } else {
      // after confirm, show the newly set time
      sprintf(changedBuf, "%02d:%02d", newHour, newMinute);
    }

    lcd.setCursor(0, 0);
    lcd.print("Current:  "); 
    lcd.print(timeBuf);

    lcd.setCursor(0, 1);
    lcd.print("Changed:  ");
    lcd.print(changedBuf);

    // 4) Beep on any keypress
    if (k) {
      tone(2, 586, 100);
    }

    // 5) Handle keypad
    if (!timeReady && k) {
      // digit keys
      if (k >= '0' && k <= '9' && inputIndex < 4) {
        timeInput[inputIndex++] = k;
      }
      // confirm entry
      else if (k == '#') {
        if (inputIndex == 4) {
          // parse HHMM
          newHour   = (timeInput[0] - '0') * 10 + (timeInput[1] - '0');
          newMinute = (timeInput[2] - '0') * 10 + (timeInput[3] - '0');

          // apply to RTC (keep today's date)
          rtc.adjust(DateTime(now.year(),
                              now.month(),
                              now.day(),
                              newHour,
                              newMinute,
                              0));
          timeReady = true;

          lcd.clear();
          lcd.setCursor(0, 0);   
          lcd.print("+ Time Changed +");  
          lcd.setCursor(0, 1);   
          lcd.print("+              +");
          tone(2, 660, 200);
          delay(200);
          tone(2, 880, 700);
          delay(2200); 
          firstRun  = true;
          X = 0; 
        }
      }

      else if (k == '*') {
        inputIndex = 0;
        memset(timeInput, 0, sizeof(timeInput));
      }
    }
  }

  enterAdminMode();  // Return to admin mode
}

void adminDate() {
  lcd.clear();
  secondinputIndex = 0;
  dateReady = false;
  memset(dateInput, 0, sizeof(dateInput));   //reset time input
  int Y = 1; 
  DateTime now = rtc.now();

  int newYear = 0, newMonth = 0; int newDay = 0;

  while (Y == 1) {
    char k   = getKeyPressed();

    char dateBuf[9];   // "DD/MM/YY" + null
    sprintf(dateBuf, "%02d/%02d/%02d",
          now.day(),
          now.month(),
          now.year() % 100);

    //    start as placeholder "--:--"
    char changedBuf[9] = "__/__/__";
    if (!dateReady) {
      // fill in any typed digits
      for (int i = 0; i < secondinputIndex && i < 6; i++) {
        // place first two digits at [0],[1], next two at [3],[4]
        if      (i < 2)            changedBuf[i]     = dateInput[i];
        else if (i >= 2 && i < 4)  changedBuf[i + 1] = dateInput[i];
        else                       changedBuf[i + 2] = dateInput[i];
      }
    } else {
      // after confirm, show the newly set time
      sprintf(changedBuf, "%02d/%02d/%02d", newDay, newMonth, newYear);
    }

    lcd.setCursor(0, 0);
    lcd.print("Current:"); 
    lcd.print(dateBuf);

    lcd.setCursor(0, 1);
    lcd.print("Changed:");
    lcd.print(changedBuf);

    // 4) Beep on any keypress
    if (k) {
      tone(2, 586, 100);
    }

    // 5) Handle keypad
    if (!dateReady && k) {
      // digit keys
      if (k >= '0' && k <= '9' && secondinputIndex < 6) {
        dateInput[secondinputIndex++] = k;
      }
      // confirm entry
      else if (k == '#') {
        if (secondinputIndex == 6) {

          newDay   = (dateInput[0] - '0') * 10 + (dateInput[1] - '0');
          newMonth = (dateInput[2] - '0') * 10 + (dateInput[3] - '0');
          newYear = (dateInput[4] - '0') * 10 + (dateInput[5] - '0');

          // apply to RTC (keep today's date)
          rtc.adjust(DateTime(newYear + 2000,
                              newMonth,
                              newDay,
                              now.hour(),
                              now.minute(),
                              now.second()));
          dateReady = true;

          lcd.clear();
          lcd.setCursor(0, 0);   
          lcd.print("+ Date Changed +");  
          lcd.setCursor(0, 1);   
          lcd.print("+              +");
          tone(2, 660, 200);
          delay(200);
          tone(2, 880, 700);
          delay(2200); 
          
          Y = 0; 
        }
      }

      else if (k == '*') {
        secondinputIndex = 0;
        memset(dateInput, 0, sizeof(dateInput));
      }
    }
  }

  enterAdminMode();  // Return to admin mode
 }


 void CardMenu() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Cards: ");
  lcd.write(byte(1));  
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.write(byte(3));
  lcd.print(" ");
  lcd.write(byte(4));  

  lcd.setCursor(1, 1);
  lcd.write(byte(5));  
  lcd.print("Reset   ");
  lcd.write(byte(6));  
  lcd.print("Exit");

}

void adminCard() {
  CardMenu();
  while (true) {
    char k = getKeyPressed();
    if (k) {
      tone(2, 586, 100);
    }
    if (k == '1') {
      IDCard1();
      return;
    } else if (k == '2') {
      IDCard2();
      return;
    } else if (k == '3') {
      IDCard3();
      return;
    } else if (k == '4') {
      IDCard4();
      return;
    } else if (k == '5') {
      IDReset();
      return;
    } else if (k == '6') {
      enterAdminMode();
      return;  // Exit card menu
    }
  }
}

void IDCard1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please Scan Card");
  lcd.setCursor(0, 1);
  lcd.print("   to Update:   ");


  while (!myRFID.PICC_IsNewCardPresent() || !myRFID.PICC_ReadCardSerial()) {
    // wait here until card is scanned
  }
  char uidString[CARD_SIZE + 1];
  int len = myRFID.uid.size; // UID length in bytes
  for (int i = 0; i < len && i < CARD_SIZE / 2; i++) {
    sprintf(&uidString[i*2], "%02X", myRFID.uid.uidByte[i]);
  }
  uidString[len*2] = '\0';  // null terminate

  // Save to EEPROM
  int addr = CARD1_ADDR;
  for (int i = 0; i < CARD_SIZE; i++) {
    char c = (i < len*2) ? uidString[i] : '\0';
    EEPROM.update(addr + i, c);
}
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("+     Card     +");
  lcd.setCursor(0, 1);
  lcd.print("+    Saved!    +");
  tone(2, 660, 200);
  delay(200);
  tone(2, 880, 700);
  delay(2200);
  adminCard();  // Return to card menu
}

void IDCard2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please Scan Card");
  lcd.setCursor(0, 1);
  lcd.print("   to Update:   ");


  while (!myRFID.PICC_IsNewCardPresent() || !myRFID.PICC_ReadCardSerial()) {
    // wait here until card is scanned
  }
  char uidString[CARD_SIZE + 1];
  int len = myRFID.uid.size; // UID length in bytes
  for (int i = 0; i < len && i < CARD_SIZE / 2; i++) {
    sprintf(&uidString[i*2], "%02X", myRFID.uid.uidByte[i]);
  }
  uidString[len*2] = '\0';  // null terminate

  // Save to EEPROM
  int addr = CARD2_ADDR;
  for (int i = 0; i < CARD_SIZE; i++) {
    char c = (i < len*2) ? uidString[i] : '\0';
    EEPROM.update(addr + i, c);
}
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("+     Card     +");
  lcd.setCursor(0, 1);
  lcd.print("+    Saved!    +");
  tone(2, 660, 200);
  delay(200);
  tone(2, 880, 700);
  delay(2200);
  adminCard();  // Return to card menu
}

void IDCard3() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please Scan Card");
  lcd.setCursor(0, 1);
  lcd.print("   to Update:   ");


  while (!myRFID.PICC_IsNewCardPresent() || !myRFID.PICC_ReadCardSerial()) {
    // wait here until card is scanned
  }
  char uidString[CARD_SIZE + 1];
  int len = myRFID.uid.size; // UID length in bytes
  for (int i = 0; i < len && i < CARD_SIZE / 2; i++) {
    sprintf(&uidString[i*2], "%02X", myRFID.uid.uidByte[i]);
  }
  uidString[len*2] = '\0';  // null terminate

  // Save to EEPROM
  int addr = CARD3_ADDR;
  for (int i = 0; i < CARD_SIZE; i++) {
    char c = (i < len*2) ? uidString[i] : '\0';
    EEPROM.update(addr + i, c);
}
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("+     Card     +");
  lcd.setCursor(0, 1);
  lcd.print("+    Saved!    +");
  tone(2, 660, 200);
  delay(200);
  tone(2, 880, 700);
  delay(2200);
  adminCard();  // Return to card menu
}

void IDCard4() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please Scan Card");
  lcd.setCursor(0, 1);
  lcd.print("   to Update:   ");


  while (!myRFID.PICC_IsNewCardPresent() || !myRFID.PICC_ReadCardSerial()) {
    // wait here until card is scanned
  }
  char uidString[CARD_SIZE + 1];
  int len = myRFID.uid.size; // UID length in bytes
  for (int i = 0; i < len && i < CARD_SIZE / 2; i++) {
    sprintf(&uidString[i*2], "%02X", myRFID.uid.uidByte[i]);
  }
  uidString[len*2] = '\0';  // null terminate

  // Save to EEPROM
  int addr = CARD4_ADDR;
  for (int i = 0; i < CARD_SIZE; i++) {
    char c = (i < len*2) ? uidString[i] : '\0';
    EEPROM.update(addr + i, c);
}
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("+     Card     +");
  lcd.setCursor(0, 1);
  lcd.print("+    Saved!    +");
  tone(2, 660, 200);
  delay(200);
  tone(2, 880, 700);
  delay(2200);
  adminCard();  // Return to card menu
}

void IDReset() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Deleting All   ");
  lcd.setCursor(0, 1);
  lcd.print("  Stored Cards   ");
  delay(500); // Wait for 0.5 seconds

  // Reset all card slots in EEPROM
  for (int i = CARD1_ADDR; i < CARD4_ADDR + CARD_SIZE; i++) {
    EEPROM.update(i, 0);
  }
  tone(2, 660, 200);
  delay(200);
  tone(2, 880, 700);
  delay(2200);
  adminCard();  // Return to card menu
}


void adminResetInit() {
    char newPin[CODE_LENGTH + 1];
  byte pos = 0;
  lcd.clear();
  lcd.print("New PIN:");

  // block until 6 digits are collected
  while (pos < CODE_LENGTH) {
    char k = getKeyPressed();
    if (k) {
      tone(2, 586, 100); // Short beep for key press
    }
    if (k == '*') {                   // Reset input
      pos = 0;
      lcd.setCursor(0, 0);
      lcd.print("New PIN:      ");
      lcd.setCursor(7, 0);
    }
    if (k && isDigit(k)) {
      newPin[pos++] = k;
      lcd.setCursor(pos + 7, 0);
      lcd.print('*');
  }
}
 while (pos == CODE_LENGTH) {
     char k = getKeyPressed();
      if (k) {
        tone(2, 586, 100); // Short beep for key press
      }
      if (k == '*') {                   // Reset input
        enterAdminMode(); // Restart the admin mode input
        return;
      }
      if (k == '#') {
        newPin[CODE_LENGTH] = '\0';
        break;  // Exit the loop when the user confirms the new PIN
      }
 }
  strcpy(PASSCODE, newPin);
  for (byte i = 0; i < CODE_LENGTH; i++) {
    EEPROM.update(PASSWORD_ADDR + i, PASSCODE[i]);
  }

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("PIN Updated!");
  delay(1000);
//
//
  lcd.clear();
  secondinputIndex = 0;
  dateReady = false;
  memset(dateInput, 0, sizeof(dateInput));   //reset time input
  int Y = 1; 
  DateTime now = rtc.now();

  int newYear = 0, newMonth = 0; int newDay = 0;

  while (Y == 1) {
    char k   = getKeyPressed();

    char dateBuf[9];   // "DD/MM/YY" + null
    sprintf(dateBuf, "%02d/%02d/%02d",
          now.day(),
          now.month(),
          now.year() % 100);

    //    start as placeholder "--:--"
    char changedBuf[9] = "__/__/__";
    if (!dateReady) {
      // fill in any typed digits
      for (int i = 0; i < secondinputIndex && i < 6; i++) {
        // place first two digits at [0],[1], next two at [3],[4]
        if      (i < 2)            changedBuf[i]     = dateInput[i];
        else if (i >= 2 && i < 4)  changedBuf[i + 1] = dateInput[i];
        else                       changedBuf[i + 2] = dateInput[i];
      }
    } else {
      // after confirm, show the newly set time
      sprintf(changedBuf, "%02d/%02d/%02d", newDay, newMonth, newYear);
    }

    lcd.setCursor(0, 0);
    lcd.print("Current:"); 
    lcd.print(dateBuf);

    lcd.setCursor(0, 1);
    lcd.print("Changed:");
    lcd.print(changedBuf);

    // 4) Beep on any keypress
    if (k) {
      tone(2, 586, 100);
    }

    // 5) Handle keypad
    if (!dateReady && k) {
      // digit keys
      if (k >= '0' && k <= '9' && secondinputIndex < 6) {
        dateInput[secondinputIndex++] = k;
      }
      // confirm entry
      else if (k == '#') {
        if (secondinputIndex == 6) {

          newDay   = (dateInput[0] - '0') * 10 + (dateInput[1] - '0');
          newMonth = (dateInput[2] - '0') * 10 + (dateInput[3] - '0');
          newYear = (dateInput[4] - '0') * 10 + (dateInput[5] - '0');

          // apply to RTC (keep today's date)
          rtc.adjust(DateTime(newYear + 2000,
                              newMonth,
                              newDay,
                              now.hour(),
                              now.minute(),
                              now.second()));
          dateReady = true;

          lcd.clear();
          lcd.setCursor(0, 0);   
          lcd.print("+ Date Changed +");  
          lcd.setCursor(0, 1);   
          lcd.print("+              +");
          delay(1000); 
          
          Y = 0; 
        }
      }

      else if (k == '*') {
        secondinputIndex = 0;
        memset(dateInput, 0, sizeof(dateInput));
      }
    }
  }
//
//
  lcd.clear();
  inputIndex = 0;
  timeReady = false;
  memset(timeInput, 0, sizeof(timeInput));   //reset time input
  int X = 1; 

  int newHour = 0, newMinute = 0;

  while (X == 1) {
    char k   = getKeyPressed();
    DateTime now = rtc.now();

    char timeBuf[6];
    sprintf(timeBuf, "%02d:%02d", now.hour(), now.minute());

    //    start as placeholder "--:--"
    char changedBuf[6] = "__:__";
    if (!timeReady) {
      // fill in any typed digits
      for (int i = 0; i < inputIndex && i < 4; i++) {
        // place first two digits at [0],[1], next two at [3],[4]
        if (i < 2)         changedBuf[i]     = timeInput[i];
        else               changedBuf[i + 1] = timeInput[i];
      }
    } else {
      // after confirm, show the newly set time
      sprintf(changedBuf, "%02d:%02d", newHour, newMinute);
    }

    lcd.setCursor(0, 0);
    lcd.print("Current:  "); 
    lcd.print(timeBuf);

    lcd.setCursor(0, 1);
    lcd.print("Changed:  ");
    lcd.print(changedBuf);

    // 4) Beep on any keypress
    if (k) {
      tone(2, 586, 100);
    }

    // 5) Handle keypad
    if (!timeReady && k) {
      // digit keys
      if (k >= '0' && k <= '9' && inputIndex < 4) {
        timeInput[inputIndex++] = k;
      }
      // confirm entry
      else if (k == '#') {
        if (inputIndex == 4) {
          // parse HHMM
          newHour   = (timeInput[0] - '0') * 10 + (timeInput[1] - '0');
          newMinute = (timeInput[2] - '0') * 10 + (timeInput[3] - '0');

          // apply to RTC (keep today's date)
          rtc.adjust(DateTime(now.year(),
                              now.month(),
                              now.day(),
                              newHour,
                              newMinute,
                              now.second()));
          timeReady = true;

          lcd.clear();
          lcd.setCursor(0, 0);   
          lcd.print("+ Time Changed +");  
          lcd.setCursor(0, 1);   
          lcd.print("+              +");
          delay(1000); 
          firstRun = true;
          X = 0; 
        }
      }

      else if (k == '*') {
        inputIndex = 0;
        memset(timeInput, 0, sizeof(timeInput));
      }
    }
  }
}