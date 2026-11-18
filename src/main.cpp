#include <Arduino.h>
#include <ModbusRTU.h>

#include "config.h"

ModbusRTU mb;

void setup() {
  Serial.begin(DEBUG_BAUD);

  Serial2.begin(MODBUS_BAUD, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);

  mb.begin(&Serial2);
  mb.slave(SLAVE_ID);
  mb.addHreg(REG_TEST_VALUE, TEST_VALUE);
  mb.addHreg(REG_UPTIME_S, 0);

  Serial.println("Modbus RTU slave er klar");
}

void loop() {
  static uint32_t lastUptimeUpdate = 0;
  static uint32_t lastHeartbeat = 0;
  const uint32_t now = millis();

  // Uptime i sekunder; tælleren løber rundt efter 65535 s
  if (now - lastUptimeUpdate >= UPTIME_UPDATE_MS) {
    lastUptimeUpdate = now;
    mb.Hreg(REG_UPTIME_S, static_cast<uint16_t>(now / 1000));
  }

  if (now - lastHeartbeat >= HEARTBEAT_LOG_MS) {
    lastHeartbeat = now;
    Serial.printf("Slave %u kører, uptime %lu s\n", SLAVE_ID, now / 1000);
  }

  mb.task();
  yield();
}
