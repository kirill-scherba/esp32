#ifndef TEO_EEPROM_H
#define TEO_EEPROM_H

#include <EEPROM.h>

// WiFi_DATA is wifi connection data.
struct WiFi_DATA {
  char id1;
  char ssid[31];
  char id2;
  char passwd[31];
};

class TeoEEPROMClass {
public:
  TeoEEPROMClass();

  WiFi_DATA read();
  void write(WiFi_DATA buf);
  WiFi_DATA ask();
  bool check(WiFi_DATA buf);
  void reset();

private:
  String readString();
};

#endif