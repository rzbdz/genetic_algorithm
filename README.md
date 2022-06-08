## Genetic Algorithm for function optimization

---
example:

Optimize the input so as to get the maximum output of function below:
```shell
                       1
f_2 = ----------------------------------
      x_1^2 + x_2^2 + x_3^2 + x_4 ^2 + 1
```
It's obvious that:
```shell
argmax(f_2) = (0, 0, 0, 0)
max(f_2) = 1
```
We set the evaluate function to be `std::identity`, exploring range of input variables to be `[-5.0, 5.0]` with variable precision be `10e-4`, use roulette policy to do selection:

```cpp
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
```

test:
```shell
make main
```

50 initial individuals, evolve 100 generations:
```
(-0.000801093, 3.81473e-05, -0.0100327, 0.00110627)
res: 0.999897
best: 0.999897
```
30 iniial individuals, evolve 1000 generations:
```
(0.000343325, -3.81473e-05, 0.000114442, -0.000114442)
res: 1
best: 1
```
