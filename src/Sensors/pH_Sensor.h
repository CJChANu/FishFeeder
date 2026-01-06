#ifndef PH_SENSOR_H
#define PH_SENSOR_H

#define PH_PIN 34
#define PH_READINGS 10
#define PH_CALIBRATION_PH4 1.75   // Adjust with buffer
#define PH_CALIBRATION_PH7 2.05
#define PH_CALIBRATION_PH10 2.35

class CalibratedPHSensor {
private:
  float readings[PH_READINGS];
  uint8_t index = 0;
  float ph4_voltage, ph7_voltage;
  
public:
  void calibrate(float ph4_v, float ph7_v) {
    ph4_voltage = ph4_v;
    ph7_voltage = ph7_v;
    Serial.println("pH calibrated");
  }
  
  float readCalibrated(float temp_c) {
    float raw = analogRead(PH_PIN) * (3.3 / 4095.0);
    
    // Temperature compensation
    float compensated = raw * (1.0 + 0.02 * (temp_c - 25.0));
    
    // Linear calibration
    float slope = (PH_CALIBRATION_PH10 - PH_CALIBRATION_PH4) / 
                  (PH_CALIBRATION_PH10 - PH_CALIBRATION_PH4);
    float ph_value = PH7 + (PH7 - compensated) / slope;
    
    readings[index] = ph_value;
    index = (index + 1) % PH_READINGS;
    
    // Median filter
    float sorted[PH_READINGS];
    memcpy(sorted, readings, sizeof(readings));
    std::sort(sorted, sorted + PH_READINGS);
    return sorted[PH_READINGS/2];
  }
};

CalibratedPHSensor pHSensor;

#endif
