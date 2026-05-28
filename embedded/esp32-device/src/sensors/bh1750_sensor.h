#ifndef BH1750_SENSOR_H
#define BH1750_SENSOR_H

#include <BH1750.h>
#include <Wire.h>

class BH1750Sensor {
public:
    BH1750Sensor(uint8_t sda, uint8_t scl);
    bool begin();
    bool read(float &lux);

private:
    BH1750 sensor;
    uint8_t sda, scl;
};

#endif
