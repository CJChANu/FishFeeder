#ifndef SYSTEM_WATCHDOG_H
#define SYSTEM_WATCHDOG_H

class ProductionWatchdog {
public:
  void init() {
    esp_task_wdt_init(30, true);  // 30s timeout
    esp_task_wdt_add(NULL);
  }
  
  void feed() {
    esp_task_wdt_reset();
  }
};

ProductionWatchdog Watchdog;

#endif
