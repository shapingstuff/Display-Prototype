#pragma once

#include <Arduino.h>

void encoder_init();
void encoder_update();
int get_encoder_delta();
bool is_encoder_pressed();
