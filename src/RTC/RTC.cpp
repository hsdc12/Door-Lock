#include <Arduino.h>
#include "RTC.h"
#include <Wire.h>
#include <RTClib.h>
#include <rgb_lcd.h>

enum { C = 262, G = 392, F = 349, A = 440 };

// The four Westminster chime phrases (indexes into bellNotes[])
const int bellNotes[4] = { C, G, F, A };
const uint8_t phrases[4][4] = {
  {2, 3, 1, 0},  // Phrase 1: G#4, F#4, E4, B3
  {2, 1, 3, 2},  // Phrase 2: E4, G#4, F#4, B3
  {3, 2, 1, 0},  // Phrase 3: G#4, E4, F#4, B3
  {0, 1, 3, 2}   // Phrase 4: F#4, E4, G#4, B3
};

// Timing parameters (milliseconds)
const unsigned long noteDuration   = 600;
const unsigned long notePause      = 150;
const unsigned long phrasePause    = 900;
const unsigned long strikeDuration = 1000;
const unsigned long strikePause    = 1500;
byte lastHour = 343; // Set to an impossible hour initially
bool firstRun = true; // Flag to indicate if it's the first run

RTC_DS3231 rtc;
extern rgb_lcd lcd; // Declare the rgb_lcd object

void TimeDateTempsetup() {
    rtc.begin();
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
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


  if (now.hour() != lastHour) {
    if (!firstRun) {
      BigBenChime();
    }
    firstRun  = false;
    lastHour  = now.hour();
  }
}



void BigBenChime() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("!    PLEASE    !");
  lcd.setCursor(0, 1); 
  lcd.print("!   STAND BY   !");


  for (uint8_t p = 0; p < 4; p++) {
    for (uint8_t n = 0; n < 4; n++) {
      uint8_t idx = phrases[p][n];
      tone(2, bellNotes[idx], noteDuration);
      delay(noteDuration);
      noTone(2);
      delay(notePause);
    }
    delay(phrasePause);
  }
  

if (lastHour < 13) {
  uint8_t strikeCount = lastHour;
  for (uint8_t i = 0; i <= strikeCount; i++) {
    tone(2, 440, strikeDuration);
    delay(strikeDuration);
    noTone(2);
    delay(strikePause);
  }
}

if (lastHour > 12) {
  uint8_t strikeCount = lastHour - 12; // Adjust for 12-hour format
  for (uint8_t i = 0; i <= strikeCount; i++) {
    tone(2, 440, strikeDuration);
    delay(strikeDuration);
    noTone(2);
    delay(strikePause);
  }
}

  noTone(2);
  lcd.clear();
}
