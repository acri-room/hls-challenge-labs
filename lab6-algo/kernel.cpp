#include "kernel.hpp"

void kernel(
  const float in[1024],
  const int size,
  float mean[1],
  float vari[1]
) {
  // Calc mean
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += in[i];
  }
  mean[0] = sum / size;

  // Calc var
  sum = 0;
  for (int i = 0; i < size; i++) {
    sum += (in[i] - mean[0]) * (in[i] - mean[0]);
  }
  vari[0] = sum / size;
}