#include <random>
#include <algorithm>

#include "kernel.hpp"

int main(int argc, char** argv)
{
  // Randomize input vector
  //std::random_device seed;
  auto seed = []() { return 1; };
  std::default_random_engine engine(seed());
  std::uniform_int_distribution<int> dist(-1000, 1000);

  int in[SIZE], out[SIZE], ref[SIZE];

  for (int i=0; i<SIZE; i++) {
      in[i] = dist(engine);
  }

  kernel(in, out);

  std::copy(in, in + SIZE, ref);
  std::sort(ref, ref + SIZE);

  // Check
  bool pass = true;
  for (int i=0; i<SIZE; i++) {
    if (out[i] != ref[i]) pass = false;
  }
  if (!pass) return EXIT_FAILURE;
}