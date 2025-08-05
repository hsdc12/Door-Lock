#include <Arduino.h>
#include <Keypad.h>
#include "keypadHandler.h"
#include <rgb_lcd.h>
#include <EEPROM.h>

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


extern rgb_lcd lcd;


void initKeypad() {
  for (byte i = 0; i < CODE_LENGTH; i++) {
    PASSCODE[i] = char( EEPROM.read(i));  // Initialize the passcode with zeros
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
      EEPROM.update(i, PASSCODE[i]);  // Save the default passcode to EEPROM
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
    lcd.setCursor(3, 0);   
    lcd.print("Admin Mode");
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





  // commit it
  strcpy(PASSCODE, newPin);
  for (byte i = 0; i < CODE_LENGTH; i++) {
    EEPROM.update(i, PASSCODE[i]);
  }

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("PIN Updated!");
  tone(2, 660, 200);
  delay(200); // Wait for 0.3 seconds
  tone(2, 880, 700);
  delay(2200); // Wait for 2.5 seconds
}
