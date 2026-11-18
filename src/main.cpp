#include <Arduino.h>
#include <ModbusRTU.h>

#include "config.h"

ModbusRTU modbus_slave;

void setup() {
  Serial.begin(DEBUG_BAUD);

  Serial2.begin(MODBUS_BAUD, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);

  modbus_slave.begin(&Serial2);
  modbus_slave.slave(SLAVE_ID);
  // 11 dB dæmpning giver det største målbare område på ADC1
  analogSetPinAttenuation(ADC_INPUT_PIN, ADC_11db);

  modbus_slave.addHreg(ADC_MILLIVOLTS_REGISTER, 0);
  modbus_slave.addHreg(UPTIME_SECONDS_REGISTER, 0);

  Serial.println("Modbus RTU slave er klar");
}

void loop() {
  static uint32_t lastAdcUpdate = 0;
  static uint32_t lastUptimeUpdate = 0;
  static uint32_t lastHeartbeat = 0;
  const uint32_t nowMilliseconds = millis();

  // Kalibreret spænding i millivolt; en måling sker uden at blokere Modbus-svar
  if (nowMilliseconds - lastAdcUpdate >= ADC_UPDATE_MILLISECONDS) {
    lastAdcUpdate = nowMilliseconds;
    modbus_slave.Hreg(ADC_MILLIVOLTS_REGISTER, static_cast<uint16_t>(analogReadMilliVolts(ADC_INPUT_PIN)));
  }

  // Uptime i sekunder; tælleren løber rundt efter 65535 s
  if (nowMilliseconds - lastUptimeUpdate >= UPTIME_UPDATE_MILLISECONDS) {
    lastUptimeUpdate = nowMilliseconds;
    modbus_slave.Hreg(UPTIME_SECONDS_REGISTER, static_cast<uint16_t>(nowMilliseconds / 1000));
  }

  if (nowMilliseconds - lastHeartbeat >= HEARTBEAT_LOG_MILLISECONDS) {
    lastHeartbeat = nowMilliseconds;
    Serial.printf("Slave %u kører, uptime %lu s, ADC %u mV\n", SLAVE_ID, nowMilliseconds / 1000,
                  modbus_slave.Hreg(ADC_MILLIVOLTS_REGISTER));
  }

  modbus_slave.task();
  yield();
}
