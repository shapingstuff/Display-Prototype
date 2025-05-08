#include <Wire.h>
#include <Adafruit_PN532.h>

// --- Pins ---
#define SDA_PIN 8
#define SCL_PIN 9
#define ENCODER_CLK 4
#define ENCODER_DT  2  // Avoid pin 1 for stability
#define ENCODER_SW  0

// --- Encoder state ---
volatile int encoder_delta = 0;
volatile bool button_pressed = false;
int lastEncoded = 0;

// --- RFID ---
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

// --- Interrupt handlers ---
void IRAM_ATTR handle_encoder() {
  int MSB = digitalRead(ENCODER_CLK);
  int LSB = digitalRead(ENCODER_DT);
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    encoder_delta++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    encoder_delta--;

  lastEncoded = encoded;
}

void IRAM_ATTR handle_button() {
  button_pressed = true;
}

// --- RFID Task ---
void rfid_task(void *param) {
  while (1) {
    uint8_t uid[7];
    uint8_t uidLength;

    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100)) {
      Serial1.print("RFID Tag: ");
      for (uint8_t i = 0; i < uidLength; i++) {
        Serial1.print(uid[i], HEX);
        Serial1.print(" ");
      }
      Serial1.println();
    } else {
      // optional: indicate no tag
      // Serial1.println("No tag");
    }

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

// --- Setup ---
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 20, 21);

  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), handle_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_DT), handle_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_SW), handle_button, FALLING);

  // Init I2C + PN532
  Wire.begin(SDA_PIN, SCL_PIN);
  nfc.begin();
  if (!nfc.SAMConfig()) {
    Serial.println("RFID init failed!");
    Serial1.println("RFID init failed!");
    while (1) delay(10);
  }
  Serial1.println("RFID Ready");

  // Start RFID task on core 0
  xTaskCreatePinnedToCore(
    rfid_task,       // Task function
    "RFIDTask",      // Name
    4096,            // Stack size
    NULL,            // Params
    1,               // Priority
    NULL,            // Task handle
    0                // Core
  );
}

// --- Main loop ---
void loop() {
  if (encoder_delta != 0) {
    int delta = encoder_delta;
    encoder_delta = 0;
    Serial1.println(delta);  // Use +1 or -1 format
  }

  if (button_pressed) {
    button_pressed = false;
    Serial1.println("Button Pressed");
  }

  delay(5);  // Keep loop light
}
