package com.example.iot.controller;

import com.example.iot.common.Result;
import com.example.iot.dto.AlarmHandleDTO;
import com.example.iot.entity.AlarmRecord;
import com.example.iot.service.AlarmService;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequiredArgsConstructor
@RequestMapping("/api/alarm")
public class AlarmController {

    private final AlarmService alarmService;

    // 查询报警记录；handled 不传表示查询全部，传 false 表示只看未处理报警。
    @GetMapping("/list")
    public Result<List<AlarmRecord>> list(@RequestParam(required = false) Boolean handled) {
        return Result.success(alarmService.list(handled));
    }

    // 将某条报警标记为已处理。
    @PostMapping("/handle")
    public Result<Void> handle(@Valid @RequestBody AlarmHandleDTO handleDTO) {
        alarmService.handle(handleDTO);
        return Result.success();
    }
}
