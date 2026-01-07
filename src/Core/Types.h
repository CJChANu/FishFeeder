#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

// In src/Core/Types.h
struct SystemState {
    float temp_celsius;   // Changed from temp
    float temp;
    float ph;
    float water_quality_index;
    float quality_index;
    float ph_value;       // Changed from ph
    float turbidity_ntu;
    uint8_t water_quality_index;
    uint32_t uptime_seconds; // Added this missing member
    bool feeding_active;
    bool anomaly_detected;
};

#endif