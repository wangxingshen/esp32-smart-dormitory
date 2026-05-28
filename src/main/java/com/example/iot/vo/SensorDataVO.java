package com.example.iot.vo;

import lombok.Data;

import java.time.LocalDateTime;

@Data
public class SensorDataVO {

    // VO 用来组织接口返回给前端/ESP32 的数据。
    private Long id;

    private String deviceId;

    private Double temperature;

    private Double humidity;

    private Double light;

    private Double smoke;

    private LocalDateTime createdAt;
}
