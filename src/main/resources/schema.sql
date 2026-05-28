CREATE DATABASE IF NOT EXISTS iot_dormitory DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

USE iot_dormitory;

CREATE TABLE IF NOT EXISTS device (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    device_id VARCHAR(64) UNIQUE NOT NULL,
    device_name VARCHAR(100),
    location VARCHAR(100),
    status VARCHAR(20) DEFAULT 'offline',
    last_seen DATETIME,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS sensor_data (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    device_id VARCHAR(64) NOT NULL,
    temperature DOUBLE,
    humidity DOUBLE,
    light DOUBLE,
    smoke DOUBLE,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS device_command (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    device_id VARCHAR(64) NOT NULL,
    command_type VARCHAR(50) NOT NULL,
    command_value VARCHAR(50) NOT NULL,
    status VARCHAR(20) DEFAULT 'pending',
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    executed_at DATETIME
);

CREATE TABLE IF NOT EXISTS alarm_record (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    device_id VARCHAR(64) NOT NULL,
    alarm_type VARCHAR(50),
    alarm_message VARCHAR(255),
    alarm_level VARCHAR(20),
    is_handled TINYINT DEFAULT 0,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
