#pragma once
#include <Encoder.h>

class RotaryEncoder {
  public:
    RotaryEncoder(int pinA, int pinB);
    void begin();
    void update();
    int getDirection(); // +1 = CW, -1 = CCW, 0 = no change

  private:
    Encoder encoder;
    long lastPosition = 0;
};