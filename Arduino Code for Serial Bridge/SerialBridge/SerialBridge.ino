void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Started");
  Serial1.begin(115200,SERIAL_8N1, 20,21); //int8_t rxPin=4, int8_t txPin=5 }
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println("Looping 0");
delay(500);
Serial1.println("Looping 1");
delay(500);
}