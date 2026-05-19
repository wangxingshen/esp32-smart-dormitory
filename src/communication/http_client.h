#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>

class HttpClient {
public:
    HttpClient(const char *host, uint16_t port);
    bool postSensorData(const String &jsonPayload);
    String getPendingCommand(const char *deviceId);
    bool ackCommand(const String &jsonPayload);

private:
    String baseUrl;
    String buildUrl(const char *path);
};

#endif
