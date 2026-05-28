#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <DHT.h>

class DHTSensor {
public:
    DHTSensor(uint8_t pin);
    bool begin();
    bool read(float &temperature, float &humidity);

private:
    DHT dht;
    uint8_t pin;
};

#endif
