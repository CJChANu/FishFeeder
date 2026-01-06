#ifndef DS18B20_H
#define DS18B20_H

#include <OneWire.h>
#include <DallasTemperature.h>

class FilteredTempSensor {
private:
    OneWire oneWire;
    DallasTemperature sensors;

public:
    // Initializer list handles the object setup correctly
    FilteredTempSensor(int pin) : oneWire(pin), sensors(&oneWire) {}

    void init() {
        sensors.begin();
    }

    float read() {
        sensors.requestTemperatures();
        float temp = sensors.getTempCByIndex(0);
        return (temp == DEVICE_DISCONNECTED_C) ? 26.0 : temp; 
    }
};

#endif