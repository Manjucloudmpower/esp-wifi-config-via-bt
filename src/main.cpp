
#include <Arduino.h>
#include "BluetoothProv.hpp"

void setup() {
  Serial.begin(115200);
  initBLProvision();
  delay(3000);
  Serial.println(WiFi.status() == WL_CONNECTED? "Auto Connected": "Not connected");
}

void loop() {
Serial.println("Loop...");
delay(5000);  
}