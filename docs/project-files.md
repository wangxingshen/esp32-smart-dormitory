# 项目文件讲解

这份文档按目录说明每个文件的作用。你学习时可以先看“整体流程”，再按 Controller -> DTO -> Service -> Entity/Mapper -> MySQL 的顺序读代码。

当前项目分成两块：

- Java 后端和前端页面在根目录的 `src/` 下。
- ESP32 嵌入式代码在 `embedded/esp32-device/` 下。

## 整体流程

以 ESP32 上传传感器数据为例：

```text
ESP32 / Apifox
  -> POST /api/device/data
  -> DeviceDataController
  -> SensorDataUploadDTO
  -> DeviceDataServiceImpl
  -> SensorDataMapper
  -> sensor_data 表
```

查询最新数据的流程：

```text
浏览器 / 前端
  -> GET /api/device/latest?deviceId=esp32_001
  -> DeviceDataController
  -> DeviceDataServiceImpl
  -> SensorDataMapper
  -> sensor_data 表
  -> SensorDataVO
  -> Result JSON
```

## 根目录文件

### `pom.xml`

Maven 项目配置文件。它负责声明项目名称、Java 版本、依赖和打包方式。

本项目里重要依赖有：

- `spring-boot-starter-web`：提供 Spring MVC 和 HTTP 接口能力。
- `mybatis-plus-spring-boot3-starter`：让 Java 代码可以方便操作 MySQL。
- `mysql-connector-j`：MySQL JDBC 驱动，Spring Boot 连接 MySQL 必须有它。
- `lombok`：通过 `@Data`、`@RequiredArgsConstructor` 自动生成 getter、setter、构造方法。
- `spring-boot-starter-validation`：支持 `@Valid`、`@NotBlank`、`@NotNull` 参数校验。

### `README.md`

项目启动说明和学习路线。你忘记怎么建库、启动、测试接口时，先看这个文件。

### `.gitignore`

告诉 Git 哪些文件不需要提交，比如 `target/`、IDE 配置、日志文件。

## `embedded/esp32-device` 目录

这是 ESP32 端 PlatformIO 项目。

重要文件：

- `platformio.ini`：PlatformIO 配置，声明 ESP32 开发板、Arduino 框架和依赖库。
- `src/config.h`：Wi-Fi、后端 IP、引脚、上传间隔等配置。
- `src/main.cpp`：ESP32 主程序，负责读取传感器、上传数据、轮询控制命令。
- `src/sensors/`：DHT22、BH1750、MQ-2 传感器模块。
- `src/communication/`：HTTP 请求封装。
- `src/network/`：Wi-Fi 连接和断线重连。
- `src/actuators/`：LED、蜂鸣器、继电器控制。
- `src/display/`：OLED 显示。

这个目录要用 VS Code + PlatformIO 打开，而不是用 IDEA 当 Java 项目打开。

## `docs` 目录

### `docs/api.http`

接口测试文件。IDEA 可以直接打开它，点击绿色三角发送 HTTP 请求。

它适合用来测试：

- 上传传感器数据
- 查询最新数据
- 查询历史数据
- 创建控制命令
- ESP32 拉取命令
- 命令 ack
- 查询和处理报警

### `docs/project-files.md`

就是当前这份文档，用来讲解项目文件作用。

## `src/main/resources` 目录

### `application.yml`

Spring Boot 配置文件。

主要配置：

- `server.port`：后端端口，当前是 `8080`。
- `spring.datasource`：MySQL 连接地址、用户名、密码。
- `mybatis-plus`：MyBatis-Plus 配置。
- `iot.alarm`：报警阈值配置。
- `iot.device.online-timeout-seconds`：设备在线判断时间窗口。

### `schema.sql`

数据库建表脚本。第一次运行项目前，需要在 MySQL 中执行它。

它会创建：

- `device`：设备表。
- `sensor_data`：传感器数据表。
- `device_command`：控制命令表。
- `alarm_record`：报警记录表。

