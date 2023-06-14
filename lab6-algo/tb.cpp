#include <cassert>
#include <random>

#include "kernel.hpp"

void golden(
  const float in[1024],
  const int size,
  double mean[1],
  double vari[1]
) {
  // 平均を計算
  double sum = 0;
  for (int i = 0; i < size; i++) {
    sum += in[i];
  }
  mean[0] = sum / size;

  // 分散を計算
  sum = 0;
  for (int i = 0; i < size; i++) {
    sum += (in[i] - mean[0]) * (in[i] - mean[0]);
  }
  vari[0] = sum / size;
}

int main(int argc, char** argv)
{
  // Randomize input vector
  //std::random_device seed;
  auto seed = []() { return 1; };
  std::default_random_engine engine(seed());
  std::normal_distribution<float> dist(100, 10);

  float in[1024];

  const int size = 1024;
  for (int i = 0; i < size; i++) {
    in[i] = dist(engine);
  }

  // Golden
  double golden_mean, golden_vari;
  golden(in, size, &golden_mean, &golden_vari);

  float mean, vari;
  kernel(in, size, &mean, &vari);

  // Check
  bool pass = true;
  if (!(std::abs(mean - golden_mean) <= 1e-2)) pass = false;
  if (!(std::abs(vari - golden_vari) <= 1e-2)) pass = false;
  if (!pass) return EXIT_FAILURE;
}
