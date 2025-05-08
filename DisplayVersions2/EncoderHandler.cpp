#include "EncoderHandler.h"
#include <Arduino.h>

#define ENCODER_CLK 44
#define ENCODER_DT 43
#define ENCODER_SW 15

static int last_encoder_state = 0;
static int encoder_delta = 0;
static bool button_pressed = false;
static bool last_button_state = HIGH;

void encoder_init() {
    pinMode(ENCODER_CLK, INPUT_PULLUP);
    pinMode(ENCODER_DT, INPUT_PULLUP);
    pinMode(ENCODER_SW, INPUT_PULLUP);
    last_encoder_state = digitalRead(ENCODER_CLK);
    //Serial.println("Encoder Enabled");
}

void encoder_update() {
    int clk_state = digitalRead(ENCODER_CLK);
    int dt_state = digitalRead(ENCODER_DT);
    if (clk_state != last_encoder_state) {
        if (dt_state != clk_state) {
            encoder_delta++;
        } else {
            encoder_delta--;
        }
    }
    last_encoder_state = clk_state;

    bool current_button_state = digitalRead(ENCODER_SW);
    if (last_button_state == HIGH && current_button_state == LOW) {
        button_pressed = true;
    }
    last_button_state = current_button_state;
}

int get_encoder_delta() {
    int delta = encoder_delta;
    encoder_delta = 0;
    return delta;
}

bool is_encoder_pressed() {
    if (button_pressed) {
        button_pressed = false;
        return true;
    }
    return false;
}
