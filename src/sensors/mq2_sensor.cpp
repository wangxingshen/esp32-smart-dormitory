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
        rawValue = lastValid;  // 丢弃无效值，沿用上一次
        return true;
    }

    // 滑动平均，平滑抖动
    lastValid = (lastValid * 3 + raw) / 4;
    rawValue = lastValid;
    return true;
}
