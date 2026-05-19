#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED {
public:
    LED(uint8_t pin, bool activeLow = false);
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
