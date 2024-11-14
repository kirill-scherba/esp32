// This simple UDP example for ESP32 board.
//
// created 11 Nov 2024
// modified 14 Nov 2024
// by Kirill Scherba

#include <teoWiFi.h>

TeoWiFiClass wifi;

void setup() {
  // Start serial
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }

  // Clear saved wifi credentials
  // wifi.reset();

  // Start WiFi and UDP
  wifi.start();
  wifi.startUDP();
}

void loop() {
  wifi.listenUDP();
  delay(100);
}
