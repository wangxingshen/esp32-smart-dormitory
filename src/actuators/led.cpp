#include "actuators/led.h"

LED::LED(uint8_t pin, bool activeLow)
    : pin(pin), activeLow(activeLow), state(false) {}

void LED::begin() {
    pinMode(pin, OUTPUT);
    off();
}

void LED::on() {
    state = true;
    digitalWrite(pin, activeLow ? LOW : HIGH);
}

void LED::off() {
    state = false;
    digitalWrite(pin, activeLow ? HIGH : LOW);
}

void LED::toggle() {
    state ? off() : on();
}

bool LED::isOn() {
    return state;
}
