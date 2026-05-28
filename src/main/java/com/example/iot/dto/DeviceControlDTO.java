package com.example.iot.dto;

import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
public class DeviceControlDTO {

    // 网页端发起控制请求时使用，例如 commandType=light，commandValue=on。
    @NotBlank(message = "deviceId cannot be blank")
    private String deviceId;

    @NotBlank(message = "commandType cannot be blank")
    private String commandType;

    @NotBlank(message = "commandValue cannot be blank")
    private String commandValue;
}
