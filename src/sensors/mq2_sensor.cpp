#include "sensors/mq2_sensor.h"

#include <Arduino.h>

MQ2Sensor::MQ2Sensor(uint8_t pin) : pin(pin), lastValid(0) {}

bool MQ2Sensor::begin() {
    pinMode(pin, INPUT);
    delay(100);
    return true;
}

bool MQ2Sensor::read(int &rawValue) {
    int raw = analogRead(pin);

    if (raw <= 5 || raw >= 4090) {
        rawValue = lastValid;
        return true;
    }

    // Smooth noisy analog values with a simple moving average.
    lastValid = (lastValid * 3 + raw) / 4;
    rawValue = lastValid;
    return true;
}
