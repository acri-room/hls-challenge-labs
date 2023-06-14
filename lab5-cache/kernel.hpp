#pragma once

const int SIZE = 128;

extern "C" {
void kernel(const int in[SIZE], int out[SIZE]);
}