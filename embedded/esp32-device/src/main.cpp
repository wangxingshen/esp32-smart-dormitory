#include <Arduino.h>

#include "config.h"
#include "actuators/buzzer.h"
#include "actuators/led.h"
#include "actuators/relay.h"
#include "communication/http_client.h"
#include "display/oled_display.h"
#include "network/wifi_manager.h"
#include "sensors/bh1750_sensor.h"
#include "sensors/dht_sensor.h"
#include "sensors/mq2_sensor.h"
#include "utils/json_helper.h"

// Sensors.
DHTSensor dht(PIN_DHT22);
BH1750Sensor bh(I2C_SDA, I2C_SCL);
MQ2Sensor mq2(PIN_MQ2);

// Display.
OLEDDisplay oled(OLED_ADDR, I2C_SDA, I2C_SCL);

// Network and backend client.
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
HttpClient httpClient(SERVER_HOST, SERVER_PORT);

// Actuators. Relay is active-low for many common 5V relay modules.
LED led1(PIN_LED_1);
LED led2(PIN_LED_2);
Buzzer buzzer(PIN_BUZZER);
Relay relay(PIN_RELAY, true);

unsigned long lastSensorRead = 0;
unsigned long lastDataUpload = 0;
unsigned long lastCommandPoll = 0;

float temperature = 0;
float humidity = 0;
float light = 0;
int smoke = 0;
bool hasSensorData = false;

void initSensors();
void initActuators();
void readAllSensors();
void printSensorData();
void updateOLED();
void uploadSensorData();
void pollCommand();
void handleCommand(const ControlCommand &command);
bool isOnValue(const String &value);

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("========================================");
    Serial.println(" Smart Dormitory Monitor - ESP32");
    Serial.println("========================================");

    initSensors();
    initActuators();

    if (oled.begin()) {
        oled.showMessage("ESP32 IoT", "Starting WiFi...", "");
        Serial.println("[OLED] OK");
    } else {
        Serial.println("[OLED] FAIL");
    }

    wifiManager.connect();
    if (wifiManager.isConnected()) {
        oled.showMessage("WiFi connected", wifiManager.getIP().c_str(), SERVER_HOST);
    } else {
        oled.showMessage("WiFi failed", "Check config.h", "");
    }

    Serial.println("========================================");
}

void loop() {
    unsigned long now = millis();

    wifiManager.checkConnection();
    buzzer.check();

    if (now - lastSensorRead >= SENSOR_READ_INTERVAL) {
        lastSensorRead = now;
        readAllSensors();
        printSensorData();
        updateOLED();
    }

    if (now - lastDataUpload >= DATA_UPLOAD_INTERVAL) {
        lastDataUpload = now;
        uploadSensorData();
    }

    if (now - lastCommandPoll >= COMMAND_POLL_INTERVAL) {
        lastCommandPoll = now;
        pollCommand();
    }
}

void initSensors() {
    Serial.print("[SENSOR] DHT22... ");
    Serial.println(dht.begin() ? "OK" : "FAIL");

    Serial.print("[SENSOR] BH1750... ");
    Serial.println(bh.begin() ? "OK" : "FAIL");

    Serial.print("[SENSOR] MQ-2... ");
    Serial.println(mq2.begin() ? "OK" : "FAIL");
}

void initActuators() {
    led1.begin();
    led2.begin();
    buzzer.begin();
    relay.begin();
    Serial.println("[ACTUATOR] LED/Buzzer/Relay OK");
}

void readAllSensors() {
    bool ok = true;

    if (!bh.read(light)) {
        ok = false;
        Serial.println("[WARN] BH1750 read failed");
    }

    if (!dht.read(temperature, humidity)) {
        ok = false;
        Serial.println("[WARN] DHT22 read failed");
    }

    if (!mq2.read(smoke)) {
        ok = false;
        Serial.println("[WARN] MQ2 read failed");
    }

    hasSensorData = ok;
}

void printSensorData() {
    Serial.println("--- Sensor Data ---");
    Serial.printf("  Device: %s\n", DEVICE_ID);
    Serial.printf("  Temp:   %.1f C\n", temperature);
    Serial.printf("  Hum:    %.1f %%\n", humidity);
    Serial.printf("  Light:  %.0f lx\n", light);
    Serial.printf("  Smoke:  %d\n", smoke);
}

void updateOLED() {
    oled.showSensorData(temperature, humidity, light, smoke);
}

void uploadSensorData() {
    if (!wifiManager.isConnected()) {
        Serial.println("[UPLOAD] Skip, WiFi not connected");
        return;
    }
    if (!hasSensorData) {
        Serial.println("[UPLOAD] Skip, no valid sensor data");
        return;
    }

    String payload = JsonHelper::buildSensorData(DEVICE_ID, temperature, humidity, light, smoke);
    Serial.print("[UPLOAD] ");
    Serial.println(payload);

    bool ok = httpClient.postSensorData(payload);
    Serial.println(ok ? "[UPLOAD] Success" : "[UPLOAD] Failed");
}

void pollCommand() {
    if (!wifiManager.isConnected()) {
        return;
    }

    String response = httpClient.getPendingCommand(DEVICE_ID);
    if (response.length() == 0) {
        return;
    }

    ControlCommand command = JsonHelper::parseCommand(response);
    if (!command.valid) {
        return;
    }

    handleCommand(command);

    String ack = JsonHelper::buildCommandAck(command.commandId, DEVICE_ID, "executed");
    httpClient.ackCommand(ack);
}

void handleCommand(const ControlCommand &command) {
    String type = command.commandType;
    String value = command.commandValue;
    type.toLowerCase();
    value.toLowerCase();

    Serial.printf("[COMMAND] id=%ld type=%s value=%s\n",
                  command.commandId,
                  type.c_str(),
                  value.c_str());

    if (type == "light" || type == "led" || type == "led1") {
        isOnValue(value) ? led1.on() : led1.off();
        return;
    }

    if (type == "led2") {
        isOnValue(value) ? led2.on() : led2.off();
        return;
    }

    if (type == "buzzer") {
        if (value == "beep") {
            buzzer.beep(1000);
        } else {
            isOnValue(value) ? buzzer.on() : buzzer.off();
        }
        return;
    }

    if (type == "relay" || type == "fan") {
        isOnValue(value) ? relay.on() : relay.off();
    }
}

bool isOnValue(const String &value) {
    return value == "on" || value == "1" || value == "true" || value == "open";
}
