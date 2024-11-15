/**
 * @file    teoWiFi.cpp
 * @author  Kirill Scherba
 *
 * @copyright (C) Kirill Scherba 2024 <kirill@scherba.ru>
 *
 * @license teoWiFi is free software: you can redistribute it and/or modify it
 * under the terms of the BSD 3-clause license as published by the
 * University of California, Berkeley.
 *
 * teoWiFi is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the BSD 3-clause license for more details.
 *
 * You should have received a copy of the BSD 3-clause license along
 * with this program.  If not, see <http://www.opensource.org/licenses/bsd-license.php>.
 *
 * @version 0.0.1
 *
 * @brief TeoWiFi is Arduino library for connect to WiFi network.
 *
 * @details
 * TeoWiFi is a light library for Arduino for connect to WiFi network.
 * It based on Arduino WiFi library.
 * And it's use TeoEEPROM library for store WiFi login and password.
 */

#include "teoWiFi.h"
#include "teoEEPROM.h"

/**
 * TeoWiFiClass constructor.
 * Does nothing.
 */
TeoWiFiClass::TeoWiFiClass() {}

/**
 * Connect to a WiFi network with the given ssid and password.
 *
 * @param ssid The name of the WiFi network to connect to.
 * @param password The password of the WiFi network to connect to.
 */
void TeoWiFiClass::start(const char *ssid, const char *password) {

  // Serial should be started before this

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println("******************************************************");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Start the WiFi connection
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/**
 * Connect to a WiFi network with the data stored in the EEPROM.
 *
 * First, reads the WiFi connection data from the EEPROM.
 * If the data is invalid, asks the user for the WiFi connection data and stores
 * it in the EEPROM.
 * Then, connects to the WiFi network using the stored data.
 */
void TeoWiFiClass::start() {
  TeoEEPROMClass eeprom;

  // Read the WiFi connection data from the EEPROM
  WiFi_DATA buf = eeprom.read();

  // Check if the data is valid
  if (!eeprom.check(buf)) {
    // If the data is invalid, ask the user for the WiFi connection data and
    // store it in the EEPROM
    buf = eeprom.ask();
    eeprom.write(buf);
  }

  // Connect to the WiFi network using the stored data
  start(buf.ssid, buf.passwd);
}

/**
 * Start UDP server on port 8888
 */
void TeoWiFiClass::startUDP() { udp.begin(8888); }

// listen udp
void TeoWiFiClass::listenUDP() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = udp.remoteIP();
    for (int i = 0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(udp.remotePort());

    // read the packet into packetBufffer
    char packetBuffer[255];
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }

    Serial.print("Message: ");
    Serial.println(packetBuffer);
  }
}

/**
 * Resets the WiFi connection data stored in the EEPROM.
 *
 * This function is useful if you want to clear the stored WiFi connection
 * data and start fresh.
 */
void TeoWiFiClass::reset() {
  // Reset the EEPROM by calling the TeoEEPROMClass method
  TeoEEPROMClass().reset();
}
