#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

// WiFi Configuration
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";

// Pin Definitions
const int SERVO_PIN = 18;
const int ULTRASONIC_TRIG_PIN = 5;
const int ULTRASONIC_ECHO_PIN = 19;
const int IR_SENSOR_ENTRY_PIN = 21;
const int IR_SENSOR_EXIT_PIN = 22;
const int LED_STATUS_PIN = 2;
const int BUZZER_PIN = 23;

// Constants
const int PARKING_DISTANCE_THRESHOLD = 10; // cm
const int GATE_OPEN_ANGLE = 90;
const int GATE_CLOSE_ANGLE = 0;
const int GATE_OPEN_DURATION = 3000; // ms
const int MAX_PARKING_SLOTS = 5;

// Global Variables
Servo gateServo;
WebServer server(80);
bool isParkingFull = false;
int availableSlots = MAX_PARKING_SLOTS;
unsigned long gateOpenTime = 0;
bool isGateOpen = false;
bool entryDetected = false;
bool exitDetected = false;

// Function Declarations
void initializeHardware();
void connectToWiFi();
void setupWebServer();
void handleSensorReadings();
void controlGate();
void updateParkingStatus();
long readUltrasonicDistance();
bool readIRSensor(int pin);
void openGate();
void closeGate();
void blinkStatusLED(int times);
void soundBuzzer(int duration);

// Web Server Handlers
void handleRoot();
void handleStatus();
void handleAPI();

void setup()
{
  Serial.begin(115200);
  Serial.println("Smart Parking System Starting...");

  initializeHardware();
  connectToWiFi();
  setupWebServer();

  Serial.println("Smart Parking System Ready!");
  blinkStatusLED(3);
}

void loop()
{
  server.handleClient();
  handleSensorReadings();
  controlGate();
  updateParkingStatus();
  delay(100); // Small delay for stability
}

void initializeHardware()
{
  // Initialize pins
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  pinMode(IR_SENSOR_ENTRY_PIN, INPUT);
  pinMode(IR_SENSOR_EXIT_PIN, INPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize servo
  gateServo.attach(SERVO_PIN);
  closeGate();

  Serial.println("Hardware initialized");
}

void connectToWiFi()
{
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void setupWebServer()
{
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/api", handleAPI);

  server.begin();
  Serial.println("Web server started");
}

void handleSensorReadings()
{
  static unsigned long lastReading = 0;
  if (millis() - lastReading < 200)
    return; // Read sensors every 200ms
  lastReading = millis();

  bool currentEntryDetection = readIRSensor(IR_SENSOR_ENTRY_PIN);
  bool currentExitDetection = readIRSensor(IR_SENSOR_EXIT_PIN);

  // Entry detection logic
  if (currentEntryDetection && !entryDetected && !isParkingFull)
  {
    entryDetected = true;
    availableSlots--;
    openGate();
    soundBuzzer(200);
    Serial.println("Vehicle entering - Gate opened");
  }
  else if (!currentEntryDetection && entryDetected)
  {
    entryDetected = false;
  }

  // Exit detection logic
  if (currentExitDetection && !exitDetected)
  {
    exitDetected = true;
    if (availableSlots < MAX_PARKING_SLOTS)
    {
      availableSlots++;
      openGate();
      soundBuzzer(100);
      Serial.println("Vehicle exiting - Gate opened");
    }
  }
  else if (!currentExitDetection && exitDetected)
  {
    exitDetected = false;
  }
}

void controlGate()
{
  if (isGateOpen && millis() - gateOpenTime >= GATE_OPEN_DURATION)
  {
    closeGate();
  }
}

void updateParkingStatus()
{
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate < 1000)
    return; // Update every second
  lastUpdate = millis();

  isParkingFull = (availableSlots <= 0);

  // Status LED indication
  if (isParkingFull)
  {
    digitalWrite(LED_STATUS_PIN, HIGH); // Solid red when full
  }
  else
  {
    // Blink green when available
    digitalWrite(LED_STATUS_PIN, !digitalRead(LED_STATUS_PIN));
  }

  Serial.printf("Available slots: %d/%d, Parking full: %s\n",
                availableSlots, MAX_PARKING_SLOTS,
                isParkingFull ? "Yes" : "No");
}

long readUltrasonicDistance()
{
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2; // Convert to centimeters

  return distance;
}

bool readIRSensor(int pin)
{
  return digitalRead(pin) == LOW; // IR sensor typically LOW when object detected
}

void openGate()
{
  if (!isGateOpen)
  {
    gateServo.write(GATE_OPEN_ANGLE);
    isGateOpen = true;
    gateOpenTime = millis();
    Serial.println("Gate opened");
  }
}

void closeGate()
{
  if (isGateOpen)
  {
    gateServo.write(GATE_CLOSE_ANGLE);
    isGateOpen = false;
    Serial.println("Gate closed");
  }
}

void blinkStatusLED(int times)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(LED_STATUS_PIN, HIGH);
    delay(200);
    digitalWrite(LED_STATUS_PIN, LOW);
    delay(200);
  }
}

void soundBuzzer(int duration)
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void handleRoot()
{
  String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Smart Parking System</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; background-color: #f0f0f0; }
        .container { max-width: 600px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        .status-card { background: #e8f4fd; padding: 15px; border-radius: 5px; margin: 10px 0; }
        .available { background: #d4edda; color: #155724; }
        .full { background: #f8d7da; color: #721c24; }
        .btn { background: #007bff; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; }
        .btn:hover { background: #0056b3; }
        h1 { color: #333; text-align: center; }
        .refresh { text-align: center; margin: 20px 0; }
    </style>
    <script>
        function refreshStatus() {
            fetch('/api')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('slots').textContent = data.available_slots + '/' + data.total_slots;
                    document.getElementById('status').textContent = data.is_full ? 'PENUH' : 'TERSEDIA';
                    document.getElementById('status-card').className = 'status-card ' + (data.is_full ? 'full' : 'available');
                });
        }
        setInterval(refreshStatus, 2000);
    </script>
</head>
<body>
    <div class="container">
        <h1>🅿️ Smart Parking System</h1>
        <div id="status-card" class="status-card available">
            <h3>Status Parkiran: <span id="status">TERSEDIA</span></h3>
            <p><strong>Slot Tersedia:</strong> <span id="slots">)" + String(availableSlots) + "/" + String(MAX_PARKING_SLOTS) + R"(</span></p>
            <p><strong>IP Address:</strong> )" + WiFi.localIP().toString() + R"(</p>
        </div>
        <div class="refresh">
            <button class="btn" onclick="refreshStatus()">Refresh Status</button>
        </div>
    </div>
</body>
</html>
  )";
  server.send(200, "text/html", html);
}

void handleStatus()
{
  String status = isParkingFull ? "FULL" : "AVAILABLE";
  server.send(200, "text/plain", "Parking Status: " + status + "\nAvailable Slots: " + String(availableSlots) + "/" + String(MAX_PARKING_SLOTS));
}

void handleAPI()
{
  DynamicJsonDocument doc(200);
  doc["available_slots"] = availableSlots;
  doc["total_slots"] = MAX_PARKING_SLOTS;
  doc["is_full"] = isParkingFull;
  doc["gate_status"] = isGateOpen ? "open" : "closed";
  doc["ip_address"] = WiFi.localIP().toString();

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}