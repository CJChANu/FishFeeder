#ifndef DS18B20_H
#define DS18B20_H

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 15

class FilteredTempSensor {
private:
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors;
  float readings[10];
  uint8_t index = 0;
  
public:
  void init() {
    sensors.setOneWire(&oneWire);
    sensors.begin();
    if (sensors.getDeviceCount() == 0) {
      Serial.println("ERROR: No DS18B20 found");
    }
  }
  
  float readFiltered() {
    sensors.requestTemperatures();
    float raw = sensors.getTempCByIndex(0);
    if (raw == DEVICE_DISCONNECTED_C) return readings[index-1];
    
    readings[index] = raw;
    index = (index + 1) % 10;
    
    // Moving average filter
    float sum = 0;
    for (int i = 0; i < 10; i++) sum += readings[i];
    return sum / 10.0;
  }
};

FilteredTempSensor DS18B20;

#endif
