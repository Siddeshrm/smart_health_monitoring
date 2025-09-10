#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid = "Manoj";
const char* password = "12345678";

// Create an instance of the web server on port 80
WebServer server(80);

// Random data variables
int heartrate, heartrate1, spo2, spo;
float degree, degree1, cel, cel1, lat, lon;

// Function to generate random data
void generateRandomData() {
  heartrate = random(75, 86);
  heartrate1 = random(10, 20);
  spo2 = random(95, 100);
  spo = random(60, 70);
  degree = random(350, 370) / 10.0; // For decimal values
  degree1 = random(100, 150) / 10.0;
  cel = degree * (9.0 / 5.0) + 32;
  cel1 = degree1 * (9.0 / 5.0) + 32;
  lat = random(13133297, 13123295) / 1000000.0;
  lon = random(77567316, 77552316) / 1000000.0;
}

// Serve the dashboard HTML
void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP32 Dashboard</title>
      <style>
        body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
        table { margin: 0 auto; border-collapse: collapse; width: 50%; }
        th, td { border: 1px solid #ddd; padding: 8px; }
        th { background-color: #f2f2f2; }
      </style>
    </head>
    <body>
      <h1>ESP32 Dashboard</h1>
      <table>
        <tr><th>Parameter</th><th>Value</th></tr>
        <tr><td>Heart Rate</td><td id="heartrate"></td></tr>
        <tr><td>SpO2</td><td id="spo2"></td></tr>
        <tr><td>Temperature (°C)</td><td id="degree"></td></tr>
        <tr><td>Temperature (°F)</td><td id="cel"></td></tr>
        <tr><td>Latitude</td><td id="lat"></td></tr>
        <tr><td>Longitude</td><td id="lon"></td></tr>
      </table>
      <script>
        setInterval(() => {
          fetch('/data').then(response => response.json()).then(data => {
            document.getElementById('heartrate').textContent = data.heartrate;
            document.getElementById('spo2').textContent = data.spo2;
            document.getElementById('degree').textContent = data.degree;
            document.getElementById('cel').textContent = data.cel;
            document.getElementById('lat').textContent = data.lat;
            document.getElementById('lon').textContent = data.lon;
          });
        }, 1000); // Update every second
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

// Serve the random data as JSON
void handleData() {
  String json = "{";
  if (digitalRead(23) == HIGH) {
    json += "\"heartrate\":" + String(heartrate) + ",";
    json += "\"spo2\":" + String(spo2) + ",";
    json += "\"degree\":" + String(degree, 1) + ",";
    json += "\"cel\":" + String(cel, 1) + ",";
  } else {
    json += "\"heartrate\":" + String(heartrate1) + ",";
    json += "\"spo2\":" + String(spo) + ",";
    json += "\"degree\":" + String(degree1, 1) + ",";
    json += "\"cel\":" + String(cel1, 1) + ",";
  }
  json += "\"lat\":" + String(lat, 6) + ",";
  json += "\"lon\":" + String(lon, 6);
  json += "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  // Set pin 15 as input
  pinMode(15, INPUT);

  // Generate initial random data
  generateRandomData();

  // Define server routes
  server.on("/", handleRoot);    // Dashboard route
  server.on("/data", handleData); // Data route

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Generate new random data every 1 second
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 1000) {
    generateRandomData();
    lastUpdate = millis();
  }

  // Handle client requests
  server.handleClient();
}
