#include <Wire.h>
#include <Adafruit_PN532.h>

#define SDA_PIN 8
#define SCL_PIN 9

// PN532 over I2C
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 20, 21); // Or other UART pins

  Serial.println("Started");

  Wire.begin(SDA_PIN, SCL_PIN);
  nfc.begin();

  if (!nfc.SAMConfig()) {
    Serial.println("PN532 init failed!");
    while (1) delay(10);
  }

  Serial.println("PN532 ready.");
}

void loop() {
  Serial.println("loop");
  delay(500);
}