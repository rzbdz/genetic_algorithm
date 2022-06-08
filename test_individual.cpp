// #define DEBUG
#include "individual.hpp"
using namespace xdu_ai;
void test_bitcount() {
  assert_eq(bitcount(-3.0, 12.1, 1e-4), 18);
  assert_eq(15, bitcount(4.1, 5.8, 1e-4));
}
void test_decode() {
  int b = 0x1529;
  std::bitset<18> bs(b);
  LOG("%lf", decode(-3.0, 12.1, bs));
  b = 0x22ed47;
  auto bs1 = std::bitset<22>(b);
  LOG("%lf", decode(-1, 2, bs1));
}
void try_bitset_usage() {
  constexpr size_t x1l = 18, x2l = 15;
  std::string_view case1 = "000001010100101001101111011111110";
  std::string_view x1 = case1.substr(0, x1l);
  std::string_view x2 = case1.substr(x1l, x2l);
  std::bitset<x1l> b1(x1.data(), x1.size());
  std::bitset<x2l> b2(x2.data(), x2.size());
  std::cout << b1 << std::endl << b2 << std::endl;
}
void test_chromosome_decode() {
  constexpr size_t x1l = 18, x2l = 15;
  std::string_view case1 = "000001010100101001101111011111110";
  std::string_view x1 = case1.substr(0, x1l);
  std::string_view x2 = case1.substr(x1l, x2l);
  individual<x1l, x2l> chrom(x1, x2);
  println(chrom.decode({range_type{-3.0, 12.1}, range_type{4.1, 5.8}}));
}
void test_problem() {
  auto f1 = [](double x1, double x2) {
    constexpr double PI = 3.14159265;
    return 21.5 + x1 * std::sin(4 * PI * x1) + x2 * std::sin(20 * PI * x2);
  };
  auto cmp = [](double l, double r) { return l < r; };
  using std::make_pair;
  constexpr std::array<range_type, 2> var_ranges{make_pair(-3.0, 12.1),
                                                 make_pair(4.1, 5.8)};
  constexpr double precision = 1e-4;
  constexpr my_size_type x1l = bitcount(var_ranges[0].first,
                                        var_ranges[0].second, precision),
                         x2l = bitcount(var_ranges[1].first,
                                        var_ranges[1].second, precision);
  std::string_view case1 = "000001010100101001101111011111110";
  std::string_view x1 = case1.substr(0, x1l);
  std::string_view x2 = case1.substr(x1l, x2l);
  individual<x1l, x2l> chrom(x1, x2);
  println(chrom.decode(var_ranges));
  individual<x1l, x2l> c1;
  println(c1.decode(var_ranges));
  individual<x1l, x2l> c2;
  println(c2.decode(var_ranges));
  c2.mutate();
  LOG("mutated: ");
  println(c2.decode(var_ranges));
  LOG("cross c1 and c2, before: ");
  std::cout << "c1: " << c1 << "\nc2: " << c2 << std::endl;
  individual<x1l, x2l>::cross(c1, c2);
  LOG("crossed c1 and c2, after: ");
  std::cout << "c1: " << c1 << "\nc2: " << c2 << std::endl;
  println(c1.decode(var_ranges));
  println(c2.decode(var_ranges));
}
int main() {
  test_bitcount();
  test_decode();
  // test_chromosome_decode();
  test_problem();
  return 0;
}