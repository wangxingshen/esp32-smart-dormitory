#include "sensors/dht_sensor.h"

#include <Arduino.h>

DHTSensor::DHTSensor(uint8_t pin) : dht(pin, DHT22), pin(pin) {}

bool DHTSensor::begin() {
    dht.begin();
    delay(100);
    return true;
}

bool DHTSensor::read(float &temperature, float &humidity) {
    float t = dht.readTemperature(false);
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
        return false;
    }

    temperature = t;
    humidity = h;
    return true;
}
