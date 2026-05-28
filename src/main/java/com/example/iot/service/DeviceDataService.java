package com.example.iot.service;

import com.example.iot.dto.SensorDataUploadDTO;
import com.example.iot.vo.DeviceStatusVO;
import com.example.iot.vo.SensorDataVO;

import java.util.List;

public interface DeviceDataService {

    // 上传数据并保存到数据库。
    SensorDataVO upload(SensorDataUploadDTO uploadDTO);

    // 查询最新一条传感器数据。
    SensorDataVO latest(String deviceId);

    // 查询最近 N 条历史数据。
    List<SensorDataVO> history(String deviceId, Integer limit);

    // 查询设备在线/离线状态。
    DeviceStatusVO status(String deviceId);
}
