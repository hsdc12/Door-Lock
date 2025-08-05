#include "main.h"




rgb_lcd lcd;

void setup() {
  initRFID(); // Initialize the RFID reader
  TimeDateTempsetup();
  initKeypad();
  setupLock();  // Initialize the lock system
  LCDsetup(); // Initialize the LCD
}


void loop() 
{
  TimeDateTempvoid();
  handleRFID(); // Handle RFID scanning and processing
  keypadloop();
  delay(25); // Small delay to avoid overwhelming the loop
}



