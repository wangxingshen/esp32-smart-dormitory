package com.example.iot.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@TableName("sensor_data")
public class SensorData {

    // sensor_data 表保存 ESP32 每次上传的一条环境数据。
    @TableId(type = IdType.AUTO)
    private Long id;

    private String deviceId;

    private Double temperature;

    private Double humidity;

    private Double light;

    private Double smoke;

    private LocalDateTime createdAt;
}
