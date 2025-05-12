
# ESP32 Project: Monitoring Temperature, Humidity, and Distance

This project uses two ESP32s to monitor temperature and humidity using DHT22 sensors and measure distance using HC-SR04 sensors. The data from the sensors is sent using ESP-NOW to the other ESP32 receiving device.

## Features

- **DHT22 sensor**: Reads the temperature and humidity of the surrounding environment.
- HC-SR04** Sensor: Measures distance using an ultrasonic sensor.
- **ESP-NOW Communication**: Transmits sensor data to other devices wirelessly using the ESP-NOW protocol.
- **OLED Display (ESP32 A)**: Displays sensor readings on an OLED display using I2C.
- **Blynk Integration (ESP32 A)**: Sends data to the Blynk app for remote monitoring of distance and temperature.
- Wi-Fi and Blynk Connection**: Connects to a Wi-Fi network for integration with the Blynk server.

## Requirements

- **ESP32** (for both devices)
- **DHT22** sensor (to read temperature and humidity)
- HC-SR04** sensor (to measure distance)
- 128x64 OLED screen** (only for ESP32 A)
- Blynk App** (for remote monitoring of distance and temperature)
- **Library**: 
  - `esp_now.h` (for ESP-NOW communication)
  - `WiFi.h` (for Wi-Fi connection)
  - `DHT.h` (for DHT22 sensor)
  - `Wire.h` and `Adafruit_SSD1306.h` (for OLED display)
  - `BlynkSimpleEsp32.h` (for integration with Blynk)

## Installation

1. **Clone repository**:
   bash
   git clone
   ```https://github.com/hfzrfa/ESP_NOW-Blynk_Legacy```
   ``` cd ESP_NOW-Blynk_Legacy ```
3. **Install the Required Libraries**:
   - Open the Arduino IDE.
   - Go to **Sketch > Include Library > Manage Libraries**.
   - Locate and install the following libraries:
     - `Adafruit SSD1306`
     - `Adafruit GFX`
     - `DHT sensor library`
     - `Blynk`

4. **Set Up Devices**:
   - Connect the ESP32 to your computer.
   - Install the DHT22 sensor on GPIO pin 14.
   - Attach the HC-SR04 sensor with TRIG_PIN on GPIO pin 13 and ECHO_PIN on GPIO pin 32.
   - Connect the OLED display with I2C to the SDA and SCL pins on the ESP32.

5. **Configure and Upload Code**:
   - Replace the Blynk **auth token**, **SSID**, and **Wi-Fi password** in the code.
   - Upload the `ESP32 A.ino` code for the device that displays data on the OLED screen and sends data to Blynk.
   - Upload the `ESP32 B.ino` code for devices that send temperature and distance data using ESP-NOW.

6. **Install APK for Android**:
   ``` https://drive.google.com/file/d/1nrwVfLhIXAS69Yc0cLEQJbWF3kFcvQ-N/view?usp=sharing ```

## How to use

1. After uploading the code to both ESP32s, the first device will start sending temperature, humidity, and distance data via ESP-NOW.
2. The second device will receive this data and display it on the OLED screen and send the data to the Blynk app.
3. You can monitor the data in real-time through the Blynk app on your phone.

## Technology Used

- **ESP32**: A microcontroller platform used for wireless communication and data processing.
- **ESP-NOW**: Wireless communication protocol for ESP32 devices.
- **DHT22**: Sensor for measuring temperature and humidity.
- **HC-SR04**: Ultrasonic sensor for measuring distance.
- **Blynk**: IoT platform for real-time data monitoring.
- **OLED**: Screen to display information locally.

## Contribution

If you would like to contribute to this project, please fork this repository and submit a pull request with your changes.

1. Fork the repository.
2. Create a new branch:
   
   ```bash
    git checkout -b your-features ```
   
4. Commit your changes:
   ```bash
   git commit -m “Description of changes”
   ```
5. Push your branch:
   ```bash
   git push origin feature-you
   ```
6. Send a pull request to the main branch.

## License

This project uses the [MIT License](LICENSE) license.
