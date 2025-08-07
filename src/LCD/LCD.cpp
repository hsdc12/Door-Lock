#include <Arduino.h>
#include <rgb_lcd.h>

extern rgb_lcd lcd;

byte oneDotChar[8] = {
  B00100,
  B01100,
  B00100,
  B00100,
  B00100,
  B00000,
  B00100,
  B00000
};

byte twoDotChar[8] = {
  B01110,
  B00001,
  B00010,
  B00100,
  B01000,
  B00000,
  B00100,
  B00000
};

byte threeDotChar[8] = {
  B01110,
  B00001,
  B00110,
  B00001,
  B01110,
  B00000,
  B00100,
  B00000
};

byte fourDotChar[8] = {
  B00010,
  B00110,
  B01010,
  B10010,
  B11111,
  B00000,
  B00100,
  B00000
};

byte fiveDotChar[8] = {
  B11111,
  B10000,
  B11110,
  B00001,
  B11110,
  B00000,
  B00100,
  B00000
};

void LCDsetup() {
    lcd.begin(16, 2);
    lcd.setRGB(0, 128, 255);
    lcd.createChar(1, oneDotChar);
    lcd.createChar(2, twoDotChar);
    lcd.createChar(3, threeDotChar);
    lcd.createChar(4, fourDotChar);
    lcd.createChar(5, fiveDotChar); 
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("CHINGDOM INC");
    delay(2500);
    lcd.clear();
}


//    lcd.print("1PIN 2Card 3EXIT");
//    lcd.print("4RST 5time 6Date");