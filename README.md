# ESP32 智能宿舍环境监测与远程控制系统

这是一个“ESP32 嵌入式端 + Java Spring Boot 后端 + 静态前端页面”的 IoT 项目。

当前核心链路：

```text
ESP32 采集温湿度、光照、烟雾
-> HTTP JSON 上传
-> Spring Boot 接收
-> MyBatis-Plus 写入 MySQL
-> 前端页面动态展示
-> 网页下发控制命令
-> ESP32 轮询并执行命令
```

## 目录结构

```text
esp32/
├── pom.xml                         # Java 后端 Maven 配置
├── README.md                       # 当前说明
├── docs/                           # 接口测试和项目讲解文档
├── embedded/
│   └── esp32-device/               # ESP32 PlatformIO 项目
├── src/main/java/com/example/iot/  # Spring Boot 后端代码
├── src/main/resources/
│   ├── application.yml             # 后端配置
│   ├── schema.sql                  # MySQL 建表脚本
│   └── static/                     # 前端静态页面
└── target/                         # Maven 构建产物，已忽略
```

## 后端启动

1. 在 MySQL 执行：

```text
src/main/resources/schema.sql
```

2. 修改数据库配置：

```text
src/main/resources/application.yml
```

3. 启动后端：

```powershell
mvn spring-boot:run
```

4. 打开前端页面：

```text
http://localhost:8080/dashboard.html
```

不要直接用 `file:///.../dashboard.html` 打开前端页面，否则接口请求路径和刷新行为可能不符合后端运行环境。

## ESP32 端

ESP32 项目目录：

```text
embedded/esp32-device
```

在 VS Code + PlatformIO 中打开这个目录，修改：

```text
embedded/esp32-device/src/config.h
```

重点配置：

```cpp
#define WIFI_SSID       "你的WiFi"
#define WIFI_PASSWORD   "你的WiFi密码"
#define SERVER_HOST     "你的电脑局域网IP"
#define SERVER_PORT     8080
```

然后执行：

```text
Build
Upload
Monitor
```

串口看到：

```text
[UPLOAD] Success
```

说明 ESP32 已经成功上传数据到后端。

## 常用接口

```http
POST /api/device/data
GET  /api/device/latest?deviceId=esp32_001
GET  /api/device/history?deviceId=esp32_001&limit=100
GET  /api/device/status?deviceId=esp32_001
POST /api/device/control
GET  /api/device/command?deviceId=esp32_001
POST /api/device/command/ack
GET  /api/alarm/list
POST /api/alarm/handle
```
