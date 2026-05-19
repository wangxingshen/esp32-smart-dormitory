#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <Arduino.h>

struct ControlCommand {
    bool valid;
    String deviceId;
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

    static String buildCommandAck(const char *deviceId,
                                  const char *commandType,
                                  const char *status);

    static ControlCommand parseCommand(const String &json);
};

#endif
