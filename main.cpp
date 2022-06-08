#include "population.hpp"
using namespace xdu_ai;
struct F1 {
  double operator()(std::array<double, 2> arr) {
    double x1 = arr[0], x2 = arr[1];
    constexpr double PI = 3.14159265;
    return 21.5 + x1 * std::sin(4 * PI * x1) + x2 * std::sin(20 * PI * x2);
  }
};

double test_problem1(int init, int gen) {
  using std::make_pair;
  constexpr std::array<range_type, 2> var_ranges{make_pair(-3.0, 12.1),
                                                 make_pair(4.1, 5.8)};
  constexpr double precision = 1e-4;
  constexpr my_size_type x1l = bitcount(var_ranges[0].first,
                                        var_ranges[0].second, precision),
                         x2l = bitcount(var_ranges[1].first,
                                        var_ranges[1].second, precision);
  using IndividualType = individual<x1l, x2l>;
  roulette_policy<IndividualType, F1, std::identity> popula(init, var_ranges);
  for (int i = 0; i < gen; i++) {
    popula.next_generation();
  }
  auto [vars, res] = popula.get_best_result();
  println(vars);
  std::cout << "res: " << res << std::endl;
  std::cout << "best: " << popula.best << std::endl;
  return res;
}

struct F2 {
  double operator()(std::array<double, 4> arr) {
    double x1 = arr[0], x2 = arr[1], x3 = arr[2], x4 = arr[3];
    return 1.0 / (x1 * x1 + x2 * x2 + x3 * x3 + x4 * x4 + 1);
  }
};

double test_problem2(int init, int gen) {
  using std::make_pair;
  constexpr std::array<range_type, 4> var_ranges{
      make_pair(-5.0, 5.0), make_pair(-5.0, 5.0), make_pair(-5.0, 5.0),
      make_pair(-5.0, 5.0)};
  constexpr double precision = 1e-4;
  constexpr my_size_type x1l = bitcount(var_ranges[0].first,
                                        var_ranges[0].second, precision),
                         x2l = bitcount(var_ranges[0].first,
                                        var_ranges[0].second, precision),
                         x3l = bitcount(var_ranges[0].first,
                                        var_ranges[0].second, precision),
                         x4l = bitcount(var_ranges[0].first,
                                        var_ranges[0].second, precision);
  using IndividualType = individual<x1l, x2l, x3l, x4l>;
  roulette_policy<IndividualType, F2, std::identity> popula(init, var_ranges);
  for (int i = 0; i < gen; i++) {
    popula.next_generation();
  }
  auto [vars, res] = popula.get_best_result();
  println(vars);
  std::cout << "res: " << res << std::endl;
  std::cout << "best: " << popula.best << std::endl;
  return res;
}

double test_problem(int init, int gen) { return test_problem1(init, gen); }

int main(int argc, char **argv) {
  int init = 1200, gen = 50;
  if (argc >= 3) {
    init = ::atoi(argv[1]);
    gen = ::atoi(argv[2]);
  }
  MUTATE_POSB = 0.4;
  double r = test_problem(init, gen);
  MUTATE_POSB = 0.99;
  r = test_problem(init, gen);
  MUTATE_POSB = 0.1;
  r = test_problem(init, gen);

  return 0;
}