# 📱 Mobile Interface (MVP Strategy)

## Why Telegram?
For the Hackathon MVP, we optimized for **speed and cross-platform reliability**. Instead of building a custom Flutter/React Native app that requires installation, we integrated a **Telegram Bot API** directly into the ESP32.

### Advantages:
1. **Zero-Friction Install**: Caregivers just scan a QR code to join the alert channel.
2. **Instant Push Notifications**: Bypasses Android/iOS background app limitations.
3. **Low Latency**: Direct ESP32 -> Telegram Webhook communication via `WiFiClientSecure`.

## Future Scope
Post-hackathon, we plan to transition this into a full Flutter application (Dart) with historical seizure tracking and data visualization.
