/*
 * Human Following Robot Code
 * Uses ultrasonic sensors to detect and follow humans
 * Controls 4 DC motors for movement
 */

#include <NewPing.h>           // Ultrasonic sensor library
#include <Servo.h>             // Servo library
#include <AFMotor.h>           // Adafruit Motor Shield library

// Ultrasonic sensor pins
#define TRIGGER_PIN_CENTER A1  // Center ultrasonic sensor
#define ECHO_PIN_CENTER A0
#define MAX_DISTANCE 200       // Maximum ping distance (200cm)

unsigned int centerDistance = 0;  // Stores distance from center sensor

// Ultrasonic sensor object
NewPing sonarCenter(TRIGGER_PIN_CENTER, ECHO_PIN_CENTER, MAX_DISTANCE);

// Create motor objects (M1-M4)
AF_DCMotor Motor1(1, MOTOR12_1KHZ); // Motor 1, 1KHz PWM
AF_DCMotor Motor2(2, MOTOR12_1KHZ); // Motor 2
AF_DCMotor Motor3(3, MOTOR34_1KHZ); // Motor 3
AF_DCMotor Motor4(4, MOTOR34_1KHZ); // Motor 4

Servo myservo;  // Create servo object for scanning
int pos = 0;    // Variable to store servo position

int speed = 75;  // Base motor speed (0-255)

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  myservo.attach(10);  // Servo attached to pin 10
  
  // Servo initialization sweep
  for(pos = 90; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15);
  } 
  for(pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
  for(pos = 0; pos <= 90; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
}

void loop() {
  // Read center ultrasonic sensor
  centerDistance = sonarCenter.ping_cm();
  
  reading(); // Print sensor readings to serial
  
  // Decision making based on sensor readings
  if(centerDistance < 89) {
    // Object very close - stop to avoid collision
    stopMotors();
  }
  else if(centerDistance >= 90) {
    // Object in front at safe distance - move forward
    moveForward();
  }
}

// Stop all motors
void stopMotors() {
  Motor1.setSpeed(0);
  Motor1.run(RELEASE);
  Motor2.setSpeed(0);
  Motor2.run(BACKWARD);
  Motor3.setSpeed(10);
  Motor3.run(BACKWARD);
  Motor4.setSpeed(10);
  Motor4.run(RELEASE);
  Serial.println("Motor Stopped");
}

// Move all motors forward
void moveForward() {
  Motor1.setSpeed(speed);
  Motor1.run(FORWARD);
  Motor2.setSpeed(speed);
  Motor2.run(FORWARD);
  Motor3.setSpeed(speed);
  Motor3.run(FORWARD);
  Motor4.setSpeed(speed);
  Motor4.run(FORWARD);
  Serial.println("Motor Start");
}

// Print sensor readings to serial monitor
void reading(){
  Serial.print("center - ");
  Serial.println(centerDistance);
  delay(500);
}
