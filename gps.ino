#include <TinyGPSPlus.h>

TinyGPSPlus gps;

// Create a new serial port (Serial1) on GPIO 20 (TX), 21 (RX)
#define GPS_RX 20
#define GPS_TX 21

void setup() {
  Serial.begin(115200); // Serial monitor
  Serial1.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX); // GPS

  Serial.println("GPS setup started...");
}

void loop() {
  while (Serial1.available()) {
    gps.encode(Serial1.read());
  }

  if (gps.location.isUpdated()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Speed: ");
    Serial.println(gps.speed.kmph());
    Serial.println("------------------------");
  }
}
