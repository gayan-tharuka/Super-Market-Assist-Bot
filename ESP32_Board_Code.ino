/*
 * ESP32 Board Code for Super Market Assist Bot
 * Receives product information from Python middleware
 * Displays on 16x2 LCD
 */

#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD with I2C address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi credentials
const char* ssid     = "ssid";
const char* password = "pass";

// Create server on port 80
WiFiServer server(80);

// Function to scroll text on LCD
void scrollText(int row, String message, int delayTime, int lcdColumns) {
  // Add padding to the message for smooth scrolling
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message; 
  } 
  message = message + " "; 
  
  // Scroll the message
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.setHostname("esp32_board"); // Set device hostname

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to Wi-Fi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("WELCOME!");
}

void loop() {
  // Check for client connections
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client connected");

    // Wait for client data
    while (client.connected() && !client.available()) {
      delay(1);
    }

    // Read incoming message
    String message = client.readStringUntil('\n');
    Serial.println("Message received: " + message);

    // Display total on second line if message contains "Total"
    if (message.indexOf("Total") != -1){
      lcd.setCursor(0, 1);
      lcd.print(message);
      delay(3000);
    }
    else{
      // Scroll other messages on second line
      scrollText(1, message, 500, 16);
    }

    // Send acknowledgment back to client
    client.println("Message received");

    // Close connection
    client.stop();
    Serial.println("Client disconnected");
  }
}
