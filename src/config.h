#ifndef CONFIG_H
#define CONFIG_H

// Wi-Fi. Replace these two values with your real router hotspot name and password.
#define WIFI_SSID       "REDMI K80"
#define WIFI_PASSWORD   "998244353"

// Spring Boot backend. Use the computer LAN IP, not localhost.
#define SERVER_HOST     "10.2.29.194"
#define SERVER_PORT     8080

// Device id must match the deviceId used by the Java backend.
#define DEVICE_ID       "esp32_001"

// Sensor pins.
#define PIN_DHT22       4
#define PIN_MQ2         34

// Actuator pins.
#define PIN_LED_1       2
#define PIN_LED_2       19
#define PIN_BUZZER      5
#define PIN_RELAY       18

// I2C pins.
#define I2C_SDA         21
#define I2C_SCL         22
#define OLED_ADDR       0x3C

// Time intervals in milliseconds.
#define SENSOR_READ_INTERVAL    2000   // 串口打印 + 传感器读数间隔（毫秒）
#define DATA_UPLOAD_INTERVAL    5000   // 数据上传到后端间隔
#define COMMAND_POLL_INTERVAL   2000   // 轮询后端命令间隔

// Local warning thresholds. The Java backend also has its own alarm rules.
#define TEMP_HIGH_THRESHOLD       35.0f
#define TEMP_LOW_THRESHOLD        5.0f
#define HUMIDITY_HIGH_THRESHOLD   85.0f
#define HUMIDITY_LOW_THRESHOLD    20.0f
#define SMOKE_HIGH_THRESHOLD      500
#define LIGHT_LOW_THRESHOLD       50

#endif
