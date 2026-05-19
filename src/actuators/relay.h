#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay {
public:
    Relay(uint8_t pin, bool activeLow = true);
    void begin();
    void on();
    void off();
    void toggle();
    bool isOn();

private:
    uint8_t pin;
    bool activeLow;
    bool state;
};

#endif
