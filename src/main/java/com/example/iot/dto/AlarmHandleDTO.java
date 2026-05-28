package com.example.iot.dto;

import jakarta.validation.constraints.NotNull;
import lombok.Data;

@Data
public class AlarmHandleDTO {

    // 前端处理报警时只需要传报警记录 id。
    @NotNull(message = "alarmId cannot be null")
    private Long alarmId;
}
