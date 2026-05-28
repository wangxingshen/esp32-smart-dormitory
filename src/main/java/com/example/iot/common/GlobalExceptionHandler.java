package com.example.iot.common;

import jakarta.validation.ConstraintViolationException;
import org.springframework.validation.BindException;
import org.springframework.web.bind.MethodArgumentNotValidException;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;

@RestControllerAdvice
public class GlobalExceptionHandler {

    // 业务参数不合法时走这里，例如设备或命令不存在。
    @ExceptionHandler(IllegalArgumentException.class)
    public Result<Void> handleIllegalArgument(IllegalArgumentException exception) {
        return Result.fail(400, exception.getMessage());
    }

    // @Valid、@NotBlank、@NotNull 这类参数校验失败时走这里。
    @ExceptionHandler({MethodArgumentNotValidException.class, BindException.class, ConstraintViolationException.class})
    public Result<Void> handleValidation(Exception exception) {
        return Result.fail(400, exception.getMessage());
    }

    // 没有被上面捕获的异常统一包装成 JSON，避免浏览器看到一大段默认错误页。
    @ExceptionHandler(Exception.class)
    public Result<Void> handleException(Exception exception) {
        return Result.fail(exception.getMessage());
    }
}
