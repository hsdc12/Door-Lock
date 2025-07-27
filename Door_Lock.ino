#include "src/Lock/Lock.h"

void setup() {
  pinMode(2, OUTPUT);  // Piezo
  pinMode(4, OUTPUT);  // Solenoid
  pinMode(5, OUTPUT);  // green LED
  pinMode(6, OUTPUT);  // red LED

  lockdoor();          // Start with the door locked
}

void loop() {
  // Example: Unlock the door for 4 seconds every 10 seconds
  unlockdoor();
  delay(4000);         // Door unlocked for 4 seconds
  lockdoor();
  delay(6000);         // Door locked for 6 seconds
}