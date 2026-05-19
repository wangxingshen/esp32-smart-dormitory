#include "sensors/bh1750_sensor.h"

BH1750Sensor::BH1750Sensor(uint8_t sda, uint8_t scl)
    : sda(sda), scl(scl) {}

bool BH1750Sensor::begin() {
    Wire.begin(sda, scl);
    return sensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
}

bool BH1750Sensor::read(float &lux) {
    if (!sensor.measurementReady()) {
        return false;
    }
    lux = sensor.readLightLevel();
    return lux >= 0;
}
