# 🔌 Hardware Wiring Guide

## Bill of Materials (BOM)
- 1x ESP32 Microcontroller
- 1x MPU6050 (Accelerometer & Gyroscope)
- 1x Active Buzzer
- 1x Push Button (with pull-up configuration)
- 1x LED (Status Indicator)

## Pin Mapping
| Component | ESP32 Pin | Notes |
|-----------|-----------|-------|
| MPU6050 SDA | GPIO 5 | I2C Data |
| MPU6050 SCL | GPIO 6 | I2C Clock |
| Buzzer VCC | GPIO 4 | Active HIGH |
| Button | GPIO 8 | Uses internal `INPUT_PULLUP` |
| LED Anode | GPIO 3 | Status indicator |

*Note: Power saving is enabled. CPU frequency is downclocked to 80MHz to preserve battery life during continuous monitoring.*
