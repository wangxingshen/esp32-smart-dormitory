package com.example.iot;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@MapperScan("com.example.iot.mapper")
@SpringBootApplication
public class IotApplication {

    public static void main(String[] args) {
        // Spring Boot 项目入口：运行这个 main 方法后，后端服务会监听 application.yml 中配置的端口。
        SpringApplication.run(IotApplication.class, args);
    }
}
