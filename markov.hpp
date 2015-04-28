#ifndef MARKOV_HPP_MARKOVMODEL_INCLUDED
#define MARKOV_HPP_MARKOVMODEL_INCLUDED
#include "mtp.hpp"
#include <array>
#include <vector>
#include <algorithm>

namespace markov {

template <class T, unsigned Order>
class markov_chain {
public:
  unsigned order() const {
    return Order;
  }

  template <class Iter>
  void add(Iter it, double prob=1) {
    Iter last = it + Order;
    std::copy(it, last, _tmp.begin());
    _mtp.add(_tmp, *last, prob);
  }

  template <class Iter>
  void set(Iter it, double prob) {
    Iter last = it + Order;
    std::copy(it, last, _tmp.begin());
    _mtp.set(_tmp, *last, prob);
  }

private:
  mtp<std::array<T, Order>, T> _mtp;
  std::array<T, Order> _tmp;
};

template <class T>
class markov_chain<T, 1> {
public:
  unsigned order() const {
    return 1;
  }

  template <class Iter>
  void add(Iter it, double prob=1) {
    _mtp.add(*it, *(it + 1), prob);
  }

  template <class Iter>
  void set(Iter it, double prob) {
    _mtp.set(*it, *(it + 1), prob);
  }

  template <class RndGen>
  T generate(T &current, RndGen &gen) {
    return _mtp.generate(current, gen);
  }

private:
  mtp<T, T> _mtp;
};

template <class T>
class markov_chain<T, 0> {
public:
  unsigned order() const {
    return 0;
  }

  template <class Iter>
  void add(Iter it, double prob=1) {
    _mtp.add(*it, prob);
  }

  template <class Iter>
  void set(Iter it, double prob) {
    _mtp.set(*it, prob);
  }

private:
  prob_table<T> _mtp;
};

template <class T>
class markov_chain_nth {
  markov_chain_nth(unsigned order_arg): _order(order_arg) {}

  unsigned order() const {
    return _order;
  }

  template <class Iter>
  void add(Iter it, double prob=1) {
    Iter last = it + _order;
    std::copy(it, last, _tmp.begin());
    _mtp.add(_tmp, *last, prob);
  }

  template <class Iter>
  void set(Iter it, double prob=1) {
    Iter last = it + _order;
    std::copy(it, last, _tmp.begin());
    _mtp.set(_tmp, *last, prob);
  }

private:
  const unsigned _order;
  mtp<std::vector<T>, T> _mtp;
  std::vector<T> _tmp;
};

template <class T, unsigned Num>
markov_chain<T, Num> markov_create() {
  return std::move(markov_chain<T, Num>());
}

template <class T>
markov_chain_nth<T> markov_create(unsigned order) {
  return std::move(markov_chain_nth<T>(order));
}

template <class T, class Iter>
void read_seq(T &chain, Iter begin, Iter end) {
  if (end - begin <= chain.order()) {
    return;
  }
  Iter last_begin = end - chain.order();
  for (; begin != last_begin; ++begin) {
    chain.add(begin);
  }
}

}

#endif
