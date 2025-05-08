// This is the working version to be uploaded

#include <SPI.h>
#include <MFRC522.h>
#include <Encoder.h>

#define RST_PIN 7     // RC522 RST
#define SS_PIN  1     // RC522 SDA/SS

#define ENCODER_CLK 2
#define ENCODER_DT  9
#define ENCODER_SW  8

Encoder myEnc(ENCODER_CLK, ENCODER_DT);
long lastPosition = -999;
bool lastBtnState = HIGH;

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 20, 21); // UART to ESP32 display

  pinMode(ENCODER_SW, INPUT_PULLUP);
  SPI.begin();  // Uses default pins for SCK, MISO, MOSI on ESP32-C3
  mfrc522.PCD_Init();

  Serial.println("RFID + Encoder ready");
}

void loop() {
  // --- ENCODER CHECK ---
  long newPos = myEnc.read();
  if (newPos != lastPosition) {
    int delta = newPos - lastPosition;
    lastPosition = newPos;
    Serial1.println(delta > 0 ? "1" : "-1");
  }

  bool btn = digitalRead(ENCODER_SW);
  if (btn == LOW && lastBtnState == HIGH) {
    Serial1.println("B");
  }
  lastBtnState = btn;

  // --- RFID CHECK ---
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial1.print("RFID UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) Serial1.print("0");
      Serial1.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial1.println();
    mfrc522.PICC_HaltA();  // Halt until next detection
  }

  delay(10);  // light debounce and polling loop
}