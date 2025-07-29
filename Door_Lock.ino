#include "main.h"  // Or: #include "src/Lock/Lock.h" if you want to skip main.h

void setup() {
  setupLock();  // Initialize the lock system
}

void loop() {
  // Example: Unlock the door for 4 seconds every 10 seconds
  unlockdoor();
  delay(4000);         // Door unlocked for 4 seconds
  lockdoor();
  delay(6000);         // Door locked for 6 seconds
}