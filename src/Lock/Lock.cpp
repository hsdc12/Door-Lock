#include <Arduino.h>
#include "Lock.h"

void unlockdoor() {
    digitalWrite(4, HIGH);   //solenoid on (opens)
    digitalWrite(5, LOW);    //red LED off
    digitalWrite(6, HIGH);   //green LED on
    tone(2, 440);            //buzzer on     
}

void lockdoor() {
    digitalWrite(4, LOW);   //solenoid off (closes)
    digitalWrite(5, HIGH);  //red LED on
    digitalWrite(6, LOW);   //green LED off
    noTone(2);              //buzzer off
}

void setupLock() {
    pinMode(2, OUTPUT);  // Piezo
    pinMode(4, OUTPUT);  // Solenoid
    pinMode(5, OUTPUT);  // green LED
    pinMode(6, OUTPUT);  // red LED

    noTone(2); // Stop any tone that might be playing
    lockdoor();          // Start with the door locked
}
