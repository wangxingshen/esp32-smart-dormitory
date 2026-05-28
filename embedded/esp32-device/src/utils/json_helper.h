#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <Arduino.h>

struct ControlCommand {
    bool valid;
    long commandId;
    String commandType;
    String commandValue;
};

class JsonHelper {
public:
    static String buildSensorData(const char *deviceId,
                                  float temperature,
                                  float humidity,
                                  float light,
                                  int smoke);

    static String buildCommandAck(long commandId,
                                  const char *deviceId,
                                  const char *status);

    static ControlCommand parseCommand(const String &json);
};

#endif
