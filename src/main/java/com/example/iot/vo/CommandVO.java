package com.example.iot.vo;

import lombok.Data;

@Data
public class CommandVO {

    // ESP32 拉取命令接口返回的数据结构。
    private Long commandId;

    private String commandType;

    private String commandValue;
}