## `com.example.iot`

### `IotApplication.java`

Spring Boot 启动类。运行它的 `main` 方法，整个后端服务就会启动。

关键注解：

- `@SpringBootApplication`：表示这是 Spring Boot 项目入口。
- `@MapperScan("com.example.iot.mapper")`：告诉 MyBatis-Plus 去哪里找 Mapper。

## `common` 公共目录

### `Result.java`

统一接口返回格式。

所有接口尽量都返回这种结构：

```json
{
  "code": 200,
  "message": "success",
  "data": {}
}
```

这样前端、ESP32、Apifox 看到的响应格式更稳定。

### `GlobalExceptionHandler.java`

全局异常处理器。

作用是把 Java 异常转换成统一 JSON 响应，避免接口报错时直接返回一大段默认错误页面。

例如设备不存在时，代码抛出：

```java
throw new IllegalArgumentException("device not found");
```

它会被包装成：

```json
{
  "code": 400,
  "message": "device not found",
  "data": null
}
```

## `config` 配置目录

### `WebConfig.java`

Web 相关配置。

当前主要配置跨域访问，方便后期前端页面调用后端 `/api/**` 接口。

## `controller` 控制器目录

Controller 是 HTTP 接口入口。浏览器、Apifox、ESP32 请求后端时，首先进入 Controller。

### `DeviceDataController.java`

设备数据接口。

包含：

- `POST /api/device/data`：上传传感器数据。
- `GET /api/device/latest`：查询最新数据。
- `GET /api/device/history`：查询历史数据。
- `GET /api/device/status`：查询设备在线状态。

### `DeviceCommandController.java`

设备控制命令接口。

包含：

- `POST /api/device/control`：网页端创建控制命令。
- `GET /api/device/command`：ESP32 拉取待执行命令。
- `POST /api/device/command/ack`：ESP32 回传命令执行结果。

### `AlarmController.java`

报警接口。

包含：

- `GET /api/alarm/list`：查询报警记录。
- `POST /api/alarm/handle`：标记报警已处理。

## `dto` 请求数据目录

DTO 用来接收请求 JSON。你可以把它理解为“请求参数对象”。

### `SensorDataUploadDTO.java`

接收 ESP32 上传的传感器数据。

对应 JSON：

```json
{
  "deviceId": "esp32_001",
  "temperature": 26.5,
  "humidity": 58.2,
  "light": 320,
  "smoke": 120
}
```

### `DeviceControlDTO.java`

接收网页端下发控制命令的请求。

对应 JSON：

```json
{
  "deviceId": "esp32_001",
  "commandType": "light",
  "commandValue": "on"
}
```

### `CommandAckDTO.java`

接收 ESP32 命令执行结果。

对应 JSON：

```json
{
  "commandId": 1,
  "deviceId": "esp32_001",
  "status": "executed"
}
```

### `AlarmHandleDTO.java`

接收处理报警请求。

对应 JSON：

```json
{
  "alarmId": 1
}
```

## `entity` 实体目录

Entity 对应数据库表。MyBatis-Plus 根据 Entity 操作数据库。

### `Device.java`

对应 `device` 表。

保存设备基础信息和最后在线时间，比如 `deviceId`、`status`、`lastSeen`。

### `SensorData.java`

对应 `sensor_data` 表。

保存每次上传的温度、湿度、光照、烟雾数据。

### `DeviceCommand.java`

对应 `device_command` 表。

保存网页端下发给 ESP32 的命令，包括命令类型、命令值、执行状态。

### `AlarmRecord.java`

对应 `alarm_record` 表。

保存报警类型、报警消息、报警级别、是否处理。

## `mapper` 数据库访问目录

Mapper 是数据库访问入口。它们继承 MyBatis-Plus 的 `BaseMapper` 后，就自动拥有常用 CRUD 方法。

常见方法：

