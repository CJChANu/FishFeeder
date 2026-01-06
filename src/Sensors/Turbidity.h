#ifndef TURBIDITY_H
#define TURBIDITY_H

#define TURB_PIN 35

// Kalman filter parameters
class KalmanFilter {
private:
  float q = 0.125;  // Process noise
  float r = 0.5;    // Measurement noise
  float x = 0;      // Value
  float p = 1;      // Estimation error
  
public:
  float update(float measurement) {
    // Prediction
    p += q;
    
    // Update
    float k = p / (p + r);
    x += k * (measurement - x);
    p *= (1 - k);
    return x;
  }
};

class TurbiditySensor {
private:
  KalmanFilter kalman;
  
public:
  float readNTU() {
    int raw_adc = analogRead(TURB_PIN);
    float voltage = raw_adc * (3.3 / 4095.0);
    
    // Turbidity formula (calibrate with standards)
    float ntu = (-1120.4 * voltage * voltage) + (5742.3 * voltage) - 4352.9;
    return constrain(ntu, 0, 1000);
  }
  
  float kalmanFilter() {
    float raw_ntu = readNTU();
    return kalman.update(raw_ntu);
  }
};

TurbiditySensor Turbidity;

#endif
