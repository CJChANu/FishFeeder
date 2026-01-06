#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H

#include "Arduino.h"
#include <WiFi.h>
#include <RTClib.h>
#include "../Core/Types.h"

class ProductionLogger {
private:
  unsigned long last_heartbeat = 0;
  const unsigned long HEARTBEAT_INTERVAL = 30000;  // 30s
  
public:
  void init() {
    Serial.begin(115200);
    delay(1000);
    printBootBanner();
  }
  
  void printBootBanner() {
    Serial.println();
    Serial.println("╔══════════════════════════════════════╗");
    Serial.println("║  SMART FISH FEEDER v2.0 PROD        ║");
    Serial.println("║  SLIIT Year 1 IT/Engineering        ║");
    Serial.println("╚══════════════════════════════════════╝");
    Serial.printf("Boot: %s %s\n", __DATE__, __TIME__);
    Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
    Serial.println("───────────────────────────────────────");
  }
  
  void heartbeat(SystemState* state) {
    if (millis() - last_heartbeat > HEARTBEAT_INTERVAL) {
      Serial.println("❤️ SYSTEM ALIVE ❤️");
      Serial.printf("[RTC] %02d:%02d:%02d | T:%.1f°C pH:%.1f NTU:%.0f Q:%d\n",
                    RTC.now().hour(), RTC.now().minute(), RTC.now().second(),
                    state->temp, state->ph, state->turbidity_ntu,
                    state->water_quality_index);
      Serial.printf("Feed: %s | Anomalies: %s\n",
                    state->feeding_active ? "ACTIVE" : "IDLE",
                    state->anomaly_detected ? "DETECTED" : "NONE");
      Serial.printf("WiFi: %s | Uptime: %lus\n",
                    WiFi.status() == WL_CONNECTED ? "OK" : "DOWN",
                    millis() / 1000);
      Serial.println("───────────────────────────────────────");
      last_heartbeat = millis();
    }
  }
  
  void logEvent(const char* event) {
    Serial.printf("[EVENT] %s | %lus\n", event, millis() / 1000);
  }
  
  void logSensorCalibration(float ph4, float ph7, float temp) {
    Serial.printf("🔧 Calibration: pH4=%.2fV pH7=%.2fV Temp=%.1f°C\n", ph4, ph7, temp);
  }
  
  void logFeeding(float grams) {
    Serial.printf("🍽️  FEEDING: %.1fg dispensed | Quality Index: %d\n", grams, PredictiveAlgo.qualityScore(NULL));
  }
  
  void logAnomaly(SystemState* state) {
    Serial.printf("🚨 ANOMALY DETECTED! T=%.1f pH=%.1f NTU=%.0f\n",
                  state->temp, state->ph, state->turbidity_ntu);
  }
};

ProductionLogger Logger;

#endif