- `insert(entity)`：插入一条数据。
- `selectById(id)`：按主键查询。
- `selectOne(wrapper)`：按条件查询一条。
- `selectList(wrapper)`：按条件查询多条。
- `updateById(entity)`：按主键更新。

### `DeviceMapper.java`

操作 `device` 表。

### `SensorDataMapper.java`

操作 `sensor_data` 表。

### `DeviceCommandMapper.java`

操作 `device_command` 表。

### `AlarmRecordMapper.java`

操作 `alarm_record` 表。

## `service` 业务接口目录

Service 接口定义“这个业务能做什么”，不直接写具体实现。

### `DeviceDataService.java`

定义设备数据相关能力：

- 上传数据
- 查询最新数据
- 查询历史数据
- 查询设备状态

### `DeviceCommandService.java`

定义设备控制命令相关能力：

- 创建命令
- 获取 pending 命令
- 确认命令执行结果

### `AlarmService.java`

定义报警相关能力：

- 检查并创建报警
- 查询报警列表
- 处理报警

## `service/impl` 业务实现目录

ServiceImpl 是真正写业务逻辑的地方。

### `DeviceDataServiceImpl.java`

设备数据核心业务实现。

上传数据时做了这些事：

1. 把 DTO 转成 `SensorData` Entity。
2. 调用 `sensorDataMapper.insert()` 保存传感器数据。
3. 更新或创建设备记录，刷新 `lastSeen`。
4. 调用报警服务检查是否超阈值。
5. 返回 `SensorDataVO`。

查询最新数据时：

1. 按 `deviceId` 筛选。
2. 按 `createdAt` 倒序。
3. `LIMIT 1` 只取最新一条。

设备在线判断时：

```text
当前时间 - lastSeen <= 30 秒：在线
当前时间 - lastSeen > 30 秒：离线
```

### `DeviceCommandServiceImpl.java`

设备控制命令业务实现。

网页端创建命令时：

```text
插入一条 status = pending 的命令
```

ESP32 拉取命令时：

```text
查询最早的一条 pending 命令
```

ESP32 ack 时：

```text
根据 commandId 找到命令
校验 deviceId
更新 status 和 executedAt
```

### `AlarmServiceImpl.java`

报警业务实现。

每次上传数据后，根据配置的阈值判断：

- 温度过高
- 湿度过高
- 烟雾过高
- 光照过低

如果触发规则，就插入一条 `alarm_record`。

## `vo` 返回数据目录

VO 用来组织接口返回数据。它和 Entity 不完全一样，因为数据库字段不一定都适合直接返回给前端或 ESP32。

### `SensorDataVO.java`

上传数据、查询最新数据、查询历史数据时返回。

### `DeviceStatusVO.java`

设备在线状态接口返回。

包含：

- `deviceId`
- `status`
- `online`
- `lastSeen`

### `CommandVO.java`

ESP32 拉取命令时返回。

包含：

- `commandId`
- `commandType`
- `commandValue`

## `src/main/resources/static` 前端页面目录

### `dashboard.html`

前端监控页面入口。后端启动后访问：

```text
http://localhost:8080/dashboard.html
```

### `dashboard.css`

页面样式文件，控制布局、颜色、卡片和响应式显示。

### `dashboard.js`

页面逻辑文件。它会定时请求后端接口，动态刷新：

- 最新传感器数据
- 数据采集时间
- 页面刷新时间
- 设备在线状态
- 历史曲线
- 报警记录
- 远程控制按钮状态

## 建议阅读顺序

如果你是从 0 学 Java 后端，建议这样读：

1. `docs/api.http`
2. `DeviceDataController.java`
3. `SensorDataUploadDTO.java`
4. `DeviceDataService.java`
5. `DeviceDataServiceImpl.java`
6. `SensorData.java`
7. `SensorDataMapper.java`
8. `schema.sql`

先把“上传数据”和“查询最新数据”这条主线吃透，再看控制命令和报警模块。
