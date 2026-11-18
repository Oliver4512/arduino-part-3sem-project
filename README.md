<!-- _Formål: Fase 1 på ESP32-DevKit: Modbus RTU-slave over HW-519, som en PC-master kan læse. -->

# Fase 1: RTU-slave (DevKit + HW-519)

ESP32'en er Modbus RTU-slave med slave-ID `1` på 9600 8N1. Guiden: afsnit "Fase 1: ESP32 som Modbus RTU-slave".

## Registre

| Holding register | Indhold |
| --- | --- |
| 0 | Testværdi `1234` |
| 1 | Uptime i sekunder (løber rundt efter 65535) |

## Hardware

Board `esp32dev`, HW-519 R4 og GmM USB-RS485 på PC-siden. Pins står i `include/config.h`.

| ESP32 | HW-519 |
| --- | --- |
| GPIO16 (UART2 TX) | `TXD` |
| GPIO17 (UART2 RX) | `RXD` |
| 5 V | `VCC` |
| GND | `GND` |

HW-519 har automatisk retning, så der er ingen DE/RE-pin. Kontrollér kortets silketryk før strøm tilsluttes. ESP32'ens RX må ikke udsættes for 5 V.

## Test

1. Upload og åbn seriel monitor ved 115200. Forventet udskrift: `Modbus RTU slave er klar`.
2. Forbind GmM `A`/`B`/`G` til HW-519 `A`/`B`/GND.
3. I QModMaster: RTU, GmM's port, 9600, 8, None, 1, slave-ID 1, funktion 03, startadresse 0, antal 1.
4. Forventet svar: `1234`.

## Fejlfinding

Timeout: kontrollér 9600 8N1, slave-ID `1`, A/B-polaritet, forsyning til HW-519, at pins i `config.h` passer, og fælles GND. Kun ét program må åbne GmM-porten ad gangen.

## Status

Compile-testet og hardwaretestet sammen med `devkit-phase2-rtu-master`: masteren læste register `0` som `1234` over to HW-519-moduler ved `9600 8N1`.
