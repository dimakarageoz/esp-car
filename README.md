# ESP32 RC Car Project

This project is a custom remote-controlled car featuring differential drive, real-time video/shots capture, and an independent radio control channel using SX1268 LoRa modules.

## 📁 Repository Structure

The repository consists of three independent sub-projects, each fulfilling a specific role in the system:

* `car-main-v2/` — Main firmware for the car's controller (ESP32). It handles receiving commands via the radio channel and controlling the motor drivers using FreeRTOS tasks.
* `remote-controller/` — Firmware for the remote control unit (ESP32). It reads user input and transmits control commands to the car over the radio (434 MHz).
* `car-camera/` — Firmware for the ESP32-CAM module. It hosts an MJPEG web server over Wi-Fi for live video streaming and flashlight control.

## 🛠 Hardware Components

* **Microcontrollers:** ESP32 (main car controller and remote), ESP32-CAM (video server).
* **Motor Drivers:** PWM DC motor drivers (used for differential steering).
* **Radio Modules:** SX1268 (LoRa modules operating at 434 MHz) for reliable communication between the remote and the car.
* **Camera:** OV2640 module paired with the ESP32-CAM.

## 💻 Tech Stack

* **car-main-v2 & remote-controller:** Written in C++ using the Arduino framework. Projects are configured for [PlatformIO](https://platformio.org/). The [RadioLib](https://github.com/jgromes/RadioLib) library is used for radio communication, and `FreeRTOS` queues/tasks manage the controller's workload.
* **car-camera:** Written in C using the native [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/) framework. It utilizes the [esp_camera](https://github.com/espressif/esp32-camera) component and `esp_http_server` to stream frames directly to a web interface.

## 🚀 Setup and Installation

### 1. car-main-v2 (Car Receiver)

Open the `car-main-v2` folder in VS Code with the PlatformIO extension installed.
Ensure the SPI pins (MOSI=4, MISO=5, CLK=7, CS=10) and radio module pins (DIO1=9, NRST=12, BUSY=11) match your actual hardware wiring. Build and flash the code to the car's microcontroller.

### 2. remote-controller (Transmitter)

Open `remote-controller` via PlatformIO. This project is set up to receive characters via the Serial port and relay them over the radio. Default SPI pins are set for a standard ESP32 (SCK=18, MISO=19, MOSI=23). Compile and upload the code to the remote control board.

### 3. car-camera (Video/Shots Capture)

Open the `car-camera` project. You will need a configured ESP-IDF environment.
Before building, you must specify your Wi-Fi credentials (or local AP details) in the `main.c` file:
#### 🔑 Secrets and Credentials Configuration
To ensure security, network credentials and private radio channel settings are moved out of the main executable code into isolated configuration files `secrets.ini`, which are added to .gitignore.
```c
; secrets.ini

[secrets]
build_flags =
    -D WIFI_SSID=\YourWifiName\"
    -D WIFI_PASSWORD=\"YourWifiPassword\"
```

Build the project using `idf.py build` and flash the board using `idf.py -p (YOUR_PORT) flash`. Once successfully connected to Wi-Fi, the IP address for accessing the video stream will be printed in the Serial monitor.

## 🎮 Controls

The current remote controller implementation works in an asymmetrical master/slave testing mode (Ping-Pong / Command echo). To control the car, send standard WASD characters (W, A, S, D) via the Serial Monitor to the `remote-controller` device (which initiates the first PING).

The car's controller (`car-main-v2`) receives these packets and triggers movement through a FreeRTOS command queue. By default, motors are driven at 75% PWM capacity. During a differential turn, the left and right motors spin in opposite directions to rotate the car in place.
