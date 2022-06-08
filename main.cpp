#include "population.hpp"
using namespace xdu_ai;
struct f1 {
  double operator()(std::array<double, 2> arr) {
    double x1 = arr[0], x2 = arr[1];
    constexpr double PI = 3.14159265;
    return 21.5 + x1 * std::sin(4 * PI * x1) + x2 * std::sin(20 * PI * x2);
  }
};

double test_problem1(int init, int gen) {
  constexpr range_array<2> var_ranges{range_type(-3.0, 12.1),
                                      range_type(4.1, 5.8)};
  constexpr double precision = 1e-4;
  constexpr my_size_type x1l = bitcount(var_ranges[0], precision),
                         x2l = bitcount(var_ranges[1], precision);
  roulette_policy<individual<x1l, x2l>, f1, std::identity> p(init, var_ranges);
  for (int i = 0; i < gen; i++) {
    p.next_generation();
  }
  auto [vars, res] = p.get_best_result();
  print_double_array(vars);
  std::cout << "res: " << res << std::endl;
  std::cout << "best: " << p.best << std::endl;
  return res;
}

struct f2 {
  double operator()(std::array<double, 4> arr) {
    double x1 = arr[0], x2 = arr[1], x3 = arr[2], x4 = arr[3];
    return 1.0 / (x1 * x1 + x2 * x2 + x3 * x3 + x4 * x4 + 1);
  }
};

double test_problem2(int init, int gen) {
  using std::make_pair;
  constexpr range_array<4> var_ranges{
      range_type(-5.0, 5.0), range_type(-5.0, 5.0), range_type(-5.0, 5.0),
      range_type(-5.0, 5.0)};
  constexpr double precision = 1e-4;
  constexpr my_size_type xl = bitcount(var_ranges[0], precision);
  roulette_policy<individual<xl, xl, xl, xl>, f2, std::identity> p(init,
                                                                   var_ranges);
  for (int i = 0; i < gen; i++) {
    p.next_generation();
  }
  auto [vars, res] = p.get_best_result();
  print_double_array(vars);
  std::cout << "res: " << res << std::endl;
  std::cout << "best: " << p.best << std::endl;
  return res;
}

double problem2_brute_force() {
  double best = 0;
  for (double i = -5.0; i <= 5.0; i += 1e-2) {
    for (double j = -5.0; j <= 5.0; j += 1e-2) {
      for (double k = -5.0; k <= 5.0; k += 1e-2) {
        for (double l = -5.0; l <= 5.0; l += 1e-2) {
          best = std::max(best, f2()({i, j, k, l}));
        }
      }
    }
  }
  std::cout << best << "\n";
  return best;
}

double test_problem(int init, int gen) {
  test_problem2(init, gen);
  return test_problem1(init, gen);
}

int main(int argc, char **argv) {
  int init = 1200, gen = 50;
  if (argc >= 3) {
    init = ::atoi(argv[1]);
    gen = ::atoi(argv[2]);
  }
  // MUTATE_POSB = 0.4;
  // test_problem(init, gen);
  // MUTATE_POSB = 0.99;
  // test_problem(init, gen);
  MUTATE_POSB = 0.1;
  test_problem(init, gen);
  // problem2_brute_force();
  return 0;
}