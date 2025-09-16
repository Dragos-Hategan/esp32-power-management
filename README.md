# ESP32 Power Management

This repository demonstrates how to use **ESP-IDF power management features** on the ESP32, focusing on CPU frequency scaling, light sleep, and deep sleep. The project provides simple routines to observe how current consumption changes across modes.

## Features

- **Active mode frequency scaling**  
  Dynamically reconfigure CPU frequency between 80, 160, and 240 MHz, and measure the effect on current consumption.

- **Light sleep**  
  Enter light sleep a timer wake-up source. The CPU clock is gated while RAM and peripherals are retained.

- **Deep sleep**  
  Enter deep sleep a timer wake-up source. The chip consumes the lowest current but always resets on wake-up.

- **LED feedback**  
  The on-board LED blinks before entering sleep to provide visible status.

---

## Photos

### Console Output
![console](/docs/Console.png)

### Hardware Setup
![setup](/docs/Setup.jpg)

---

## Why DevKit Consumption Is Higher Than Module Specs

The current values measured on the **ESP32 DevKit** are higher than those in the official Espressif datasheet for the bare ESP32-WROOM module. This is because the DevKit board includes extra circuitry that draws current even when the ESP32 core is in deep sleep:

- **USB–UART bridge (CH340/CP2102)**: always powered depending on what power rail it is connected to.  
- **LDO regulator (5V → 3.3V)**: adds quiescent current and conversion losses.  
- **On-board LEDs**: the power LED alone draws ~1 mA continuously.  
- **USB protection & support circuitry**: small but non-negligible leakage currents.  

As a result, while the **bare ESP32-WROOM module** can reach deep sleep currents as low as **~10 µA**, the **DevKit** typically stays in the **mA range**. For ultra-low power applications, it is recommended to design with the bare module or a custom PCB.

---

## ESP32 Devkit Current Consumption

Measurements of ESP32 power consumption in different modes, with radio disabled:

| Mode                        | Frequency | Wake-up Source | Current (mA) |
|-----------------------------|-----------|----------------|--------------|
| Active Loop                 | 80 MHz    | –              | 35.1         |
| Active Loop                 | 160 MHz   | –              | 40.4         |
| Active Loop                 | 240 MHz   | –              | 51.9         |
| Active (flash erased, idle) | 160 MHz   | –              | 31.96        |
| Light Sleep                 | 160 MHz   | Timer          | 12.3         |
| Deep Sleep                  | 160 MHz   | Timer          | 10.8         |

---

## File Overview

- `frequency.c / frequency.h`  
  Demonstrates CPU frequency scaling using ESP-IDF power management (DFS) and prints measured clock rates.

- `sleep_modes.c / sleep_modes.h`  
  Demonstrates active, light sleep, and deep sleep modes with wake-up source (timer).

