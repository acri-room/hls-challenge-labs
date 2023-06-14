#include "kernel.hpp"

void kernel(const float in[1024], float out[1024], int size) {
  for (int i = 0; i < size; i++) {
#pragma HLS PIPELINE off
    out[i] = in[i] * 2;
  }
}
