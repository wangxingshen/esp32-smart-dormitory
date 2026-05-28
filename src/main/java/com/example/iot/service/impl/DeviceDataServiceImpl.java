package com.example.iot.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.example.iot.dto.SensorDataUploadDTO;
import com.example.iot.entity.Device;
import com.example.iot.entity.SensorData;
import com.example.iot.mapper.DeviceMapper;
import com.example.iot.mapper.SensorDataMapper;
import com.example.iot.service.AlarmService;
import com.example.iot.service.DeviceDataService;
import com.example.iot.vo.DeviceStatusVO;
import com.example.iot.vo.SensorDataVO;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.Duration;
import java.time.LocalDateTime;
import java.util.List;

@Service
@RequiredArgsConstructor
public class DeviceDataServiceImpl implements DeviceDataService {

    // Mapper 负责访问数据库，Service 负责组织业务流程。
    private final DeviceMapper deviceMapper;
    private final SensorDataMapper sensorDataMapper;
    private final AlarmService alarmService;

    @Value("${iot.device.online-timeout-seconds:30}")
    private long onlineTimeoutSeconds;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public SensorDataVO upload(SensorDataUploadDTO uploadDTO) {
        LocalDateTime now = LocalDateTime.now();

        // DTO 是接口收到的数据，Entity 是准备保存进数据库的数据。
        SensorData sensorData = new SensorData();
        BeanUtils.copyProperties(uploadDTO, sensorData);
        sensorData.setCreatedAt(now);
        sensorDataMapper.insert(sensorData);

        // 上传数据同时刷新设备最后在线时间，并根据阈值判断是否需要生成报警。
        upsertDeviceSeen(uploadDTO.getDeviceId(), now);
        alarmService.checkAndCreateAlarms(sensorData);

        return toSensorDataVO(sensorData);
    }

    @Override
    public SensorDataVO latest(String deviceId) {
        // 按创建时间倒序，只取第一条，就是“最新数据”。
        SensorData sensorData = sensorDataMapper.selectOne(new LambdaQueryWrapper<SensorData>()
                .eq(SensorData::getDeviceId, deviceId)
                .orderByDesc(SensorData::getCreatedAt)
                .last("LIMIT 1"));
        if (sensorData == null) {
            return null;
        }
        return toSensorDataVO(sensorData);
    }

    @Override
    public List<SensorDataVO> history(String deviceId, Integer limit) {
        // 限制 limit 范围，防止前端或 ESP32 一次请求过多历史数据。
        int safeLimit = limit == null ? 100 : Math.min(Math.max(limit, 1), 500);
        return sensorDataMapper.selectList(new LambdaQueryWrapper<SensorData>()
                        .eq(SensorData::getDeviceId, deviceId)
                        .orderByDesc(SensorData::getCreatedAt)
                        .last("LIMIT " + safeLimit))
                .stream()
                .map(this::toSensorDataVO)
                .toList();
    }

    @Override
    public DeviceStatusVO status(String deviceId) {
        Device device = deviceMapper.selectOne(new LambdaQueryWrapper<Device>()
                .eq(Device::getDeviceId, deviceId)
                .last("LIMIT 1"));
        if (device == null) {
            throw new IllegalArgumentException("device not found: " + deviceId);
        }

        // 当前时间 - lastSeen 小于等于配置的秒数，就认为设备在线。
        boolean online = device.getLastSeen() != null
                && Duration.between(device.getLastSeen(), LocalDateTime.now()).getSeconds() <= onlineTimeoutSeconds;

        DeviceStatusVO statusVO = new DeviceStatusVO();
        statusVO.setDeviceId(device.getDeviceId());
        statusVO.setOnline(online);
        statusVO.setStatus(online ? "online" : "offline");
        statusVO.setLastSeen(device.getLastSeen());
        return statusVO;
    }

    private void upsertDeviceSeen(String deviceId, LocalDateTime now) {
        // upsert 的意思是：有就更新，没有就插入。这里用于维护 device 表。
        Device existing = deviceMapper.selectOne(new LambdaQueryWrapper<Device>()
                .eq(Device::getDeviceId, deviceId)
                .last("LIMIT 1"));
        if (existing == null) {
            Device device = new Device();
            device.setDeviceId(deviceId);
            device.setDeviceName(deviceId);
            device.setStatus("online");
            device.setLastSeen(now);
            device.setCreatedAt(now);
            deviceMapper.insert(device);
            return;
        }

        existing.setStatus("online");
        existing.setLastSeen(now);
        deviceMapper.updateById(existing);
    }

    private SensorDataVO toSensorDataVO(SensorData sensorData) {
        // VO 是返回给调用方的数据对象，避免直接把数据库 Entity 暴露出去。
        SensorDataVO sensorDataVO = new SensorDataVO();
        BeanUtils.copyProperties(sensorData, sensorDataVO);
        return sensorDataVO;
    }
}
