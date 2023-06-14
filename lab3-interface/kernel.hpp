#pragma once

extern "C" {
void kernel(const float in[1024], float out[1024], int size);
}
