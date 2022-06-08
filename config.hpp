#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <concepts>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <type_traits>
#include <utility>
// decide posb to initialize chromosome, set a bit to 0 or 1
inline double BITSET_POSB = 0.5;
// decide how many swaps of two bits in a chromosome
inline int MUTATE_SWAP = 0;
// how many flips of a bit in a chromosome
inline int MUTATE_POLICY = 21;
// decide if a individual would mutate
inline double MUTATE_POSB = 0.8;
// decide if a chormosome in individual cross or not
#define CROSS_POSB 0.5
#ifdef DEBUG
#define MY_CONSTEXPR
#else
#define MY_CONSTEXPR constexpr
#endif

inline double rand01real() {
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<double> distr(0., 1.);
  return distr(eng);
}

inline double randint(int l, int r) {
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_int_distribution<int> distr(l, r - 1);
  return distr(eng);
}