#ifndef BUZZER_H
#define BUZZER_H

#define BUZZER_PIN 26

class AdvancedBuzzer {
public:
  void criticalAlert() {
    // SOS pattern
    tone(BUZZER_PIN, 800, 200);
    delay(200);
    tone(BUZZER_PIN, 800, 200);
    delay(200);
    tone(BUZZER_PIN, 800, 500);
    delay(600);
  }
  
  void melodySuccess() {
    int notes[] = {659, 698, 784};  // E5 F5 G5
    for (int i = 0; i < 3; i++) {
      tone(BUZZER_PIN, notes[i], 200);
      delay(250);
    }
    noTone(BUZZER_PIN);
  }
};

AdvancedBuzzer Buzzer;

#endif
