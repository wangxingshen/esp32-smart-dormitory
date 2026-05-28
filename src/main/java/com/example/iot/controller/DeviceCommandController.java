package com.example.iot.controller;

import com.example.iot.common.Result;
import com.example.iot.dto.CommandAckDTO;
import com.example.iot.dto.DeviceControlDTO;
import com.example.iot.service.DeviceCommandService;
import com.example.iot.vo.CommandVO;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@Validated
@RestController
@RequiredArgsConstructor
@RequestMapping("/api/device")
public class DeviceCommandController {

    private final DeviceCommandService deviceCommandService;

    // 网页端点击开灯/关灯等按钮时，请求这个接口生成一条 pending 命令。
    @PostMapping("/control")
    public Result<CommandVO> control(@Valid @RequestBody DeviceControlDTO controlDTO) {
        return Result.success(deviceCommandService.createCommand(controlDTO));
    }

    // ESP32 定时轮询这个接口，获取最早的一条 pending 命令。
    @GetMapping("/command")
    public Result<CommandVO> command(@RequestParam @NotBlank String deviceId) {
        return Result.success(deviceCommandService.getPendingCommand(deviceId));
    }

    // ESP32 执行命令后回传结果，把命令状态改成 executed 或其他状态。
    @PostMapping("/command/ack")
    public Result<Void> ack(@Valid @RequestBody CommandAckDTO ackDTO) {
        deviceCommandService.ack(ackDTO);
        return Result.success();
    }
}
