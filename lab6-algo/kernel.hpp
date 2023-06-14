#pragma once

extern "C" {
void kernel(
  const float in[1024],
  const int size,
  float mean[1],
  float vari[1]
);
}
