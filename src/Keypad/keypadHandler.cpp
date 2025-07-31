#include <Arduino.h>
#include <Keypad.h>
#include "keypadHandler.h"
#include <rgb_lcd.h>

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

const char PASSCODE[] = "123456";
const byte CODE_LENGTH  = 6;

extern rgb_lcd lcd;
char inputBuffer[CODE_LENGTH + 1];  // extra byte for the string terminator
byte inputPos = 0;


void initKeypad() {
  // No initialization needed for the library,
  // but we keep this here in case we add custom behavior
}

char getKeyPressed() {
  return keypad.getKey();
}


void checkCode() {
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