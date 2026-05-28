package com.example.iot.service;

import com.example.iot.dto.AlarmHandleDTO;
import com.example.iot.entity.AlarmRecord;
import com.example.iot.entity.SensorData;

import java.util.List;

public interface AlarmService {

    // 每次上传传感器数据后，检查是否触发报警规则。
    void checkAndCreateAlarms(SensorData sensorData);

    // 查询报警记录，handled 为 null 时查询全部。
    List<AlarmRecord> list(Boolean handled);

    // 标记报警已处理。
    void handle(AlarmHandleDTO handleDTO);
}
