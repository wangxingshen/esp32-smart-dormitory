package com.example.iot.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@TableName("device_command")
public class DeviceCommand {

    // device_command 表保存网页端下发给 ESP32 的控制命令。
    @TableId(type = IdType.AUTO)
    private Long id;

    private String deviceId;

    private String commandType;

    private String commandValue;

    private String status;

    private LocalDateTime createdAt;

    private LocalDateTime executedAt;
}
