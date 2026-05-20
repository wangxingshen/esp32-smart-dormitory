#include "actuators/relay.h"

Relay::Relay(uint8_t pin, bool activeLow)
    : pin(pin), activeLow(activeLow), state(false) {}

void Relay::begin() {
    pinMode(pin, OUTPUT);
    off();
}

void Relay::on() {
    state = true;
    digitalWrite(pin, activeLow ? LOW : HIGH);
}

void Relay::off() {
    state = false;
    digitalWrite(pin, activeLow ? HIGH : LOW);
}

void Relay::toggle() {
    state ? off() : on();
}

bool Relay::isOn() {
    return state;
}
