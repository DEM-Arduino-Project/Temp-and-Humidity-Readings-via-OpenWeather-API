# 🌦️ IoT Weather Station via OpenWeather API

> **Real-time environmental monitoring powered by Arduino Uno R4 WiFi and OpenWeatherMap.**

## 📖 Overview

This project is a robust IoT solution that fetches live weather data for a specified location (e.g., Timișoara, RO) using the OpenWeatherMap API. It visualizes the data effectively using two output methods: a **LiquidCrystal I2C Display (16x2)** for detailed information and the **Arduino Uno R4 builtin LED Matrix** for scrolling, glanceable updates.

Whether you are tracking temperature, humidity, or wind speed, this system provides accurate, real-time readings with a professional implementation of JSON parsing and WiFi management.

---

## ✨ Features

- **📶 Seamless Connectivity**: Connects to local WiFi networks efficiently using the `WiFiS3` library.
- **☁️ Real-Time API Integration**: Fetches comprehensive weather data (Temperature, Humidity, Pressure, Wind Speed, Clouds) via HTTP GET requests.
- **📊 Robust JSON Parsing**: Utilizes `ArduinoJson` to safely deserialize and extract data from API responses.
- **🖥️ Dual-Display Output**:
  - **LCD Screen**: Shows connection status, HTTP codes, and static weather metrics.
  - **LED Matrix**: Scrolls "Temperature" and "Humidity" for a modern visual effect.
- **🛠️ Debugging Ready**: Includes a `WIFI_DEBUG` switch and Serial logging for easy troubleshooting of network and API issues.

---

## 🏗️ Architecture & Tech Stack

- **Hardware**: 
  - Arduino Uno R4 WiFi
  - LCD 1602 Display (I2C Interface)
- **Language**: C++ (Arduino Framework)
- **Key Libraries**:
  - [`ArduinoJson`](https://arduinojson.org/) - For parsing JSON responses.
  - [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C) - For controlling the LCD.
  - `Arduino_LED_Matrix` & `ArduinoGraphics` - For the LED matrix display.

---

## ⚙️ Configuration & Setup

### 1. Prerequisites
Ensure you have the latest **Arduino IDE** installed along with the Board Package for **Arduino Uno R4**.

### 2. Dependencies
Install the following libraries via the Arduino Library Manager:
- `ArduinoJson`
- `LiquidCrystal_I2C`
- `ArduinoGraphics`

### 3. WiFi & API Configuration
The project uses a separate header file for sensitive data to keep the main logic clean.
Navigate to `Arduino_Wifi_Connect.h` and configure your credentials:

```cpp
// Update these variables with your network details
const char * WIFI_SSID = "YOUR_WIFI_NAME";
const char * WIFI_PASS = "YOUR_WIFI_PASSWORD";

// OpenWeatherMap Configuration
String API_KEY = "YOUR_API_KEY";
String CITY_NAME = "Timisoara";
String COUNTRY_CODE = "RO";
```

### 4. Hardware Wiring
| LCD Pin | Arduino Pin |
|---------|-------------|
| GND     | GND         |
| VCC     | 5V          |
| SDA     | SDA         |
| SCL     | SCL         |

---

## 🚀 Usage

1.  **Upload** the sketch to your Arduino Uno R4.
2.  Open the **Serial Monitor** (Baud Rate: `9600`) to view connection logs.
3.  The **LCD** will display "Connecting...", followed by "Success!" once the server responds.
4.  The system will cycle every 10 seconds, updating the weather data on the LCD and scrolling it on the LED Matrix.

---

## 📝 License

This project is open-source and available for personal and educational use.
