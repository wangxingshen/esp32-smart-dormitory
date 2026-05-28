package com.example.iot.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.example.iot.entity.Device;

// Mapper 是 MyBatis-Plus 操作数据库的入口，继承 BaseMapper 后自带 insert/select/update/delete。
public interface DeviceMapper extends BaseMapper<Device> {
}
