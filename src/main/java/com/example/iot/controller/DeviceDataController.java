package com.example.iot.controller;

import com.example.iot.common.Result;
import com.example.iot.dto.SensorDataUploadDTO;
import com.example.iot.service.DeviceDataService;
import com.example.iot.vo.DeviceStatusVO;
import com.example.iot.vo.SensorDataVO;
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

import java.util.List;

@Validated
@RestController
@RequiredArgsConstructor
@RequestMapping("/api/device")
public class DeviceDataController {

    private final DeviceDataService deviceDataService;

    // ESP32 上传温湿度、光照、烟雾数据。JSON 请求体会被 @RequestBody 转成 SensorDataUploadDTO。
    @PostMapping("/data")
    public Result<SensorDataVO> upload(@Valid @RequestBody SensorDataUploadDTO uploadDTO) {
        return Result.success(deviceDataService.upload(uploadDTO));
    }

    // 查询某个设备最新一条传感器数据，deviceId 来自 URL 查询参数。
    @GetMapping("/latest")
    public Result<SensorDataVO> latest(@RequestParam @NotBlank String deviceId) {
        return Result.success(deviceDataService.latest(deviceId));
    }

    // 查询历史数据，limit 用来限制返回条数，避免一次返回太多数据。
    @GetMapping("/history")
    public Result<List<SensorDataVO>> history(@RequestParam @NotBlank String deviceId,
                                              @RequestParam(required = false, defaultValue = "100") Integer limit) {
        return Result.success(deviceDataService.history(deviceId, limit));
    }

    // 根据 device.last_seen 判断在线状态：最近 30 秒上传过数据就认为在线。
    @GetMapping("/status")
    public Result<DeviceStatusVO> status(@RequestParam @NotBlank String deviceId) {
        return Result.success(deviceDataService.status(deviceId));
    }
}
