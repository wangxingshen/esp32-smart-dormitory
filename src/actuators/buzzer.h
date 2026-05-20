#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
public:
    Buzzer(uint8_t pin);
    void begin();
    void on();
    void off();
    void beep(unsigned int durationMs);
    void check();

private:
    uint8_t pin;
    bool active;
    unsigned long stopTime;
};

#endif
