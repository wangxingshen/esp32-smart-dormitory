package com.example.iot.service;

import com.example.iot.dto.CommandAckDTO;
import com.example.iot.dto.DeviceControlDTO;
import com.example.iot.vo.CommandVO;

public interface DeviceCommandService {

    // 网页端创建控制命令。
    CommandVO createCommand(DeviceControlDTO controlDTO);

    // ESP32 获取待执行命令。
    CommandVO getPendingCommand(String deviceId);

    // ESP32 回传执行结果。
    void ack(CommandAckDTO ackDTO);
}
