#ifndef MTP_HPP_MARKOVMODEL_INCLUDED
#define MTP_HPP_MARKOVMODEL_INCLUDED
#include <map>
#include "prob_table.hpp"

namespace markov {

template <class A, class B>
class mtp {
public:
  template <class Aa = A, class Bb = B>
  void add(Aa &&a, Bb &&b, double prob=1);

  template <class Aa = A, class Bb = B>
  void set(Aa &&a, Bb &&b, double prob);

  template <class Aa = A, class ProbB = prob_table<B>>
  void merge_prob_table(A &&a, ProbB &&b);

  template <class Aa = A, class ProbB = prob_table<B>>
  void set_prob_table(Aa &&a, ProbB &&b);

  double probability(const A &a, const B &b) const;

  double get(const A &a, const B &b) const;

  const prob_table<B> &get_prob_table(const A &a) const;
  prob_table<B> &get_prob_table(const A &a);

  void has(const A &a) const;
  void has(const A &a, const B &b) const;

  void cbegin() const;
  void cend() const;

  void begin();
  void end();

  template <class RndGen>
  B next(const A &x, RndGen &gen) const;

  bool empty();

  void clear();

  void normalize();

private:
  std::map<A, prob_table<B>> _table;
};

template <class A, class B>
template <class Aa, class Bb>
void mtp<A, B>::add(Aa &&a, Bb &&b, double prob) {
  _table[a].add(b, prob);
}

template <class A, class B>
template <class Aa, class Bb>
void mtp<A, B>::set(Aa &&a, Bb &&b, double prob) {
  _table[a].set(b, prob);
}

template <class A, class B>
template <class Aa, class ProbB>
void mtp<A, B>::merge_prob_table(A &&a, ProbB &&b) {
  _table[a].merge(b);
}

template <class A, class B>
template <class Aa, class ProbB>
void mtp<A, B>::set_prob_table(Aa &&a, ProbB &&b) {
  _table[a] = std::move(b);
}

template <class A, class B>
double mtp<A, B>::probability(const A &a, const B &b) const {
  try {
    return _table.at(a).probability(b);
  } catch (std::out_of_range &e) {
    return 0;
  }
}

template <class A, class B>
double mtp<A, B>::get(const A &a, const B &b) const {
  return _table.at(a).get(b);
}

template <class A, class B>
const prob_table<B> &mtp<A, B>::get_prob_table(const A &a) const {
  return _table.at(a);
}

template <class A, class B>
prob_table<B> &mtp<A, B>::get_prob_table(const A &a) {
  return _table[a];
}

template <class A, class B>
void mtp<A, B>::has(const A &a) const {
  return _table.find(a) != _table.end();
}

template <class A, class B>
void mtp<A, B>::has(const A &a, const B &b) const {
  try {
    return _table.at(a).has(b);
  } catch (std::out_of_range &e) {
    return false;
  }
}

template <class A, class B>
void mtp<A, B>::cbegin() const {
  return _table.cbegin();
}

template <class A, class B>
void mtp<A, B>::cend() const {
  return _table.cend();
}

template <class A, class B>
void mtp<A, B>::begin() {
  return _table.begin();
}

template <class A, class B>
void mtp<A, B>::end() {
  return _table.end();
}

template <class A, class B>
template <class RndGen>
B mtp<A, B>::next(const A &x, RndGen &gen) const {
  return _table.at(x).choose(gen);
}

template <class A, class B>
bool mtp<A, B>::empty() {
  return _table.empty();
}

template <class A, class B>
void mtp<A, B>::clear() {
  _table.clear();
}

template <class A, class B>
void mtp<A, B>::normalize() {
  for (auto &pair : _table) {
    pair->second.normalize();
  }
}


}

#endif
