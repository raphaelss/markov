/**
* Copyright (C) 2015  Raphael Sousa Santos, http://www.raphaelss.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef MARKOV_HPP_MARKOVMODEL_INCLUDED
#define MARKOV_HPP_MARKOVMODEL_INCLUDED
#include "mtp.hpp"
#include <array>
#include <vector>
#include <algorithm>

namespace markov {

namespace impl {

template <class T, class Container, class Derived>
class container_based : public mtp<Container, T> {
public:
  typedef Container container_type;
  typedef T type;

  template <class C = Container>
  container_based(C c = C()): _tmp(c) {}

  template <class Iter>
  void add_from_seq(Iter it, double prob=1) {
    Iter last = fill_tmp(it);
    this->add(_tmp, *last, prob);
  }

  template <class Iter>
  void set_from_seq(Iter it, double prob) {
    Iter last = fill_tmp(it);
    this->set(_tmp, *last, prob);
  }

  template <class Iter, class RndGen>
  T next_it(Iter it, RndGen &gen) {
    fill_tmp(it);
    return this->next(_tmp, gen);
  }

private:
  template <class Iter>
  Iter fill_tmp(Iter it) {
    Iter last = it + static_cast<Derived*>(this)->order();
    std::copy(it, last, _tmp.begin());
    return last;
  }

  Container _tmp;
};

}

template <class T, unsigned Order>
struct markov_chain : public impl::container_based<T, std::array<T, Order>,
    markov_chain<T, Order>> {
  unsigned order() const {
    return Order;
  }
};

template <class T>
struct markov_chain<T, 1> : public mtp<T, T> {
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

  template <class Iter, class RndGen>
  T next_it(Iter it, RndGen &gen) {
    return this->next(*it, gen);
  }
};

template <class T>
struct markov_chain<T, 0> : public prob_table<T> {
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

  template <class Iter, class RndGen>
  T next_it(Iter it, RndGen &gen) {
    return this->choose(gen);
  }

  template <class Tt, class RndGen>
  T next(Tt &&x, RndGen &gen) {
    return this->choose(gen);
  }
};

template <class T>
class markov_chain_nth : public impl::container_based<T, std::vector<T>,
    markov_chain_nth<T>> {
public:
  markov_chain_nth(unsigned order_arg)
      : impl::container_based<T, std::vector<T>, markov_chain_nth<T>>(
          std::vector<T>(order_arg)),
      _order(order_arg) {}

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

template <class RndGen, class MarkovChain, class Func>
void generate_seq(RndGen &gen, MarkovChain &chain,
    typename MarkovChain::container_type &seq, unsigned n, Func &&f) {
  for (; n > 0; --n) {
    try {
      typename MarkovChain::type x(chain.next(seq, gen));
      f(x);
      seq.erase(seq.begin());
      seq.push_back(x);
    } catch (std::out_of_range &e) {
      return;
    }
  }
}

}

#endif
