#include "kernel.hpp"

void kernel(const float in[1024], const int size, float *out) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += in[i];
  }
  *out = sum;
}
