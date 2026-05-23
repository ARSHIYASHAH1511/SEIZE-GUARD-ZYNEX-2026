# 💻 ESP32 Core Code

## Required Arduino Libraries
To compile this code, install the following via the Arduino Library Manager:
1. `MPU6050_tockn` (For optimized accelerometer data)
2. `WiFi` (Built-in ESP32)
3. `HTTPClient` & `WiFiClientSecure` (For Telegram API communication)

## Pin Configuration
- **SDA / SCL**: Pins 5 / 6 (MPU6050 I2C)
- **Buzzer**: Pin 4
- **Cancel Button**: Pin 8
- **Status LED**: Pin 3

