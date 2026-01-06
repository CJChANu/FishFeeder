#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

struct SystemState {
    float temp;
    float ph;
    float turbidity_ntu;
    uint8_t water_quality_index;
    bool feeding_active;
    bool anomaly_detected;
    uint8_t quality; // For PredictiveAlgo
    bool alert;     // For PredictiveAlgo
};

#endif