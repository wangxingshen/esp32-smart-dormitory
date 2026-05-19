#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
public:
    WiFiManager(const char *ssid, const char *password);
    bool connect(unsigned long timeoutMs = 15000);
    bool isConnected();
    void checkConnection();  // 断线自动重连
    String getIP();

private:
    const char *ssid;
    const char *password;
    unsigned long lastReconnectAttempt;
};

#endif
