#include <Wire.h>
#include <MPU6050_tockn.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include "esp_wifi.h"

// =====================================================
// MPU6050
// =====================================================

MPU6050 mpu6050(Wire);

// =====================================================
// WIFI
// =====================================================

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// =====================================================
// TELEGRAM
// =====================================================

String botToken = "123456789-ABCDEFGH";
String chatID = "YOUR_CHAT_BOT_ID;

// =====================================================
// PINS
// =====================================================

#define BUZZER_PIN 4
#define BUTTON_PIN 8
#define LED_PIN 3

// =====================================================
// SETTINGS
// =====================================================

float SEIZURE_THRESHOLD = 1.2;

unsigned long CONFIRM_TIME = 500;

unsigned long GRACE_PERIOD = 2700;

// =====================================================
// VARIABLES
// =====================================================

bool seizureDetected = false;
bool alarmActive = false;

unsigned long seizureStartTime = 0;

WiFiClientSecure client;

// LED blinking
unsigned long previousBlink = 0;

bool ledState = false;

// =====================================================
// WIFI CONNECT
// =====================================================

void connectWiFi()
{
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
    }
}

// =====================================================
// WIFI OFF
// =====================================================

void disconnectWiFi()
{
    WiFi.disconnect(true);

    WiFi.mode(WIFI_OFF);

    esp_wifi_stop();
}

// =====================================================
// TELEGRAM FUNCTION
// =====================================================

void sendTelegramMessage(String message)
{
    connectWiFi();

    client.setInsecure();

    HTTPClient https;

    String url =
        "https://api.telegram.org/bot" +
        botToken +
        "/sendMessage?chat_id=" +
        chatID +
        "&text=" +
        message;

    if (https.begin(client, url))
    {
        int httpCode = https.GET();

        https.end();
    }

    disconnectWiFi();
}

// =====================================================
// SETUP
// =====================================================

void setup()
{
    // Lower CPU frequency for power saving
    setCpuFrequencyMhz(80);

    Serial.begin(115200);

    delay(500);

    // =================================================
    // PIN SETUP
    // =================================================

    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    pinMode(LED_PIN, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);

    digitalWrite(LED_PIN, LOW);

    // =================================================
    // MPU6050
    // =================================================

    Wire.begin(5, 6);

    mpu6050.begin();

    mpu6050.calcGyroOffsets(true);

    // =================================================
    // WIFI OFF INITIALLY
    // =================================================

    disconnectWiFi();
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
    // =================================================
    // UPDATE MPU
    // =================================================

    mpu6050.update();

    float accX = mpu6050.getAccX();

    float accY = mpu6050.getAccY();

    float accZ = mpu6050.getAccZ();

    // =================================================
    // MAGNITUDE
    // =================================================

    float magnitude = sqrt(
        (accX * accX) +
        (accY * accY) +
        (accZ * accZ)
    );

    // =================================================
    // RESET BUTTON
    // =================================================

    if (digitalRead(BUTTON_PIN) == LOW)
    {
        alarmActive = false;

        seizureDetected = false;

        seizureStartTime = 0;

        digitalWrite(BUZZER_PIN, LOW);

        digitalWrite(LED_PIN, LOW);

        ledState = false;

        while (digitalRead(BUTTON_PIN) == LOW)
        {
            delay(10);
        }

        delay(200);
    }

    // =================================================
    // DETECTION LOGIC
    // =================================================

    if (magnitude > SEIZURE_THRESHOLD)
    {
        // Start monitoring
        if (!seizureDetected)
        {
            seizureDetected = true;

            seizureStartTime = millis();
        }

        // Confirm seizure
        if ((millis() - seizureStartTime) >= CONFIRM_TIME)
        {
            if (!alarmActive)
            {
                // Immediate buzzer warning
                digitalWrite(BUZZER_PIN, HIGH);

                bool cancelled = false;

                unsigned long graceStart = millis();

                // =================================================
                // FALSE ALERT CANCEL WINDOW
                // =================================================

                while ((millis() - graceStart) < GRACE_PERIOD)
                {
                    // Fast LED blink
                    if (millis() - previousBlink >= 120)
                    {
                        previousBlink = millis();

                        ledState = !ledState;

                        digitalWrite(LED_PIN, ledState);
                    }

                    // User cancels
                    if (digitalRead(BUTTON_PIN) == LOW)
                    {
                        cancelled = true;

                        seizureDetected = false;

                        seizureStartTime = 0;

                        digitalWrite(BUZZER_PIN, LOW);

                        digitalWrite(LED_PIN, LOW);

                        ledState = false;

                        while (digitalRead(BUTTON_PIN) == LOW)
                        {
                            delay(10);
                        }

                        break;
                    }

                    delay(5);
                }

                // =================================================
                // CONFIRMED ALERT
                // =================================================

                if (!cancelled)
                {
                    alarmActive = true;

                    sendTelegramMessage(
                        "🚨EMERGENCY_ALERT🚨Possible_seizure_detected."
                    );
                }
            }
        }
    }
    else
    {
        if (!alarmActive)
        {
            seizureDetected = false;
        }
    }

    // =================================================
    // ACTIVE ALERT MODE
    // =================================================

    if (alarmActive)
    {
        digitalWrite(BUZZER_PIN, HIGH);

        // Slow blinking LED
        if (millis() - previousBlink >= 250)
        {
            previousBlink = millis();

            ledState = !ledState;

            digitalWrite(LED_PIN, ledState);
        }
    }
    else
    {
        digitalWrite(BUZZER_PIN, LOW);

        digitalWrite(LED_PIN, LOW);
    }

    // =================================================
    // LOW POWER LOOP DELAY
    // =================================================

    delay(60);
}
