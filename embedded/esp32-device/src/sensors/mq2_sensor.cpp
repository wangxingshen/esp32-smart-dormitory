#include "sensors/mq2_sensor.h"

#include <Arduino.h>

MQ2Sensor::MQ2Sensor(uint8_t pin) : pin(pin) {}

bool MQ2Sensor::begin() {
    pinMode(pin, INPUT);
    delay(100);
    return true;
}

bool MQ2Sensor::read(int &rawValue) {
    rawValue = analogRead(pin);
    return true;
}
