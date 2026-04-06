# 🐟 Smart Fish Feeder v2.0

An IoT-based automated fish feeding system powered by **ESP32**, featuring real-time water quality monitoring, AI-based anomaly detection, cloud sync via **Firebase**, and an OLED dashboard — all managed through a modular production-ready codebase.

---

## ✨ Features

- 🕒 **Scheduled Auto-Feeding** — DS3231 RTC-based precise feeding schedule
- 🌡️ **Temperature Monitoring** — DS18B20 waterproof temperature sensor with Kalman filtering
- 🧪 **pH Monitoring** — Calibrated analog pH sensor with temperature compensation
- 💧 **Turbidity Monitoring** — Real-time water clarity tracking in NTU
- 🤖 **AI / Predictive Analytics** — Water quality scoring and anomaly detection algorithm
- ☁️ **Firebase Cloud Sync** — Real-time database updates every 5 seconds
- 📟 **OLED Dashboard** — SH1106 display showing live sensor readings
- 🔔 **Buzzer Alerts** — Critical alerts triggered on anomaly detection
- 🐾 **Watchdog & Low Power Manager** — System reliability and power efficiency
- 📋 **Serial Logger** — Heartbeat logging and event tracking via Serial Monitor

---

## 🛠️ Hardware Requirements

| Component | Description |
|---|---|
| ESP32 | Main microcontroller (Wi-Fi + BLE) |
| DS3231 RTC | Real-time clock module (I2C) |
| DS18B20 | Waterproof temperature sensor (OneWire) |
| pH Sensor | Analog pH probe + signal board |
| Turbidity Sensor | Analog turbidity sensor |
| SG90 / MG995 Servo | Servo motor for feeder mechanism |
| SH1106 OLED | 1.3" I2C OLED display |
| Buzzer | Active or passive buzzer |

---

## 📁 Project Structure

```
FishFeeder/
├── FishFeeder.ino          # Main sketch — entry point
├── data/                   # SPIFFS data files (if any)
└── src/
    ├── Core/
    │   ├── SystemWatchdog.h        # Hardware watchdog handler
    │   └── LowPowerManager.h      # Power management logic
    ├── Timekeeping/
    │   ├── DS3231_RTC.h            # RTC initialization and time reading
    │   └── FeedScheduler.h        # Feeding schedule management
    ├── Sensors/
    │   ├── DS18B20.h               # Temperature sensor with filtering
    │   ├── pH_Sensor.h             # Calibrated pH reading
    │   └── Turbidity.h            # Turbidity with Kalman filter
    ├── Actuators/
    │   ├── ServoFeeder.h           # Servo motor control for food dispensing
    │   └── Buzzer.h               # Alert tones and patterns
    ├── Displays/
    │   ├── OLED_SH1106.h          # OLED dashboard rendering
    │   └── SerialMonitor.h        # Serial logging and heartbeat
    └── Cloud/
        ├── FirebaseClient.h        # Firebase RTDB read/write
        └── PredictiveAlgo.h       # Water quality score + anomaly detection
```

---

## ⚙️ Setup & Configuration

### 1. Install Arduino Libraries

Install the following via **Arduino Library Manager** or manually:

- `Firebase ESP Client` by Mobizt — **v4.4.17**
- `ESP32Servo`
- `Wire` (built-in)
- `WiFi` (built-in for ESP32)

### 2. Configure Credentials

Open `FishFeeder.ino` and edit these 4 lines:

```cpp
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* FIREBASE_API_KEY = "YOUR_FIREBASE_WEB_API_KEY";
const char* DATABASE_URL  = "https://your-project-default-rtdb.region.firebasedatabase.app/";
```

> ⚠️ **Never commit real credentials to GitHub.** Use a `.gitignore` or environment-based config for production.

### 3. Firebase Setup

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Create a new project → Enable **Realtime Database**
3. Copy the **Web API Key** from Project Settings
4. Copy the **Database URL** from Realtime Database settings
5. Set database rules to allow read/write (or configure auth)

### 4. pH Sensor Calibration

Calibration values are set in `setup()`:

```cpp
pHSensor.calibrate(1.75, 2.05);  // pH4=1.75V, pH7=2.05V
```

Adjust these values based on your actual buffer solution measurements.

### 5. Upload

- Select board: **ESP32 Dev Module**
- Upload speed: **115200**
- Flash `FishFeeder.ino` via Arduino IDE

---

## ☁️ Firebase Data Structure

Data is pushed to `/sensors/latest` every **5 seconds**:

```json
{
  "device_id": "fish_feeder_001",
  "timestamp": 12345678,
  "uptime_s": 3600,
  "temp_c": 27.5,
  "ph": 7.2,
  "turbidity_ntu": 45.3,
  "quality_index": 85,
  "feeding_active": false,
  "anomaly_detected": false
}
```

---

## 🔄 System Loop (1-second cycle)

Each second, the system executes:

1. **Feed Watchdog** — keeps the system alive
2. **Read Sensors** — temperature, pH, turbidity
3. **AI Processing** — quality score + anomaly detection
4. **Actuation** — check and execute feeding schedule
5. **Update Display** — refresh OLED + Serial log
6. **Cloud Sync** — push JSON to Firebase every 5s
7. **Critical Alerts** — trigger buzzer if anomaly detected

---

## 📡 I2C Pin Mapping (ESP32)

| Signal | ESP32 Pin |
|---|---|
| SDA | GPIO 21 |
| SCL | GPIO 22 |

---

## 🧠 AI / Quality Index

The `PredictiveAlgo` module computes a **water quality index (0–100)** based on temperature, pH, and turbidity readings. If values fall outside safe thresholds, `anomaly_detected` is set to `true`, triggering buzzer alerts and a Firebase log event.

---

## 📜 License

This project is licensed under the terms found in the [LICENSE](LICENSE) file.

---

## 👤 Author

**CJ ChANu** — SLIIT IoT Project  
[GitHub Profile](https://github.com/CJChANu)
