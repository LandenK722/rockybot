# rockybot

A talking desk companion with a Rocky (Project Hail Mary) personality, built on a
Seeed XIAO ESP32-S3 Sense. The board handles eyes, head motion, microphone and
speaker; a Windows PC runs the conversation over USB serial.

Full build manual (wiring, verification, the twelve stages):
<https://claude.ai/code/artifact/64d20f67-fbff-414d-95ed-01e6e9121029>

## Hardware

| Part | Notes |
| --- | --- |
| XIAO ESP32-S3 Sense | 8 MB flash, 8 MB OPI PSRAM, onboard PDM mic |
| 1.3" SH1106 OLED, 128x64 | One screen, both eyes drawn side by side |
| Adafruit mini pan-tilt | SG90-class analog servos |
| MAX98357A + 4 ohm 3 W speaker | I2S class-D amp, GAIN left unconnected = 9 dB |

Power: the PC's USB powers the XIAO. A separate 5 V 3 A adapter powers the
servos and the amplifier. All grounds are common. External +5 V never touches
the XIAO's 5 V pin — that pad is USB VBUS.

## Pin map

Verified against Seeed's XIAO ESP32-S3 wiki.

| Silkscreen | GPIO | Use |
| --- | --- | --- |
| D0 | 1 | Pan servo signal |
| D1 | 2 | Tilt servo signal |
| D2 | 3 | Free — strapping pin, deliberately left free |
| D3 | 4 | Amp `SD` (mute control) |
| D4 | 5 | OLED `SDA` |
| D5 | 6 | OLED `SCK` |
| D6 | 43 | Amp `BCLK` — also UART0 TX, boot log lands here |
| D7 | 44 | Amp `LRC` |
| D8 | 7 | Amp `DIN` |
| D9 | 8 | Push-to-talk button, `INPUT_PULLUP` to GND |
| D10 | 9 | Free |
| — | 41 / 42 | Onboard PDM mic, data / clock |
| 3V3 | — | OLED `VCC` |

The mic must be on I2S0 — the ESP32-S3's PDM-to-PCM converter exists only there.
The amplifier goes on I2S1. The boot ROM prints to GPIO43 at every reset, which
is why `SD` gets a firmware-controlled mute wire on D3.

## Arduino IDE settings

Arduino ESP32 core 3.x, board `XIAO_ESP32S3`.

| Setting | Value |
| --- | --- |
| USB CDC On Boot | **Enabled** — otherwise Serial goes to GPIO43/44, not USB |
| PSRAM | **OPI PSRAM** — this board is Octal; QSPI fails silently with 0 bytes |
| Flash Size | 8MB (64Mb) |
| Partition Scheme | Default with spiffs |
| Upload Speed | 921600 (drop to 115200 if uploads fail partway) |

## Layout

- `firmware/` — Arduino sketches, one folder per stage.
  - `stage3_bringup/` — LED, serial round-trip, PSRAM check, push-to-talk button.
  - `stage4_i2c_scan/` — I2C bus scan, confirms the OLED's address before drawing.

## Where the build is

Stages 1–3 passed on the bench. Stage 3 confirmed the toolchain, the serial
round-trip and PSRAM reporting 8388608 bytes — the voice buffer in Stage 10
depends on that. Stage 4 (the OLED) is in progress; stages 5–12 are queued. Measured so far: supply 5.46 V unloaded,
polarity correct, no short; amp `SD` reads 0.478 V against 0.492 V predicted, so
the clone matches Adafruit's schematic and the amp boots enabled in (L+R)/2 mode.
Supply under load is not yet measured — that happens at Stage 6.
