#include "display/oled_display.h"

OLEDDisplay::OLEDDisplay(uint8_t addr, uint8_t sda, uint8_t scl)
    : oled(128, 64, &Wire, -1), addr(addr) {}

bool OLEDDisplay::begin() {
    // Wire 由 BH1750 先初始化，这里不再重复 begin()
    if (!oled.begin(SSD1306_SWITCHCAPVCC, addr)) {
        return false;
    }
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);
    oled.println("OLED OK!");
    oled.display();
    delay(500);
    return true;
}

void OLEDDisplay::showSensorData(float temp, float humidity, float light, int smoke) {
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.printf("Temp:  %.1f C", temp);
    oled.setCursor(0, 16);
    oled.printf("Hum:   %.1f %%", humidity);
    oled.setCursor(0, 32);
    oled.printf("Light: %.0f lx", light);
    oled.setCursor(0, 48);
    oled.printf("Smoke: %d", smoke);
    oled.display();
}

void OLEDDisplay::showMessage(const char *line1, const char *line2, const char *line3) {
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.println(line1);
    if (strlen(line2) > 0) {
        oled.setCursor(0, 20);
        oled.println(line2);
    }
    if (strlen(line3) > 0) {
        oled.setCursor(0, 40);
        oled.println(line3);
    }
    oled.display();
}

void OLEDDisplay::clear() {
    oled.clearDisplay();
    oled.display();
}
