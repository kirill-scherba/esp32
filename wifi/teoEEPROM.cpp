/*
 * Copyright 2024 Kirill Scherba <kirill@scherba.ru>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file teoEEPROM.cpp
 * Implements the TeoEEPROMClass class, which is responsible for storing and
 * retrieving WiFi connection data from the EEPROM.
 */

#include "teoEEPROM.h"

// Define the size of the EEPROM
#define EEPROM_SIZE 64

// Magic constants
#define MAGIC1 0x1A
#define MAGIC2 0x2B

/**
 * TeoEEPROMClass constructor.
 * Does nothing.
 */
TeoEEPROMClass::TeoEEPROMClass() {}

/**
 * Reads the WiFi connection data from the EEPROM.
 *
 * @return WiFi_DATA containing the SSID and password
 */
WiFi_DATA TeoEEPROMClass::read() {
  WiFi_DATA buf;

  // Begin EEPROM operations
  EEPROM.begin(EEPROM_SIZE);

  // Retrieve the data from EEPROM
  EEPROM.get(0, buf);

  // Check if the data is valid
  if (buf.id1 != MAGIC1 || buf.id2 != MAGIC2) {
    Serial.println("Invalid EEPROM format");
    buf.ssid[0] = '\0';
    buf.passwd[0] = '\0';
  } else {
    // Print the retrieved data
    // Serial.printf("EEPROM SSID: %s\n", buf.ssid);
    // Serial.printf("EEPROM Password: %s\n", buf.passwd);
  }

  // End EEPROM operations
  EEPROM.end();

  // Return the retrieved data
  return buf;
}

/**
 * Writes the WiFi connection data to the EEPROM.
 *
 * @param buf WiFi_DATA containing the SSID and password
 */
void TeoEEPROMClass::write(WiFi_DATA buf) {
  // Begin EEPROM operations
  EEPROM.begin(EEPROM_SIZE);

  // Set the magic constants
  buf.id1 = MAGIC1;
  buf.id2 = MAGIC2;

  // Ensure null termination of the strings
  buf.ssid[sizeof(buf.ssid) - 1] = '\0';
  buf.passwd[sizeof(buf.passwd) - 1] = '\0';

  // Write the data to EEPROM
  EEPROM.put(0, buf);

  // End EEPROM operations
  EEPROM.end();
}

/**
 * Asks the user for the WiFi connection data and stores it in a WiFi_DATA
 * structure.
 *
 * @return WiFi_DATA containing the SSID and password
 */
WiFi_DATA TeoEEPROMClass::ask() {
  WiFi_DATA buf;

  // Get wifi SSID
  Serial.println("Please enter wifi ssid name:");

  // Read the user input until newline
  String ssid = readString();
  ssid.trim();

  // Check if the input is valid
  if (ssid.length() == 0) {
    Serial.println("Invalid ssid");
    buf.ssid[0] = '\0';
  } else {
    // Copy the ssid to buf
    strcpy(buf.ssid, ssid.c_str());
  }
  Serial.println(ssid);

  // Get wifi password
  Serial.println("Please enter wifi password:");

  // Read the user input until newline
  String passwd = readString();
  passwd.trim();

  // Check if the input is valid
  if (passwd.length() == 0) {
    Serial.println("Invalid password");
    buf.passwd[0] = '\0';
  } else {
    // Copy the password to buf
    strcpy(buf.passwd, passwd.c_str());
  }
  Serial.println(passwd);

  return buf;
}

/**
 * Reads a string from the serial port until a newline is encountered.
 *
 * @param delimiter character to stop reading at
 * @return string containing the read data
 */
String TeoEEPROMClass::readString() {
  String receivedString = "";
  // Wait for data to be available
  while (Serial.available() == 0) {
    // You might want to add a timeout here
  }

  // Read the incoming string
  receivedString = Serial.readStringUntil('\n');

  // Remove any leading/trailing whitespace
  receivedString.trim();

  return receivedString;
}

/**
 * Checks if the WiFi connection data is valid.
 *
 * @param buf WiFi_DATA containing the SSID and password
 * @return true if the data is valid, false otherwise
 */
bool TeoEEPROMClass::check(WiFi_DATA buf) {
  // Check if the ssid or password is empty string
  return !(buf.ssid[0] == '\0' || buf.passwd[0] == '\0');
}

/**
 * Resets the EEPROM by writing empty data to it.
 *
 * This is useful if you want to clear the EEPROM and start fresh.
 */
void TeoEEPROMClass::reset() {
  WiFi_DATA buf = {}; // Empty struct to overwrite the EEPROM

  // Start the EEPROM operations
  EEPROM.begin(EEPROM_SIZE);

  // Write empty data to the EEPROM
  EEPROM.put(0, buf);

  // End the EEPROM operations
  EEPROM.end();
}
