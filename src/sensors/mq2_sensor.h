#ifndef MQ2_SENSOR_H
#define MQ2_SENSOR_H

#include <Arduino.h>

class MQ2Sensor {
public:
    MQ2Sensor(uint8_t pin);
    bool begin();
    bool read(int &rawValue);

private:
    uint8_t pin;
};

#endif
