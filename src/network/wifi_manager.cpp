#include "network/wifi_manager.h"

WiFiManager::WiFiManager(const char *ssid, const char *password)
    : ssid(ssid), password(password), lastReconnectAttempt(0) {}

bool WiFiManager::connect(unsigned long timeoutMs) {
    Serial.printf("[WIFI] Connecting to \"%s\" with password \"%s\"\n", ssid, password);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    wl_status_t status = WiFi.status();
    if (status == WL_CONNECTED) {
        Serial.print("[WIFI] Connected, IP: ");
        Serial.println(WiFi.localIP());
        return true;
    }

    Serial.printf("[WIFI] Connect failed, status=%d\n", (int)status);
    return false;
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::checkConnection() {
    if (isConnected()) {
        return;
    }

    unsigned long now = millis();
    if (now - lastReconnectAttempt < 5000) {
        return;
    }

    lastReconnectAttempt = now;
    Serial.println("[WIFI] Disconnected, reconnecting...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
}

String WiFiManager::getIP() {
    if (!isConnected()) {
        return "";
    }
    return WiFi.localIP().toString();
}
