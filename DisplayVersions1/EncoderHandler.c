#include "EncoderHandler.h"

RotaryEncoder::RotaryEncoder(int pinA, int pinB) : encoder(pinA, pinB) {}

void RotaryEncoder::begin() {
  lastPosition = encoder.read();
}

void RotaryEncoder::update() {
  long newPosition = encoder.read();
  if (newPosition != lastPosition) {
    int diff = newPosition - lastPosition;
    lastPosition = newPosition;
    // Save the direction globally or act on it here
  }
}

int RotaryEncoder::getDirection() {
  long newPosition = encoder.read();
  int direction = 0;
  if (newPosition != lastPosition) {
    direction = (newPosition > lastPosition) ? 1 : -1;
    lastPosition = newPosition;
  }
  return direction;
}
