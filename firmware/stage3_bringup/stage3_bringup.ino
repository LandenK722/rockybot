// Stage 3 - XIAO ESP32-S3 bring-up test
// Nothing connected but USB, plus a button on D9 if you have it wired.

const int LED_PIN    = 21;  // onboard user LED - ACTIVE LOW on this board
const int BUTTON_PIN = 8;   // D9 on the silkscreen

bool lastButton = HIGH;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) { }   // wait for USB, but never hang

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println();
  Serial.println("=== XIAO ESP32-S3 Stage 3 ===");
  Serial.printf("Chip model : %s\n", ESP.getChipModel());
  Serial.printf("CPU freq   : %u MHz\n", getCpuFrequencyMhz());
  Serial.printf("Flash size : %u bytes\n", ESP.getFlashChipSize());
  Serial.printf("PSRAM size : %u bytes\n", ESP.getPsramSize());
  Serial.println("Type something and press Enter to echo it.");
  Serial.println("Hold the D9 button to see it register.");
}

void loop() {
  // heartbeat - LOW turns this LED ON
  digitalWrite(LED_PIN, (millis() / 500) % 2 ? HIGH : LOW);

  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length()) Serial.printf("echo: %s\n", line.c_str());
  }

  bool b = digitalRead(BUTTON_PIN);
  if (b != lastButton) {
    delay(25);                                // debounce
    if (digitalRead(BUTTON_PIN) == b) {
      Serial.println(b == LOW ? "button: PRESSED" : "button: released");
      lastButton = b;
    }
  }
}
