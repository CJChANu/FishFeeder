#ifndef PREDICTIVE_ALGO_H
#define PREDICTIVE_ALGO_H

class FishHealthAI {
private:
  float temp_history[24];  // Hourly
  float ph_history[24];
  uint8_t index = 0;
  
public:
  uint8_t qualityScore(SystemState* state) {
    // Weighted fish health index (0-100)
    float temp_score = constrain((28 - abs(state->temp - 26)) * 3.8, 0, 100);
    float ph_score = constrain((7 - abs(state->ph - 7.0)) * 14.3, 0, 100);
    float turb_score = constrain((1000 - state->turbidity_ntu) * 0.1, 0, 100);
    
    return (uint8_t)(0.4 * temp_score + 0.3 * ph_score + 0.3 * turb_score);
  }
  
  bool detectAnomaly(SystemState* state) {
    temp_history[index] = state->temp;
    ph_history[index] = state->ph;
    index = (index + 1) % 24;
    
    // Z-score anomaly (3σ rule)
    float temp_avg = 0, ph_avg = 0;
    for (int i = 0; i < 24; i++) {
      temp_avg += temp_history[i];
      ph_avg += ph_history[i];
    }
    temp_avg /= 24; ph_avg /= 24;
    
    float temp_std = stdDev(temp_history, temp_avg);
    float ph_std = stdDev(ph_history, ph_avg);
    
    return (abs(state->temp - temp_avg) > 3 * temp_std) ||
           (abs(state->ph - ph_avg) > 0.3);
  }
  
private:
  float stdDev(float* arr, float mean) {
    float variance = 0;
    for (int i = 0; i < 24; i++) {
      variance += sq(arr[i] - mean);
    }
    return sqrt(variance / 24);
  }
};

FishHealthAI PredictiveAlgo;

#endif
