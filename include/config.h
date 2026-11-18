#pragma once
#include <Arduino.h>

// HW-519 exception wiring: TXD -> GPIO16 and RXD -> GPIO17.
constexpr int RS485_RX_PIN = 17;
constexpr int RS485_TX_PIN = 16;

constexpr uint32_t DEBUG_BAUD = 115200;
constexpr uint32_t MODBUS_BAUD = 9600;  // 9600 8N1 i alle RTU-deltagere
constexpr uint8_t SLAVE_ID = 1;

// Registermap (holding registers, nulbaseret offset)
constexpr uint16_t ADC_MILLIVOLTS_REGISTER = 0;
constexpr uint16_t UPTIME_SECONDS_REGISTER = 1;

// ADC1-kanal 6 (kun indgang). ADC1 er valgt, fordi ADC2 deler hardware med Wi-Fi.
// Med 11 dB dæmpning er det målbare område 150-2450 mV (ESP-IDF v4.4, ESP32 ADC).
constexpr int ADC_INPUT_PIN = 34;
constexpr uint32_t ADC_UPDATE_MILLISECONDS = 1000;

constexpr uint32_t UPTIME_UPDATE_MILLISECONDS = 1000;
constexpr uint32_t HEARTBEAT_LOG_MILLISECONDS = 10000;
