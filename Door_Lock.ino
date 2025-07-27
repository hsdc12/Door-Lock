#include "main.h"  // Or: #include "src/Lock/Lock.h" if you want to skip main.h
#include <rgb_lcd.h>


const char PASSCODE[] = "123456";
const byte CODE_LENGTH  = 6;

rgb_lcd lcd;
char inputBuffer[CODE_LENGTH + 1];  // extra byte for the string terminator
byte inputPos = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.setRGB(0, 128, 255);
  lcd.clear();

  pinMode(2, OUTPUT);  // Piezo
  pinMode(4, OUTPUT);  // Solenoid
  pinMode(5, OUTPUT);  // green LED
  pinMode(6, OUTPUT);  // red LED

  lockdoor();          // Start with the door locked

  initKeypad();
}

void loop() {
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

void checkCode() {
  if (strcmp(inputBuffer, PASSCODE) == 0) {
    lcd.setCursor(0, 1);
    lcd.print("Access Granted");
    unlockdoor();
    delay(4000);
    lockdoor();
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("Access Denied ");
    tone(2, 200, 600);
    delay(2000);
  }
}