#include <Arduino.h>
#include <rgb_lcd.h>

extern rgb_lcd lcd; // Declare the rgb_lcd object

void LCDsetup() {
    lcd.begin(16, 2);
    lcd.setRGB(0, 128, 255); 
    lcd.clear();
    lcd.print("CHINGDOM INC");
    delay(2500);
    lcd.clear();
}