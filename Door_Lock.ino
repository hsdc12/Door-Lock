#include "main.h"




rgb_lcd lcd;

void setup() {
  LCDsetup(); // Initialize the LCD
  initRFID(); // Initialize the RFID reader
  TimeDateTempsetup();
  initKeypad();
  setupLock();  // Initialize the lock system
}


void loop() 
{
  TimeDateTempvoid();
  handleRFID(); // Handle RFID scanning and processing
  keypadloop();
  delay(50); // Small delay to avoid overwhelming the loop
}



