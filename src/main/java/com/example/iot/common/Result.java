package com.example.iot.common;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class Result<T> {

    // 统一响应格式：前端、ESP32、Apifox 收到的 JSON 都保持 code/message/data 结构。
    private Integer code;
    private String message;
    private T data;

    // T 是泛型，表示 data 可以是传感器数据、命令数据、报警列表等任意类型。
    public static <T> Result<T> success(T data) {
        return new Result<>(200, "success", data);
    }

    // 用在不需要返回具体数据的成功操作，比如命令 ack、处理报警。
    public static Result<Void> success() {
        return new Result<>(200, "success", null);
    }

    public static <T> Result<T> fail(String message) {
        return new Result<>(500, message, null);
    }

    public static <T> Result<T> fail(Integer code, String message) {
        return new Result<>(code, message, null);
    }
}
