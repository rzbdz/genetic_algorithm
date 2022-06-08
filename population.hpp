#include "individual.hpp"
#include <algorithm>
#include <cstdlib>
#include <type_traits>

namespace xdu_ai {

/**
 * @brief
 *
 * @tparam _Policy derived class: implement `vector<Individual> select(int)`
 * function.
 * @tparam _Function `double(std::array<double, N>)`, get a target.
 * @tparam _FitFunction `double(double)`, calc a fitness of an individual,
 *          larger is better.
 */
template <typename _Individual, typename _Policy, typename _Function,
          typename _FitFunction>
class population {
protected:
  std::vector<_Individual> herd_;
  const std::array<range_type, _Individual::seg_cnt> &rg_;
  std::vector<_Individual> select(std::vector<_Individual> &asc, int how_many) {
    return static_cast<_Policy *>(this)->select(asc, how_many);
  }

public:
  double best = 0.;
  population(my_size_type init_sz,
             const std::array<range_type, _Individual::seg_cnt> r)
      : herd_(init_sz), rg_{r} {}
  /**
   * @brief impl should provide a select funtion which could produce non-unique
   * results.
   */
  void next_generation() {
    int ori = herd_.size();
    auto lam = [&](const _Individual &l, const _Individual &r) -> bool {
      return _FitFunction()(_Function()(l.decode(rg_))) >
             _FitFunction()(_Function()(r.decode(rg_)));
    };

    auto elite1 = herd_[0];
    double emax = _FitFunction()(_Function()(elite1.decode(rg_)));

    auto rs = herd_.size() - 1;

    auto selected = select(herd_, ori);

    auto now_size = selected.size();
    for (int i = 0; i + 1 < now_size; i += 2) {
      auto &copyl = selected[i], &copyr = selected[i + 1];

      double e = _FitFunction()(_Function()(copyl.decode(rg_)));
      if (e > emax) {
        emax = e;
        elite1 = copyl;
      }
      e = _FitFunction()(_Function()(copyr.decode(rg_)));
      if (e > emax) {
        emax = e;
        elite1 = copyr;
      }

      _Individual::cross(copyl, copyr);

      selected[i].mutate();
      selected[i + 1].mutate();

      herd_.emplace_back(selected[i]);
      herd_.emplace_back(selected[i + 1]);
    }

    auto new_selected = select(herd_, rs);

    new_selected.emplace_back(elite1);

    best = std::max(_Function()((elite1).decode(rg_)), best);

    std::swap(new_selected, herd_);
  }

  _Individual get_best() {
    auto elite1 = herd_[0];
    double emax = _FitFunction()(_Function()(elite1.decode(rg_)));
    for (const auto &h : herd_) {
      auto e = _FitFunction()(_Function()(h.decode(rg_)));
      if (e > emax) {
        emax = e;
        elite1 = h;
      }
    }
    return elite1;
  }

  std::pair<typename _Individual::decode_type, double> get_best_result() {
    auto elite1 = herd_[0];
    double emax = _FitFunction()(_Function()(elite1.decode(rg_)));
    for (const auto &h : herd_) {
      auto e = _FitFunction()(_Function()(h.decode(rg_)));
      if (e > emax) {
        emax = e;
        elite1 = h;
      }
    }
    return std::make_pair(elite1.decode(rg_), emax);
  }
};

/**
 * @brief roulette policy for selecting who to be pregnant.
 *
 * @tparam Function `double(std::array<double, N>)`, get a target.
 * @tparam FitFunction `double(double)`, calc a fitness of an individual,
 *          larger is better.
 */
template <typename Individual, typename Function, typename FitFunction>
struct roulette_policy
    : population<Individual, roulette_policy<Individual, Function, FitFunction>,
                 Function, FitFunction> {
  using Super =
      population<Individual, roulette_policy<Individual, Function, FitFunction>,
                 Function, FitFunction>;
  using Super::rg_;
  roulette_policy(my_size_type init_sz,
                  const std::array<range_type, Individual::seg_cnt> rg)
      : Super(init_sz, rg) {}
  std::vector<Individual> select(const std::vector<Individual> &asc,
                                 int howmany) {
    Individual mustHave;
    double best = 0;
    std::vector<double> poss(asc.size(), 0.0);
    double all_p = 0;
    for (int i = 0; i < asc.size(); i++) {
      double p = FitFunction()(Function()(asc[i].decode(rg_)));
      if(p > best){
        mustHave = asc[i];
        best = p;
      }
      all_p += p;
      poss[i] = p;
    }
    poss[0] /= all_p;
    poss[0] = poss[0];
    for (int i = 1; i < asc.size(); i++) {
      poss[i] /= all_p;
      poss[i] = poss[i - 1] + poss[i];
    }
    std::vector<Individual> result;

    result.reserve(howmany);
    for (int i = 0; i < howmany - 1; i++) {
      double rel = rand01real();
      auto it = std::lower_bound(poss.begin(), poss.end(), rel);
      int kth = it - poss.begin();
      result.push_back(asc[kth]);
    }
    result.push_back(mustHave);
    return result;
  }
};

} // namespace xdu_ai
