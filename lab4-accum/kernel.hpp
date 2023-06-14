#pragma once

extern "C" {
void kernel(const float in[1024], const int size, float *out);
}