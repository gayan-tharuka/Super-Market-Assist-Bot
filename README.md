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
- ESP32 Camera module
- Arduino UNO with motor shield
- 16x2 I2C LCD display
- Ultrasonic sensors (for human following)
- DC motors with wheels
- Power bank/battery

## Setup Instructions

1. **Human Following Bot**:
   - Upload `human_follow_bot_code.ino` to Arduino
   - Connect ultrasonic sensors and motors as per code comments

2. **ESP32 Camera Module**:
   - Upload `esp32_cam_module_code.ino` to ESP32-CAM
   - Note the IP address after upload

3. **ESP32 Board with LCD**:
   - Upload `esp32_board_code.ino` to ESP32
   - Connect I2C LCD display

4. **Python Middleware**:
   - Install required packages: `pip install opencv-python numpy pyzbar`
   - Update IP addresses in `qr_processing.py`
   - Run the script: `python qr_processing.py`

## Working
1. Customer approaches the bot which starts following them
2. Camera scans product QR codes
3. Python script processes QR codes and calculates total
4. ESP32 board displays product info and running total on LCD

## Future Enhancements
- Add voice feedback
- Implement weight sensors for product verification
- Add payment integration
- Improve human detection algorithm
