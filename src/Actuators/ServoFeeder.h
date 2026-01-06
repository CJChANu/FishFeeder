#ifndef SERVO_FEEDER_H
#define SERVO_FEEDER_H

#include <ESP32Servo.h>

#define SERVO_PIN 13
Servo feederServo;

class IntelligentFeeder {
public:
  void init() {
    feederServo.attach(SERVO_PIN, 500, 2500);
    feederServo.write(0);  // Closed
  }
  
  void dispense(float grams) {
    Serial.printf("Dispensing %.1fg feed\n", grams);
    
    // Smooth motion to avoid jamming
    for (int pos = 0; pos <= 90; pos += 5) {
      feederServo.write(pos);
      delay(50);
    }
    delay(grams * 100);  // Time proportional to weight
    
    // Return
    for (int pos = 90; pos >= 0; pos -= 5) {
      feederServo.write(pos);
      delay(50);
    }
  }
  
  void antiJamCycle() {
    // Vibration pattern if jammed
    for (int i = 0; i < 3; i++) {
      feederServo.write(10);
      delay(200);
      feederServo.write(-10);
      delay(200);
    }
    feederServo.write(0);
  }
};

IntelligentFeeder ServoFeeder;

#endif
