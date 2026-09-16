// Stage 4.1 - I2C scan
// Proves the display is wired correctly before we try to draw on it.
#include <Wire.h>

const int SDA_PIN = 5;  // D4 on the silkscreen
const int SCL_PIN = 6;  // D5 on the silkscreen - printed SCK on the display

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) { }

  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println();
  Serial.println("=== I2C scan ===");
}

void loop() {
  int found = 0;

  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("device found at 0x%02X\n", addr);
      found++;
    }
  }

  if (found == 0) Serial.println("no devices found - check wiring");
  Serial.println("---");

  delay(3000);
}
