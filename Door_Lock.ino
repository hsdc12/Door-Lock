#include "main.h"  // Or: #include "src/Lock/Lock.h" if you want to skip main.h

void setup() {
  pinMode(2, OUTPUT);  // Piezo
  pinMode(4, OUTPUT);  // Solenoid
  pinMode(5, OUTPUT);  // green LED
  pinMode(6, OUTPUT);  // red LED

  lockdoor();          // Start with the door locked


  Serial.begin(9600);
  initKeypad();
}

void loop() {
  // Example: Unlock the door for 4 seconds every 10 seconds



  char key = getKeyPressed();
  if (key) {
    Serial.println(key);
  }
}