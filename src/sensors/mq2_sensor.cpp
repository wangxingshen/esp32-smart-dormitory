#include "sensors/mq2_sensor.h"
#include <Arduino.h>

MQ2Sensor::MQ2Sensor(uint8_t pin) : pin(pin) {}

bool MQ2Sensor::begin() {
    pinMode(pin, INPUT);
    // MQ-2 预热至少 20 秒，这里延迟等传感器稳定
    delay(100);
    return true;
}

bool MQ2Sensor::read(int &rawValue) {
    rawValue = analogRead(pin);  // ESP32: 0-4095
    return true;
}
