#pragma once
#include <Arduino.h>

// HW-519 exception wiring: TXD -> GPIO16 and RXD -> GPIO17.
constexpr int RS485_RX_PIN = 17;
constexpr int RS485_TX_PIN = 16;

constexpr uint32_t DEBUG_BAUD = 115200;
constexpr uint32_t MODBUS_BAUD = 9600;  // 9600 8N1 i alle RTU-deltagere
constexpr uint8_t SLAVE_ID = 1;

// Registermap (holding registers, nulbaseret offset)
constexpr uint16_t REG_TEST_VALUE = 0;
constexpr uint16_t REG_UPTIME_S = 1;
constexpr uint16_t TEST_VALUE = 1234;

constexpr uint32_t UPTIME_UPDATE_MS = 1000;
constexpr uint32_t HEARTBEAT_LOG_MS = 10000;
