#include <Arduino.h>
#include "RTC.h"
#include <Wire.h>
#include <RTClib.h>
#include <rgb_lcd.h>

RTC_DS3231 rtc;

extern rgb_lcd lcd; // Declare the rgb_lcd object

void TimeDateTempsetup() {
    rtc.begin();
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    lcd.begin(16, 2);
    lcd.setRGB(0, 128, 255); 
    lcd.print("CHINGDOM INC");
    delay(5000);
    lcd.clear();
}

void TimeDateTempvoid() {
    float   tempC  = rtc.getTemperature();
    lcd.setCursor(12, 0);
    lcd.print("    "); // Clear previous temperature
    lcd.setCursor(12, 0);
    lcd.print(tempC, 0); // Print temperature in Celsius
    lcd.write(0xDF); // Degree symbol
    lcd.print("C");
    //
    //
    //
  DateTime now = rtc.now();

  char dateBuf[9];   // "DD/MM/YY" + null
  sprintf(dateBuf, "%02d/%02d/%02d",
          now.day(),
          now.month(),
          now.year() % 100);

  char timeBuf[6];   // "HH:MM" + null
  sprintf(timeBuf, "%02d:%02d",
          now.hour(),
          now.minute());

  lcd.setCursor(0, 1);
  lcd.print("                ");  // 16 spaces

  lcd.setCursor(0, 1);           //prints date
  lcd.print(dateBuf);

  lcd.setCursor(11, 1);          // Print time at bottom right (col 11)
  lcd.print(timeBuf);
}