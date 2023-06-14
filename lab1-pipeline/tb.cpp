#include <cassert>
#include <vector>
#include <random>

#include "kernel.hpp"

int main(int argc, char** argv)
{
  // Randomize input vector
  //std::random_device seed;
  auto seed = []() { return 1; };
  std::default_random_engine engine(seed());
  std::uniform_real_distribution<float> dist(-1000.0, 1000.0);

  float in[1024], out[1024];

  const int size = 1024;
  for (int i=0; i<size; i++) {
    in[i] = dist(engine);
  }

  kernel(in, out, size);

  // Check
  bool pass = true;
  for (int i=0; i<size; i++) if (in[i] * 2 != out[i]) pass = false;
  if (!pass) return EXIT_FAILURE;
}
