#ifndef SYSTEM_WATCHDOG_H
#define SYSTEM_WATCHDOG_H

#include <esp_task_wdt.h>

class ProductionWatchdog {
public:
    void init() {
        // Updated for ESP32 SDK 3.x+
        esp_task_wdt_config_t twdt_config = {
            .timeout_ms = 30000, // 30 seconds
            .idle_handle = NULL,
            .trigger_panic = true
        };
        esp_task_wdt_reconfigure(&twdt_config);
        esp_task_wdt_add(NULL); // Add current thread (loop)
    }

    void feed() {
        esp_task_wdt_reset();
    }
};

#endif