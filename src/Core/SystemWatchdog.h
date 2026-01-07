#ifndef SYSTEM_WATCHDOG_H
#define SYSTEM_WATCHDOG_H

#include <esp_task_wdt.h>

class ProductionWatchdog {
public:
    void init() {
    esp_task_wdt_config_t twdt_config = {
        .timeout_ms = 30000,
        .trigger_panic = true
        // Remove .idle_handle entirely
    };
    esp_task_wdt_reconfigure(&twdt_config);
    esp_task_wdt_add(NULL);
}

    void feed() {
        esp_task_wdt_reset();
    }
};

#endif