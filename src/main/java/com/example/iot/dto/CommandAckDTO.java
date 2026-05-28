package com.example.iot.dto;

import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

@Data
public class CommandAckDTO {

    // ESP32 执行完命令后，用 commandId 告诉后端是哪条命令被执行了。
    @NotNull(message = "commandId cannot be null")
    private Long commandId;

    @NotBlank(message = "deviceId cannot be blank")
    private String deviceId;

    @NotBlank(message = "status cannot be blank")
    private String status;
}
