/*
 * ESP32 Camera Module Code
 * Captures and streams images for QR code detection
 * Provides different resolution options
 */

#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
 
// WiFi credentials
const char* WIFI_SSID = "ssid";
const char* WIFI_PASS = "pass";
 
WebServer server(80); // Web server on port 80
 
// Define different resolutions
static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);

// Function to capture and serve JPEG image
void serveJpg()
{
  // Capture frame from camera
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", ""); 
    return;
  }
  
  // Log capture details
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));
 
  // Send image to client
  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}
 
// Handler for low resolution image
void handleJpgLo()
{
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}
 
// Handler for high resolution image
void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}
 
// Handler for medium resolution image
void handleJpgMid()
{
  if (!esp32cam::Camera.changeResolution(midRes)) {
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}
 
 
void setup(){
  Serial.begin(115200);
  WiFi.setHostname("esp32_cam"); // Set device hostname

  Serial.println("Initializing camera...");
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker); // Set pins for AI Thinker module
    cfg.setResolution(hiRes);     // Default to high resolution
    cfg.setBufferCount(2);        // Double buffer
    cfg.setJpeg(80);              // JPEG quality (80%)
 
    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }
  
  // Connect to WiFi
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  // Print connection details
  Serial.print("Stream available at: http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-lo.jpg");
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /cam-mid.jpg");
 
  // Set up server endpoints
  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam-mid.jpg", handleJpgMid);
 
  server.begin(); // Start web server
}
 
void loop()
{
  server.handleClient(); // Handle client requests
}
