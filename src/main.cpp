#include <Arduino.h>
#include <ModbusRTU.h>

#include "config.h"

ModbusRTU mb;

void setup() {
  Serial.begin(DEBUG_BAUD);

  Serial2.begin(MODBUS_BAUD, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);

  mb.begin(&Serial2);
  mb.slave(SLAVE_ID);
  // 11 dB dæmpning giver det største målbare område på ADC1
  analogSetPinAttenuation(ADC_INPUT_PIN, ADC_11db);

  mb.addHreg(REG_ADC_MILLIVOLTS, 0);
  mb.addHreg(REG_UPTIME_S, 0);

  Serial.println("Modbus RTU slave er klar");
}

void loop() {
  static uint32_t lastAdcUpdate = 0;
  static uint32_t lastUptimeUpdate = 0;
  static uint32_t lastHeartbeat = 0;
  const uint32_t now = millis();

  // Kalibreret spænding i millivolt; en måling sker uden at blokere Modbus-svar
  if (now - lastAdcUpdate >= ADC_UPDATE_MILLISECONDS) {
    lastAdcUpdate = now;
    mb.Hreg(REG_ADC_MILLIVOLTS, static_cast<uint16_t>(analogReadMilliVolts(ADC_INPUT_PIN)));
  }

  // Uptime i sekunder; tælleren løber rundt efter 65535 s
  if (now - lastUptimeUpdate >= UPTIME_UPDATE_MS) {
    lastUptimeUpdate = now;
    mb.Hreg(REG_UPTIME_S, static_cast<uint16_t>(now / 1000));
  }

  if (now - lastHeartbeat >= HEARTBEAT_LOG_MS) {
    lastHeartbeat = now;
    Serial.printf("Slave %u kører, uptime %lu s, ADC %u mV\n", SLAVE_ID, now / 1000,
                  mb.Hreg(REG_ADC_MILLIVOLTS));
  }

  mb.task();
  yield();
}
