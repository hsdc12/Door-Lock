#include <Arduino.h>
#include <rgb_lcd.h>

extern rgb_lcd lcd;

byte oneDotChar[8] = {
  0b00000,
  0b00100,
  0b01100,
  0b00100,
  0b00100,
  0b01110,
  0b00000,
  0b00001  // dot at column 4 only
};

byte twoDotChar[8] = {
  0b00000,
  0b11110,
  0b00010,
  0b11110,
  0b10000,
  0b11110,
  0b00000,
  0b00001
};

byte threeDotChar[8] = {
  0b00000,
  0b11110,
  0b00010,
  0b11110,
  0b00010,
  0b11110,
  0b00000,
  0b00001
};

byte fourDotChar[8] = {
  0b00000,
  0b10010,
  0b10010,
  0b11110,
  0b00010,
  0b00010,
  0b00000,
  0b00001
};

byte fiveDotChar[8] = {
  0b00000,
  0b11110,
  0b10000,
  0b11110,
  0b00010,
  0b11110,
  0b00000,
  0b00001
};

byte sixDotChar[8] = {
  0b00000,
  0b11110,
  0b10000,
  0b11110,
  0b10010,
  0b11110,
  0b00000,
  0b00001
};

void LCDsetup() {
    lcd.begin(16, 2);
    lcd.setRGB(0, 128, 255);
    lcd.createChar(1, oneDotChar);
    lcd.createChar(2, twoDotChar);
    lcd.createChar(3, threeDotChar);
    lcd.createChar(4, fourDotChar);
    lcd.createChar(5, fiveDotChar);
    lcd.createChar(6, sixDotChar); 
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("CHINGDOM INC");
    delay(2500);
    lcd.clear();
}


//    lcd.print("1PIN 2Card 3EXIT");
//    lcd.print("4RST 5time 6Date");