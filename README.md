<!-- _Formål: Modbus RTU-slave på ESP32 WROOM, som leverer en ADC-aflæsning og uptime til firmwaren i zephyr-3sem-project. -->

# Modbus RTU-slave (ESP32 WROOM + HW-519)

ESP32'en er Modbus RTU-slave med slave-ID `1` på 9600 8N1. Den er den eksterne enhed, som Zephyr-firmwaren i `zephyr-3sem-project` (ESP32-C6) læser over RS485. Dokumentationen af hele projektet ligger i workspace i `projects_documentation/_code-projects/zephyr-3sem-project/` (se `architecture/architecture.md`, afsnittet "Modbus-slaven").

## Registre

Registrene er en kontrakt med firmwaren. Ændres indekser, slave-ID eller baudrate her, skal `src/modbus_reader.c` i `zephyr-3sem-project` ændres samtidig.

| Holding register | Indhold                                    | Opdateres    |
| ---------------- | ------------------------------------------ | ------------ |
| 0                | ADC-aflæsning i millivolt                  | hvert sekund |
| 1                | Uptime i sekunder (løber rundt efter 65535) | hvert sekund |

## ADC

Et potentiometer på ADC1-kanal 6 (GPIO34, kun indgang): yderben til 3,3 V og GND, midterben til GPIO34. Aflæsningen sker med `analogReadMilliVolts()` og 11 dB dæmpning.

ADC1 er valgt, fordi ADC2 deler hardware med Wi-Fi-radioen, og fordi flere af ADC2's ben er strapping-pins. Med 11 dB er det målbare område 150 til 2450 mV (Espressif, ESP-IDF Programming Guide v4.4, ESP32 ADC), så værdier over cirka 2450 mV er upræcise, når potentiometeret forsynes fra 3,3 V. Slaven kontrollerer ikke aflæsningen og tager hverken middelværdi eller filtrerer den.

## Hardware

Board `esp32dev` og en HW-519 RS485-adapter. Pins står i `include/config.h`.

| ESP32             | HW-519 |
| ----------------- | ------ |
| GPIO16 (UART2 TX) | `TXD`  |
| GPIO17 (UART2 RX) | `RXD`  |
| 5 V               | `VCC`  |
| GND               | `GND`  |

HW-519 har automatisk retning, så der er ingen DE/RE-pin. Kontrollér kortets silketryk, før strøm tilsluttes. ESP32'ens RX må ikke udsættes for 5 V.

## Build og test

Byg med PlatformIO: `pio run`, og upload med `pio run -t upload`. Seriel monitor kører ved 115200 og skriver `Modbus RTU slave er klar`, derefter en linje hvert 10. sekund med uptime og ADC-værdi.

Test med en Modbus RTU-master, for eksempel QModMaster: RTU, 9600, 8, None, 1, slave-ID 1, funktion 03, startadresse 0, antal 2. Register 0 skal følge potentiometeret, og register 1 skal tælle op. Testscriptet fra fase 1 i `esp32-modbus-factoryio` kontrollerer register 0 mod `1234` og passer ikke længere.

## Status

Koden bygger. Den er ikke kørt på hardware, så pin, dæmpning og skalering af ADC-aflæsningen er ikke verificeret.
