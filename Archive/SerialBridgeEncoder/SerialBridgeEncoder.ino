#define ENCODER_CLK 4
#define ENCODER_DT  1
#define ENCODER_SW  0

int lastState = HIGH;
bool lastBtnState = HIGH;
int lastPos = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 20, 21); // TX=21, RX=20
  delay(100);
  Serial.println("Encoder + Serial1 Test");

  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP);

  lastState = digitalRead(ENCODER_CLK);
}

void loop() {
  // Rotary turn detection
  int clkState = digitalRead(ENCODER_CLK);
  if (clkState != lastState) {
    if (digitalRead(ENCODER_DT) != clkState) {
      lastPos++;
      Serial.println("Right");
      Serial1.println("Right");
    } else {
      lastPos--;
      Serial.println("Left");
      Serial1.println("Left");
    }
  }
  lastState = clkState;

  // Button press detection
  bool btn = digitalRead(ENCODER_SW);
  if (btn == LOW && lastBtnState == HIGH) {
    Serial.println("Button Pressed");
    Serial1.println("Button Pressed");
  }
  lastBtnState = btn;

  delay(2); // debounce
}
