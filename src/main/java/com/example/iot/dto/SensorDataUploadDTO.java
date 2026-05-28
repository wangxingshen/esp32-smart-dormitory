package com.example.iot.dto;

import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
public class SensorDataUploadDTO {

    // DTO 用来接收请求 JSON，字段名要和 ESP32 发来的 JSON key 保持一致。
    @NotBlank(message = "deviceId cannot be blank")
    private String deviceId;

    private Double temperature;

    private Double humidity;

    private Double light;

    private Double smoke;
}
