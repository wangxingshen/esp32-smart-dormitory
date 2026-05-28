package com.example.iot.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.example.iot.dto.CommandAckDTO;
import com.example.iot.dto.DeviceControlDTO;
import com.example.iot.entity.DeviceCommand;
import com.example.iot.mapper.DeviceCommandMapper;
import com.example.iot.service.DeviceCommandService;
import com.example.iot.vo.CommandVO;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;

@Service
@RequiredArgsConstructor
public class DeviceCommandServiceImpl implements DeviceCommandService {

    private final DeviceCommandMapper deviceCommandMapper;

    @Override
    public CommandVO createCommand(DeviceControlDTO controlDTO) {
        // 网页端不会直接控制 ESP32，而是先在数据库里生成一条 pending 命令。
        DeviceCommand command = new DeviceCommand();
        command.setDeviceId(controlDTO.getDeviceId());
        command.setCommandType(controlDTO.getCommandType());
        command.setCommandValue(controlDTO.getCommandValue());
        command.setStatus("pending");
        command.setCreatedAt(LocalDateTime.now());
        deviceCommandMapper.insert(command);
        return toCommandVO(command);
    }

    @Override
    public CommandVO getPendingCommand(String deviceId) {
        // ESP32 每隔一段时间轮询，取最早创建的一条 pending 命令执行。
        DeviceCommand command = deviceCommandMapper.selectOne(new LambdaQueryWrapper<DeviceCommand>()
                .eq(DeviceCommand::getDeviceId, deviceId)
                .eq(DeviceCommand::getStatus, "pending")
                .orderByAsc(DeviceCommand::getCreatedAt)
                .last("LIMIT 1"));
        if (command == null) {
            return null;
        }
        return toCommandVO(command);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void ack(CommandAckDTO ackDTO) {
        // 根据 commandId 查命令，同时校验 deviceId，避免设备误确认别人的命令。
        DeviceCommand command = deviceCommandMapper.selectById(ackDTO.getCommandId());
        if (command == null || !command.getDeviceId().equals(ackDTO.getDeviceId())) {
            throw new IllegalArgumentException("command not found");
        }

        command.setStatus(ackDTO.getStatus());
        command.setExecutedAt(LocalDateTime.now());
        deviceCommandMapper.updateById(command);
    }

    private CommandVO toCommandVO(DeviceCommand command) {
        // ESP32 只需要命令 id、类型和值，不需要数据库里的 createdAt 等字段。
        CommandVO commandVO = new CommandVO();
        commandVO.setCommandId(command.getId());
        commandVO.setCommandType(command.getCommandType());
        commandVO.setCommandValue(command.getCommandValue());
        return commandVO;
    }
}
