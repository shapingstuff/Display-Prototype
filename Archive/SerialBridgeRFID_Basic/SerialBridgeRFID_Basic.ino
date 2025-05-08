#include <Wire.h>
#include <Adafruit_PN532.h>

#define SDA_PIN 8
#define SCL_PIN 9

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

bool tag_present = false;
uint8_t last_uid[7];
uint8_t last_uid_len = 0;
uint8_t no_read_count = 0;

const uint8_t MAX_NO_READS = 20;  // Number of failed reads before confirming removal

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 20, 21);

  Wire.begin(SDA_PIN, SCL_PIN);
  nfc.begin();

  if (!nfc.SAMConfig()) {
    Serial.println("PN532 init failed");
    while (1) delay(10);
  }

  Serial.println("PN532 ready");
}

void loop() {
  uint8_t uid[7];
  uint8_t uidLength = 0;

  bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100);

  if (success) {
    no_read_count = 0;

    if (!tag_present || uidLength != last_uid_len || memcmp(uid, last_uid, uidLength) != 0) {
      tag_present = true;
      last_uid_len = uidLength;
      memcpy(last_uid, uid, uidLength);

      Serial.print("Tag detected: ");
      Serial1.print("Tag: ");
      for (uint8_t i = 0; i < uidLength; i++) {
        Serial.print(uid[i], HEX); Serial.print(" ");
        Serial1.print(uid[i], HEX); Serial1.print(" ");
      }
      Serial.println();
      Serial1.println();
    }
  } else {
    if (tag_present) {
      no_read_count++;
      if (no_read_count >= MAX_NO_READS) {
        tag_present = false;
        last_uid_len = 0;
        memset(last_uid, 0, sizeof(last_uid));
        Serial.println("Tag removed");
        Serial1.println("Tag removed");
      }
    }
  }

  delay(100);  // Adjust as needed
}
