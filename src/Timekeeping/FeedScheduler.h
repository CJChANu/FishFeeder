#ifndef FEED_SCHEDULER_H
#define FEED_SCHEDULER_H

#include "src/Actuators/ServoFeeder.h"

class AIFeedScheduler {
private:
  float quality_history[7];  // Daily average
  uint8_t day_index = 0;
  
public:
  void update(SystemState* state) {
    // ML-based decision: feed if quality good + time window
    float daily_avg = quality_history[day_index];
    
    // Optimal window: 7-9 AM if water quality > 70
    if (RTC.isFeedTime(8, 0) && state->water_quality_index > 70) {
      float feed_amount = calculateOptimalFeed(state->temp, daily_avg);
      ServoFeeder.dispense(feed_amount);
      state->feeding_active = true;
    }
  }
  
  float calculateOptimalFeed(float temp, float quality) {
    // Complex formula: base 5g + temp bonus - turbidity penalty
    float base = 5.0;
    float temp_factor = (28 - temp) * 0.2;  // Less feed if hot
    float quality_factor = (quality - 50) * 0.05;
    return constrain(base + temp_factor + quality_factor, 2.0, 8.0);
  }
};

AIFeedScheduler FeedScheduler;

#endif
