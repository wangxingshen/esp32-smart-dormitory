#ifndef CONFIG_H
#define CONFIG_H

// ========== Wi-Fi ==========
#define WIFI_SSID     "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

// ========== 后端服务器 ==========
#define SERVER_HOST   "192.168.1.100"
#define SERVER_PORT   8000

// ========== 设备 ID ==========
#define DEVICE_ID     "esp32_001"

// ========== 引脚定义 ==========
#define PIN_DHT22      4   // DHT22 温湿度传感器
#define PIN_MQ2        34  // MQ-2 烟雾传感器 (ADC1)
#define PIN_LED_1      2   // LED 模块 1
#define PIN_LED_2      19  // LED 模块 2
#define PIN_BUZZER     5   // 有源蜂鸣器
#define PIN_RELAY      18  // 继电器模块

// I2C
#define I2C_SDA        21
#define I2C_SCL        22
#define OLED_ADDR      0x3C

// ========== 采集间隔 (毫秒) ==========
#define SENSOR_READ_INTERVAL   2000    // 传感器读取间隔
#define DATA_UPLOAD_INTERVAL   5000    // 数据上传间隔
#define COMMAND_POLL_INTERVAL  2000    // 命令轮询间隔

// ========== 报警阈值 ==========
#define TEMP_HIGH_THRESHOLD    35.0f   // 高温阈值 (°C)
#define TEMP_LOW_THRESHOLD     5.0f    // 低温阈值 (°C)
#define HUMIDITY_HIGH_THRESHOLD 85.0f  // 高湿阈值 (%)
#define HUMIDITY_LOW_THRESHOLD  20.0f  // 低湿阈值 (%)
#define SMOKE_HIGH_THRESHOLD   500     // 烟雾阈值 (ADC 原始值)
#define LIGHT_LOW_THRESHOLD    50      // 光照过低阈值 (lux)

#endif
