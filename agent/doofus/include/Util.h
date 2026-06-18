#pragma once

#include <random>

class Util {
public:
  static int rand(int lo, int hi);
  static std::string squareName(int sq);

private:
  static std::mt19937 rng;
  static std::random_device rd;
};
