#include "kernel.hpp"

void kernel(const int in[SIZE], int out[SIZE]) {
  // Copy
  int tmp[SIZE];
#pragma HLS ARRAY_PARTITION variable=tmp type=cyclic factor=2 dim=1
  for (int i = 0; i < SIZE; i++) {
    tmp[i] = in[i];
  }

  // Bubble sort
  for (int i = 0; i < SIZE-1; i++) { 
    for (int j = SIZE-1; j > i; j--) {
      if (tmp[j] < tmp[j-1]) { 
        int t = tmp[j];
        tmp[j] = tmp[j-1];
        tmp[j-1] = t; 
      } 
    } 
  }

  // Copy
  for (int i = 0; i < SIZE; i++) {
    out[i] = tmp[i];
  }
}
