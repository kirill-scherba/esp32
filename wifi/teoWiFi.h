#ifndef TEO_WIFI_H
#define TEO_WIFI_H

#include <WiFi.h>
#include <WiFiUDP.h>

class TeoWiFiClass {
public:
  TeoWiFiClass();
  void start();
  void start(const char *ssid, const char *password);
  void startUDP();
  void listenUDP();

private:
  WiFiUDP udp;
};

#endif