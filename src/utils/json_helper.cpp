#include "utils/json_helper.h"

#include <ArduinoJson.h>
#include <cstring>

String JsonHelper::buildSensorData(const char *deviceId,
                                   float temperature,
                                   float humidity,
                                   float light,
                                   int smoke) {
    JsonDocument doc;
    doc["deviceId"] = deviceId;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["light"] = light;
    doc["smoke"] = smoke;

    String json;
    serializeJson(doc, json);
    return json;
}

String JsonHelper::buildCommandAck(long commandId,
                                   const char *deviceId,
                                   const char *status) {
    JsonDocument doc;
    doc["commandId"] = commandId;
    doc["deviceId"] = deviceId;
    doc["status"] = status;

    String json;
    serializeJson(doc, json);
    return json;
}

ControlCommand JsonHelper::parseCommand(const String &json) {
    ControlCommand command;
    command.valid = false;
    command.commandId = 0;

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        return command;
    }

    JsonVariant data = doc["data"];
    if (data.isNull()) {
        return command;
    }

    long commandId = data["commandId"] | 0;
    const char *commandType = data["commandType"] | "";
    const char *commandValue = data["commandValue"] | "";
    if (commandId <= 0 || strlen(commandType) == 0 || strlen(commandValue) == 0) {
        return command;
    }

    command.valid = true;
    command.commandId = commandId;
    command.commandType = commandType;
    command.commandValue = commandValue;
    return command;
}
