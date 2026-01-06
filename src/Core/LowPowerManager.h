#ifndef LOWPOWER_MANAGER_H
#define LOWPOWER_MANAGER_H

class BatteryOptimizer {
public:
  void sleepUntilFeedTime() {
    DateTime now = RTC.now();
    unsigned long sleep_ms = 300000;  // 5 min default
    
    // Sleep longer between feeds
    if (!RTC.isFeedTime(8,0) && !RTC.isFeedTime(20,0)) {
      sleep_ms = 3600000;  // 1 hour
    }
    
    esp_sleep_enable_timer_wakeup(sleep_ms * 1000ULL);
    esp_deep_sleep_start();
  }
};

BatteryOptimizer PowerManager;

#endif
