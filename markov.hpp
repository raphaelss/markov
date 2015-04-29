#ifndef MARKOV_HPP_MARKOVMODEL_INCLUDED
#define MARKOV_HPP_MARKOVMODEL_INCLUDED
#include "mtp.hpp"
#include <array>
#include <vector>
#include <algorithm>

namespace markov {

namespace impl {

template <class T, class Container, class Derived>
class markov_chain_base : mtp<Container, T> {
public:
  template <class Iter>
  void add_from_seq(Iter it, double prob=1) {
    Iter last = it + static_cast<Derived*>(this)->order();
    std::copy(it, last, _tmp.begin());
    this->add(_tmp, *last, prob);
  }

  template <class Iter>
  void set_from_seq(Iter it, double prob) {
    Iter last = it + static_cast<Derived*>(this)->order();
    std::copy(it, last, _tmp.begin());
    this->set(_tmp, *last, prob);
  }

private:
  Container _tmp;
};

}

template <class T, unsigned Order>
struct markov_chain
    : impl::markov_chain_base<T, std::array<T, Order>, markov_chain<T, Order>> {
  unsigned order() const {
    return Order;
  }
};

template <class T>
struct markov_chain<T, 1> : mtp<T, T> {
public:
  unsigned order() const {
    return 1;
  }

  template <class Iter>
  void add_from_seq(Iter it, double prob=1) {
    this->add(*it, *(it + 1), prob);
  }

  template <class Iter>
  void set_from_seq(Iter it, double prob) {
    this->set(*it, *(it + 1), prob);
  }

  template <class RndGen>
  T generate(const T &current, RndGen &gen) {
    return this->generate(current, gen);
  }
};

template <class T>
class markov_chain<T, 0> : prob_table<T> {
public:
  unsigned order() const {
    return 0;
  }

  template <class Iter>
  void add_from_seq(Iter it, double prob=1) {
    this->add(*it, prob);
  }

  template <class Iter>
  void set_from_seq(Iter it, double prob) {
    this->set(*it, prob);
  }

  template <class RndGen>
  T generate(RndGen &gen) {
    return this->choose(gen);
  }
};

template <class T>
class markov_chain_nth
    : impl::markov_chain_base<T, std::vector<T>, markov_chain_nth<T>> {
  markov_chain_nth(unsigned order_arg): _order(order_arg) {}

  unsigned order() const {
    return _order;
  }

private:
  const unsigned _order;
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
    chain.add_from_seq(begin);
  }
}

}

#endif
