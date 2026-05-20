# ESP32 端联调说明

ESP32 代码位于：

```text
esp32-smart-dormitory-master/
```

这是一个 PlatformIO 项目，入口文件是：

```text
esp32-smart-dormitory-master/src/main.cpp
```

## 当前已补全的功能

ESP32 端现在包含：

- 读取 DHT22 温湿度
- 读取 BH1750 光照
- 读取 MQ-2 烟雾模拟值
- OLED 显示传感器数据
- 连接 Wi-Fi
- 每 5 秒上传一次 JSON 到 Spring Boot
- 每 2 秒轮询一次后端控制命令
- 支持 LED、蜂鸣器、继电器控制
- 命令执行后回传 ack

## 必须修改的配置

打开：

```text
esp32-smart-dormitory-master/src/config.h
```

修改 Wi-Fi：

```cpp
#define WIFI_SSID       "你的WiFi名称"
#define WIFI_PASSWORD   "你的WiFi密码"
```

修改后端地址：

```cpp
#define SERVER_HOST     "192.168.1.100"
#define SERVER_PORT     8080
```

注意：

- `SERVER_HOST` 必须写电脑的局域网 IP。
- 不要写 `localhost`，因为 ESP32 里的 `localhost` 指的是 ESP32 自己。
- Spring Boot 后端端口目前是 `8080`。

电脑局域网 IP 可以在 PowerShell 输入：

```powershell
ipconfig
```

找到当前 Wi-Fi 网卡的 IPv4 地址。

## 后端需要先启动

先在 IDEA 中运行 Java 后端：

```text
IotApplication
```

确认浏览器访问这个接口有 JSON 返回：

```text
http://localhost:8080/api/device/latest?deviceId=esp32_001
```

如果返回：

```json
{
  "code": 200,
  "message": "success",
  "data": null
}
```

说明后端和 MySQL 已经通了。

## ESP32 上传的数据格式

ESP32 会向后端发送：

```http
POST http://电脑IP:8080/api/device/data
Content-Type: application/json
```

请求体类似：

```json
{
  "deviceId": "esp32_001",
  "temperature": 26.5,
  "humidity": 58.2,
  "light": 320,
  "smoke": 120
}
```

这个格式和后端 `SensorDataUploadDTO` 对应。

## 后端控制命令格式

网页或 Apifox 创建命令：

```http
POST http://localhost:8080/api/device/control
Content-Type: application/json
```

```json
{
  "deviceId": "esp32_001",
  "commandType": "light",
  "commandValue": "on"
}
```

ESP32 会轮询：

```text
GET /api/device/command?deviceId=esp32_001
```

拿到命令后执行，并回传：

```json
{
  "commandId": 1,
  "deviceId": "esp32_001",
  "status": "executed"
}
```

## 支持的 commandType

当前 ESP32 支持这些命令：

```text
light / led / led1
led2
buzzer
relay
fan
```

支持的 commandValue：

```text
on
off
1
0
true
beep
```

示例：

```json
{
  "deviceId": "esp32_001",
  "commandType": "buzzer",
  "commandValue": "beep"
}
```

## PlatformIO 编译

在 VS Code 安装 PlatformIO 插件后，打开：

```text
esp32-smart-dormitory-master/
```

然后执行：

```text
Build
Upload
Monitor
```

串口波特率是：

```text
115200
```

## 常见问题

### ESP32 连不上后端

检查：

- 电脑和 ESP32 是否连接同一个 Wi-Fi。
- `SERVER_HOST` 是否是电脑 Wi-Fi 的 IPv4 地址。
- Spring Boot 是否正在运行。
- Windows 防火墙是否拦截了 8080 端口。
- 后端地址是否写成了 `localhost`，如果是就改成电脑 IP。

### 后端没有收到数据

先用浏览器确认后端接口正常：

```text
http://localhost:8080/api/device/latest?deviceId=esp32_001
```

再看 ESP32 串口输出是否有：

```text
[UPLOAD] Success
```

如果是：

```text
[UPLOAD] Failed
```

重点检查 Wi-Fi、电脑 IP、防火墙和后端是否启动。
