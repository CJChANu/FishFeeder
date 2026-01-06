#ifndef DS3231_RTC_H
#define DS3231_RTC_H

#include <RTClib.h>
#include <WiFi.h>
#include "time.h"

class AdvancedRTC {
private:
  RTC_DS3231 rtc;
  unsigned long last_ntp_sync = 0;
  const unsigned long NTP_INTERVAL = 86400; // 24h

public:
  void init() {
    Wire.begin(21, 22);
    if (!rtc.begin()) {
      Serial.println("FATAL: RTC hardware failure");
      ESP.restart();
    }
    if (rtc.lostPower()) {
      Serial.println("RTC battery low - auto sync");
      syncNTP();
    }
  }
  
  DateTime now() { return rtc.now(); }
  
  void syncNTP() {
    if (WiFi.status() == WL_CONNECTED) {
      configTime(19800, 0, "pool.ntp.org"); // Sri Lanka
      struct tm timeinfo;
      if (getLocalTime(&timeinfo)) {
        rtc.adjust(DateTime(timeinfo.tm_year+1900, timeinfo.tm_mon+1, 
                           timeinfo.tm_mday, timeinfo.tm_hour, 
                           timeinfo.tm_min, timeinfo.tm_sec));
        Serial.println("NTP → RTC synced");
      }
    }
  }
  
  bool isFeedTime(uint8_t hour, uint8_t minute) {
    DateTime t = now();
    return (t.hour() == hour && t.minute() == minute);
  }
};

AdvancedRTC RTC;

#endif
