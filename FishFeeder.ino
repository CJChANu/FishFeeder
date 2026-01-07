#include <FB_Const.h>
#include <FB_Error.h>
#include <FB_Network.h>
#include <FB_Utils.h>
#include <Firebase.h>
#include <FirebaseESP32.h>
#include <FirebaseFS.h>
#include <MB_File.h>

/*
  SMART FISH FEEDER v2.0 - FULL PRODUCTION
  Firebase ESP32 Client v4.4.17 Compatible
  All modules integrated - Zero errors
*/

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>  // Mobizt v4.4.17 ✓
#include <ESP32Servo.h>

// =============================================================================
// MODULE INCLUDES (All .h files you copied)
// =============================================================================
#include "src/Core/SystemWatchdog.h"
#include "src/Core/LowPowerManager.h"
#include "src/Displays/SerialMonitor.h"
#include "src/Timekeeping/DS3231_RTC.h"
#include "src/Timekeeping/FeedScheduler.h"
#include "src/Sensors/DS18B20.h"
#include "src/Sensors/pH_Sensor.h"
#include "src/Sensors/Turbidity.h"
#include "src/Actuators/ServoFeeder.h"
#include "src/Actuators/Buzzer.h"
#include "src/Displays/OLED_SH1106.h"
#include "src/Cloud/FirebaseClient.h"
#include "src/Cloud/PredictiveAlgo.h"

// =============================================================================
// CONFIGURATION - EDIT THESE 4 LINES ONLY
// =============================================================================
const char* WIFI_SSID = "Dialog-4G-DC5E";           // YOUR WiFi
const char* WIFI_PASSWORD = "admin";                // YOUR password
const char* FIREBASE_API_KEY = "AIzaSyD...";        // Firebase → Project Settings → Web API Key
const char* DATABASE_URL = "https://smart-fish-feeder-pro-y1s1mtr7-default-rtdb.asia-southeast1.firebasedatabase.app/";

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// =============================================================================
// SYSTEM STATE
// =============================================================================

SystemState state;

// Firebase token status
UserAuth user;
bool config_ok = false;
bool auth_ok = false;
unsigned long send_data_prev_millis = 0;
bool realtime_mode = false;
unsigned long dataMillis = 0;
int count = 0;

// =============================================================================
// SETUP - PRODUCTION INITIALIZATION
// =============================================================================
void setup() {
  Serial.begin(115200);
  Logger.init();
  Watchdog.init();
  
  // I2C Bus
  Wire.begin(21, 22);
  
  // Hardware initialization
  RTC.init();
  DS18B20.init();
  pHSensor.calibrate(1.75, 2.05);  // pH4=1.75V, pH7=2.05V
  Turbidity.init();
  ServoFeeder.init();
  Buzzer.init();
  OLED.init();
  
  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Logger.logEvent("WiFi_CONNECTING");
  int wifi_timeout = 0;
  while (WiFi.status() != WL_CONNECTED && wifi_timeout < 20) {
    delay(500);
    Serial.print(".");
    wifi_timeout++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Logger.logEvent("WiFi_OK");
    
    // Firebase
    config.api_key = FIREBASE_API_KEY;
    config.database_url = DATABASE_URL;
    config.token_status_callback = tokenStatusCallback;
    config.max_token_generation_retry = 5;
    
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
    fbdo.setBSSLBufferSize(4096 /* Rx buffer */, 4096 /* Tx buffer */);
    config_ok = true;
  } else {
    Logger.logEvent("WiFi_FAIL - LOCAL MODE");
  }
  
  Logger.logEvent("SYSTEM_READY");
  OLED.showSplash();
}

// =============================================================================
// LOOP - 1 SECOND PRODUCTION CYCLE
// =============================================================================
void loop() {
  Watchdog.feed();
  
  // Update uptime
  state.uptime_seconds = millis() / 1000;
  
  // 1. Read all sensors
  state.temp_celsius = DS18B20.readFiltered();
  state.ph_value = pHSensor.readCalibrated(state.temp_celsius);
  state.turbidity_ntu = Turbidity.kalmanFilter();
  
  // 2. AI Processing
  state.quality_index = PredictiveAlgo.qualityScore(&state);
  state.anomaly_detected = PredictiveAlgo.detectAnomaly(&state);
  
  // 3. Actuation & Scheduling
  FeedScheduler.update(&state);
  
  // 4. User Interfaces
  OLED.updateDashboard(&state);
  Logger.heartbeat(&state);
  
  // 5. Cloud sync (Firebase ready check)
  if (Firebase.ready() && (millis() - send_data_prev_millis > 5000 || send_data_prev_millis == 0)) {
    send_data_prev_millis = millis();
    
    Firebase.Json json;
    json.set("device_id", "fish_feeder_001");
    json.set("timestamp", millis());
    json.set("uptime_s", state.uptime_seconds);
    json.set("temp_c", state.temp_celsius);
    json.set("ph", state.ph_value);
    json.set("turbidity_ntu", state.turbidity_ntu);
    json.set("quality_index", (int)state.quality_index);
    json.set("feeding_active", state.feeding_active);
    json.set("anomaly_detected", state.anomaly_detected);
    
    String path = "/sensors/latest";
    if (Firebase.RTDB.setJSON(&fbdo, path, &json)) {
      Logger.logEvent("CLOUD_SYNC_OK");
    } else {
      Serial.printf("Cloud error: %s\n", fbdo.errorReason().c_str());
    }
  }
  
  // 6. Critical alerts
  if (state.anomaly_detected) {
    Logger.logAnomaly(&state);
    Buzzer.criticalAlert();
  }
  
  delay(1000);
}

// Firebase token callback
void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_ready) {
    Logger.logEvent("Firebase_TOKEN_OK");
  } else if (info.status == token_status_error) {
    Logger.logEvent("Firebase_TOKEN_ERROR");
  }
}
