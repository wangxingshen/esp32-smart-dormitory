#include <Arduino.h>
#include "config.h"

// 已接: BH1750 + DHT22 + MQ2 + OLED
// 待接: LED, Buzzer, Relay, 网络

// 传感器
#include "sensors/dht_sensor.h"
#include "sensors/bh1750_sensor.h"
#include "sensors/mq2_sensor.h"

// 显示
#include "display/oled_display.h"

// ========== 全局对象 ==========
DHTSensor    dht(PIN_DHT22);
BH1750Sensor bh(I2C_SDA, I2C_SCL);
MQ2Sensor    mq2(PIN_MQ2);

OLEDDisplay oled(OLED_ADDR, I2C_SDA, I2C_SCL);

// LED     led1(PIN_LED_1);           // 未接
// LED     led2(PIN_LED_2);           // 未接
// Buzzer  buzzer(PIN_BUZZER);        // 未接
// Relay   relay(PIN_RELAY, true);    // 未接

// ========== 时间变量 ==========
unsigned long lastSensorRead = 0;

// ========== 传感器数据 ==========
float temperature = 0, humidity = 0, light = 0;
int   smoke = 0;

// ========== 函数声明 ==========
void readAllSensors();
void printSensorData();
void updateOLED();

// ========== 初始化 ==========
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n========================================");
    Serial.println("  智能宿舍环境监测系统 - Phase 1");
    Serial.println("========================================");

    // --- DHT22 ---
    Serial.print("[SENSOR] DHT22... ");
    Serial.println(dht.begin() ? "OK" : "FAIL");

    // --- BH1750 ---
    Serial.print("[SENSOR] BH1750... ");
    Serial.println(bh.begin() ? "OK" : "FAIL");

    // --- MQ-2 ---
    Serial.print("[SENSOR] MQ-2... ");
    Serial.println(mq2.begin() ? "OK" : "FAIL");

    // --- OLED ---
    Serial.print("[OLED] SSD1306... ");
    if (oled.begin()) {
        oled.showMessage("All Sensors OK", "DHT22+BH1750+MQ2", "");
        Serial.println("OK");
    } else {
        Serial.println("FAIL");
    }

    Serial.println("========================================\n");
}

// ========== 主循环 ==========
void loop() {
    unsigned long now = millis();

    if (now - lastSensorRead >= SENSOR_READ_INTERVAL) {
        lastSensorRead = now;
        readAllSensors();
        printSensorData();
        updateOLED();
    }
}

// ========== 读取传感器 ==========
void readAllSensors() {
    // --- BH1750 ---
    if (!bh.read(light)) {
        Serial.println("[WARN] BH1750 read failed");
    }

    // --- DHT22 ---
    if (!dht.read(temperature, humidity)) {
        Serial.println("[WARN] DHT22 read failed");
    }

    //--- MQ2 ---
    if (!mq2.read(smoke)) {
        Serial.println("[WARN] MQ2 read failed");
    }
}

// ========== 串口打印 ==========
void printSensorData() {
    Serial.println("--- Sensor Data ---");
    Serial.printf("  Temp:  %.1f C\n", temperature);
    Serial.printf("  Hum:   %.1f %%\n", humidity);
    Serial.printf("  Light: %.0f lx\n", light);
    Serial.printf("  Smoke: %d\n", smoke);
}

// ========== 更新 OLED ==========
void updateOLED() {
    oled.showSensorData(temperature, humidity, light, smoke);
}
