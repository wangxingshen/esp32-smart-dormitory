#include "communication/http_client.h"

HttpClient::HttpClient(const char *host, uint16_t port) {
    baseUrl = "http://";
    baseUrl += host;
    baseUrl += ":";
    baseUrl += String(port);
}

bool HttpClient::postSensorData(const String &jsonPayload) {
    HTTPClient http;
    http.begin(buildUrl("/api/device/data"));
    http.addHeader("Content-Type", "application/json");

    int code = http.POST(jsonPayload);
    String response = http.getString();
    http.end();

    Serial.printf("[HTTP] POST /api/device/data -> %d\n", code);
    if (response.length() > 0) {
        Serial.println(response);
    }
    return code >= 200 && code < 300;
}

String HttpClient::getPendingCommand(const char *deviceId) {
    HTTPClient http;
    String url = buildUrl("/api/device/command");
    url += "?deviceId=";
    url += deviceId;

    http.begin(url);
    int code = http.GET();
    String response = http.getString();
    http.end();

    Serial.printf("[HTTP] GET /api/device/command -> %d\n", code);
    if (code >= 200 && code < 300) {
        return response;
    }
    return "";
}

bool HttpClient::ackCommand(const String &jsonPayload) {
    HTTPClient http;
    http.begin(buildUrl("/api/device/command/ack"));
    http.addHeader("Content-Type", "application/json");

    int code = http.POST(jsonPayload);
    String response = http.getString();
    http.end();

    Serial.printf("[HTTP] POST /api/device/command/ack -> %d\n", code);
    if (response.length() > 0) {
        Serial.println(response);
    }
    return code >= 200 && code < 300;
}

String HttpClient::buildUrl(const char *path) {
    return baseUrl + path;
}
