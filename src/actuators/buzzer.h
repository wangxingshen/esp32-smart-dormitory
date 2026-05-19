#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
public:
    Buzzer(uint8_t pin);
    void begin();
    void on();
    void off();
    void beep(unsigned int durationMs);  // 非阻塞
    void check();  // 检查是否需要关闭

private:
    uint8_t pin;
    bool active;
    unsigned long stopTime;
};

#endif
