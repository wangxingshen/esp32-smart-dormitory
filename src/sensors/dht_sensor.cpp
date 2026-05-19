#include "sensors/dht_sensor.h"

DHTSensor::DHTSensor(uint8_t pin) : dht(pin, DHT22), pin(pin) {}

bool DHTSensor::begin() {
    dht.begin();
    delay(100);
    // 首次读取可能失败，预热一次
    float t, h;
    return dht.readTemperature(false) > -40;  // DHT22 最低 -40°C
}

bool DHTSensor::read(float &temperature, float &humidity) {
    float t = dht.readTemperature(false);   // 不强制 true=华氏度, false=摄氏度
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
        return false;
    }

    temperature = t;
    humidity = h;
    return true;
}
