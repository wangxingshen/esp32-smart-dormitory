package com.example.iot.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@TableName("device")
public class Device {

    // Entity 对应数据库表，字段采用驼峰命名，MyBatis-Plus 会映射到下划线字段。
    @TableId(type = IdType.AUTO)
    private Long id;

    private String deviceId;

    private String deviceName;

    private String location;

    private String status;

    private LocalDateTime lastSeen;

    private LocalDateTime createdAt;
}
