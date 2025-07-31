#include "main.h"  // Or: #include "src/Lock/Lock.h" if you want to skip main.h
#include <rgb_lcd.h>


rgb_lcd lcd;

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
  keypadloop();

  delay(100);
}

