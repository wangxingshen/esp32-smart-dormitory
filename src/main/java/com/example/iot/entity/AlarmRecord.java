package com.example.iot.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@TableName("alarm_record")
public class AlarmRecord {

    // alarm_record 表保存由传感器阈值触发的报警记录。
    @TableId(type = IdType.AUTO)
    private Long id;

    private String deviceId;

    private String alarmType;

    private String alarmMessage;

    private String alarmLevel;

    private Boolean isHandled;

    private LocalDateTime createdAt;
}
