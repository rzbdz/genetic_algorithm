## Genetic Algorithm for function optimization

---
usage: 
```cpp
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
```

test:
```shell
make main
```
