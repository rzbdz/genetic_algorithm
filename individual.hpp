#include "config.hpp"
#include "debug.hpp"

namespace xdu_ai {

using my_size_type = decltype(std::bitset<1>().size());
/**
 * @brief represent a closed-range, [a, b]
 */
using range_type = std::pair<double, double>;

template <my_size_type N> using range_array = std::array<range_type, N>;
template <my_size_type N> using chromosome = std::bitset<N>;

/**
 * @brief count how many bit is needed to encode a range
 *        [a, b] with specific precision.
 * @param precision 1e-4 means, at least get presision of 0.0001
 * @return int a bitcount.
 */
inline MY_CONSTEXPR my_size_type bitcount(double a, double b,
                                          double precision) {
  double ra = std::ceil((b - a) / precision);
  LOG_DEBUG("%lf", ra);
  my_size_type r = std::ceil(std::log2(ra));
  return r;
}
inline MY_CONSTEXPR my_size_type bitcount(range_type ran, double precision) {
  double a = ran.first, b = ran.second;
  double ra = std::ceil((b - a) / precision);
  LOG_DEBUG("%lf", ra);
  my_size_type r = std::ceil(std::log2(ra));
  return r;
}
/**
 * @brief decode a bitset, map it into the range of [a, b],
 *        formular : ``y = a + (b-a) * (x) / (2^N -1)``
 *                   with x = unsigned interpretation of bitset.
 * @return double decoded value.
 */
template <my_size_type N>
inline MY_CONSTEXPR double decode(double a, double b, const chromosome<N> &xb) {
  unsigned long long x = xb.to_ullong();
  double delta = ((double)x / (double)((1 << N) - 1));
  return a + ((b - a) * delta);
}
template <my_size_type N> inline chromosome<N> random_chromosome() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(BITSET_POSB);
  std::bitset<N> bs;
  for (int i = 0; i < N; i++) {
    bs.set(i, d(gen));
  }
  return bs;
}
/**
 * @brief a chromosome class, represent multiple bitsets (which correspond to
 * multiple free-varialblesm)
 *
 * @tparam Ns the bitset size of each bitset (just use bitcount to get one).
 */
template <my_size_type... Ns> class individual {
public:
  static constexpr decltype(std::size({Ns...})) seg_cnt = std::size({Ns...});
  using decode_type = std::array<double, seg_cnt>;
  /**
   * @brief construct chromosome from string
   *
   * @tparam _Str
   */
  template <typename... _Str> individual(_Str... args) {
    datas_ = std::make_tuple<std::bitset<Ns>...>(
        std::bitset<Ns>{args.data(), args.size()}...);
  }
  /**
   * @brief Construct a new chromosome, with random bit set
   */
  individual() {
    datas_ = std::make_tuple<std::bitset<Ns>...>(random_chromosome<Ns>()...);
    LOG_DEBUG("seg_cnt: %ld", seg_cnt);
  }

private:
  std::tuple<std::bitset<Ns>...> datas_;
  mutable decode_type cache_{};
  mutable bool cache_valid_{false};
  template <my_size_type decodeN>
  constexpr void decode_helper(const std::array<range_type, seg_cnt> &range_arr,
                               std::array<double, seg_cnt> &fill_arr) const {
    auto a = range_arr[decodeN].first;
    auto b = range_arr[decodeN].second;
    fill_arr[decodeN] = xdu_ai::decode(a, b, std::get<decodeN>(datas_));
    if constexpr (decodeN > 0) {
      decode_helper<decodeN - 1>(range_arr, fill_arr);
    }
  }

  template <my_size_type N> constexpr void mutate_helper() {
    auto &ref = std::get<N>(datas_);
    for (int i = 0; i < MUTATE_SWAP; i++) {
      auto lhs = randint(0, ref.size());
      auto rhs = randint(0, ref.size());
      bool tmp = ref[lhs];
      ref.set(lhs, ref[rhs]);
      ref.set(rhs, tmp);
    }
    for (int i = 0; i < ref.size(); i++) {
      if (rand01real() < MUTATE_POSB) {
        ref.flip(i);
      }
    }
    if constexpr (N > 0) {
      mutate_helper<N - 1>();
    }
  }

  template <my_size_type N>
  static constexpr void cross_helper(individual &a, individual &b) {
    auto &ref1 = std::get<N>(a.datas_);
    auto &ref2 = std::get<N>(b.datas_);
    for (int i = 0; i < MUTATE_SWAP; i++) {
      auto l = randint(0, ref1.size());
      auto r = randint(0, ref2.size());
      if (l > r)
        std::swap(l, r);
      for (int i = l; i <= r; i++) {
        bool tmp = ref1[i];
        ref1[i] = ref2[i];
        ref2[i] = tmp;
      }
    }
    if constexpr (N > 0) {
      cross_helper<(N - 1)>(a, b);
    }
  }

public:
  /**
   * @brief decode the chromosome to an array of double as output.
   *
   * @param range_arr ranges(`pair<double,double>`) mapped to each bitset
   * member.
   * @return decode_type
   */
  constexpr decode_type
  decode(const std::array<range_type, seg_cnt> &range_arr) const {
    if (cache_valid_)
      return cache_;
    decode_helper<(seg_cnt - 1)>(range_arr, cache_);
    cache_valid_ = true;
    return cache_;
  }

  void mutate() {
    cache_valid_ = false;
    mutate_helper<seg_cnt - 1>();
  }

  friend std::ostream &operator<<(std::ostream &os, const individual &ind) {
    os << "part1: " << std::get<0>(ind.datas_)
       << "\npart2: " << std::get<1>(ind.datas_) << "\n";
    return os;
  }

  static constexpr void inline cross(individual &a, individual &b) {
    a.cache_valid_ = false;
    b.cache_valid_ = false;
    cross_helper<seg_cnt - 1>(a, b);
  }
};

/**
 * @brief helper to print a result (of decoded free variables)
 *
 * @param arr a result come from `chromosome::decode`.
 */
template <my_size_type N>
inline void print_double_array(const std::array<double, N> &arr) {
  std::cout << "(";
  for (int i = 0; i < N; i++) {
    if (i != 0) {
      std::cout << ", ";
    }
    auto a = arr[i];
    std::cout << a;
  }
  std::cout << ")" << std::endl;
}

} // namespace xdu_ai