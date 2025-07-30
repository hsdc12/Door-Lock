#include "main.h"
#include <SPI.h>
#include <MFRC522.h>
#include <rgb_lcd.h>



rgb_lcd lcd;

void setup() {
  LCDsetup(); // Initialize the LCD
  initRFID(); // Initialize the RFID reader
  TimeDateTempsetup();
  setupLock();  // Initialize the lock system
}


void loop() 
{
  TimeDateTempvoid();
  handleRFID(); // Handle RFID scanning and processing
}
