#include "main.h"




rgb_lcd lcd;

void setup() {
  initRFID(); // Initialize the RFID reader
  TimeDateTempsetup();
  initKeypad();
  delay(50);
  setupLock();  // Initialize the lock system
  LCDsetup(); // Initialize the LCD
}


void loop() 
{
  TimeDateTempvoid();
  handleRFID(); // Handle RFID scanning and processing
  keypadloop();
  delay(20); // Small delay to avoid overwhelming the loop
}



