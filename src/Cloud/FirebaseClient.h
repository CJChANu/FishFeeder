#ifndef FIREBASE_CLIENT_H
#define FIREBASE_CLIENT_H

#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "Dialog-4G-DC5E"
#define WIFI_PASSWORD "admin"
#define API_KEY "your_firebase_web_api_key"
#define DATABASE_URL "https://smart-fish-feeder-pro-y1s1mtr7-default-rtdb.asia-southeast1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

class CloudSync {
public:
  void connect() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    Firebase.signUp(&config, &auth, "", ""); // Anonymous
    Firebase.begin(&config, &auth);
  }
  
  void syncData(SystemState* state) {
    if (Firebase.ready()) {
      String path = "/devices/fish_feeder_001";
      Firebase.Json data;
      data.set("timestamp/seconds", time(nullptr));
      data.set("temp", state->temp);
      data.set("ph", state->ph);
      data.set("turbidity", state->turbidity_ntu);
      data.set("quality_index", state->water_quality_index);
      
      Firebase.RTDB.setJSON(&fbdo, path + "/latest", &data);
    }
  }
};

CloudSync Firebase;

#endif
