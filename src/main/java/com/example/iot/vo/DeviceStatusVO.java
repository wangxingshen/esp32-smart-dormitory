package com.example.iot.vo;

import lombok.Data;

import java.time.LocalDateTime;

@Data
public class DeviceStatusVO {

    // 设备状态接口返回的数据结构。
    private String deviceId;

    private String status;

    private Boolean online;

    private LocalDateTime lastSeen;
}
