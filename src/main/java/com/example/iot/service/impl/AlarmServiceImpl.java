package com.example.iot.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.example.iot.dto.AlarmHandleDTO;
import com.example.iot.entity.AlarmRecord;
import com.example.iot.entity.SensorData;
import com.example.iot.mapper.AlarmRecordMapper;
import com.example.iot.service.AlarmService;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.List;

@Service
@RequiredArgsConstructor
public class AlarmServiceImpl implements AlarmService {

    private final AlarmRecordMapper alarmRecordMapper;

    // @Value 从 application.yml 中读取报警阈值，后期改阈值不需要改 Java 代码。
    @Value("${iot.alarm.temperature-high:30}")
    private double temperatureHigh;

    @Value("${iot.alarm.humidity-high:80}")
    private double humidityHigh;

    @Value("${iot.alarm.smoke-high:300}")
    private double smokeHigh;

    @Value("${iot.alarm.light-low:100}")
    private double lightLow;

    @Override
    public void checkAndCreateAlarms(SensorData sensorData) {
        // 每条传感器数据上传后都会进入这里，根据当前数值判断是否生成报警记录。
        if (sensorData.getTemperature() != null && sensorData.getTemperature() > temperatureHigh) {
            create(sensorData.getDeviceId(), "temperature", "temperature is too high", "warning");
        }
        if (sensorData.getHumidity() != null && sensorData.getHumidity() > humidityHigh) {
            create(sensorData.getDeviceId(), "humidity", "humidity is too high", "warning");
        }
        if (sensorData.getSmoke() != null && sensorData.getSmoke() > smokeHigh) {
            create(sensorData.getDeviceId(), "smoke", "smoke value is too high", "danger");
        }
        if (sensorData.getLight() != null && sensorData.getLight() < lightLow) {
            create(sensorData.getDeviceId(), "light", "light is too low", "info");
        }
    }

    @Override
    public List<AlarmRecord> list(Boolean handled) {
        // handled 为空时不加处理状态条件；不为空时按已处理/未处理过滤。
        LambdaQueryWrapper<AlarmRecord> wrapper = new LambdaQueryWrapper<AlarmRecord>()
                .orderByDesc(AlarmRecord::getCreatedAt);
        if (handled != null) {
            wrapper.eq(AlarmRecord::getIsHandled, handled);
        }
        return alarmRecordMapper.selectList(wrapper);
    }

    @Override
    public void handle(AlarmHandleDTO handleDTO) {
        AlarmRecord alarmRecord = alarmRecordMapper.selectById(handleDTO.getAlarmId());
        if (alarmRecord == null) {
            throw new IllegalArgumentException("alarm not found");
        }
        alarmRecord.setIsHandled(true);
        alarmRecordMapper.updateById(alarmRecord);
    }

    private void create(String deviceId, String alarmType, String alarmMessage, String alarmLevel) {
        // 报警记录默认是未处理，前端或用户处理后再改为 true。
        AlarmRecord alarmRecord = new AlarmRecord();
        alarmRecord.setDeviceId(deviceId);
        alarmRecord.setAlarmType(alarmType);
        alarmRecord.setAlarmMessage(alarmMessage);
        alarmRecord.setAlarmLevel(alarmLevel);
        alarmRecord.setIsHandled(false);
        alarmRecord.setCreatedAt(LocalDateTime.now());
        alarmRecordMapper.insert(alarmRecord);
    }
}
