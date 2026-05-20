#include "actuators/buzzer.h"

Buzzer::Buzzer(uint8_t pin)
    : pin(pin), active(false), stopTime(0) {}

void Buzzer::begin() {
    pinMode(pin, OUTPUT);
    off();
}

void Buzzer::on() {
    active = true;
    stopTime = 0;
    digitalWrite(pin, HIGH);
}

void Buzzer::off() {
    active = false;
    stopTime = 0;
    digitalWrite(pin, LOW);
}

void Buzzer::beep(unsigned int durationMs) {
    active = true;
    stopTime = millis() + durationMs;
    digitalWrite(pin, HIGH);
}

void Buzzer::check() {
    if (active && stopTime > 0 && millis() >= stopTime) {
        off();
    }
}
