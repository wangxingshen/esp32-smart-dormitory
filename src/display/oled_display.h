#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

class OLEDDisplay {
public:
    OLEDDisplay(uint8_t addr, uint8_t sda, uint8_t scl);
    bool begin();
    void showSensorData(float temp, float humidity, float light, int smoke);
    void showMessage(const char *line1, const char *line2 = "", const char *line3 = "");
    void clear();

private:
    Adafruit_SSD1306 oled;
    uint8_t addr;
};

#endif
