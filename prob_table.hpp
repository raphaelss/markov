#ifndef PROB_TABLE_HPP_MARKOVMODEL_INCLUDED
#define PROB_TABLE_HPP_MARKOVMODEL_INCLUDED
#include <unordered_map>
#include <random>

namespace markov {

template <class T>
class prob_table {
public:
  prob_table();

  double probability(const T &x) const;

  double get(const T &x) const;

  double prob_sum() const;

  template <class Tt>
  void add(Tt &&x, double prob=1);

  template <class Tt>
  void set(Tt &&x, double prob);

  bool has(const T &x) const;

  bool empty();

  void clear();

  template <class RndGen>
  T choose(RndGen &gen) const;

  void normalize(double total=1);

  void cbegin() const;
  void cend() const;

  void merge(const prob_table<T> &other);

  void move_merge(prob_table<T> &&other);

private:
  std::unordered_map<T, double> _table;
  double _sum;
};

template <class T>
prob_table<T>::prob_table(): _table(), _sum(0) {}

template <class T>
double prob_table<T>::probability(const T &x) const {
  try {
    return _table.at(x);
  } catch (std::out_of_range &e) {
    return 0;
  }
}

template <class T>
double prob_table<T>::get(const T &x) const {
  return _table.at(x);
}

template <class T>
double prob_table<T>::prob_sum() const {
  return _sum;
}

template <class T>
template <class Tt>
void prob_table<T>::add(Tt &&x, double prob) {
  _table[x] += prob;
  _sum += prob;
}

template <class T>
template <class Tt>
void prob_table<T>::set(Tt &&x, double prob) {
  _sum = _sum - probability(x) + prob;
  _table[x] = prob;
}

template <class T>
bool prob_table<T>::has(const T &x) const {
  return _table.find(x) != _table.end();
}

template <class T>
bool prob_table<T>::empty() {
  return _table.empty();
}

template <class T>
void prob_table<T>::clear() {
  _table.clear();
  _sum = 0;
}

template <class T>
template <class RndGen>
T prob_table<T>::choose(RndGen &gen) const {
  std::uniform_real_distribution<double> dist(0, _sum);
  auto iter = _table.begin();
  for (double r = dist(gen) - iter->second; r > 0; ++iter, r -= iter->second);
    return iter->first;
}

template <class T>
void prob_table<T>::normalize(double total) {
  for (auto &pair : _table) {
    pair.second /= _sum;
  }
  _sum = 1;
}

template <class T>
void prob_table<T>::cbegin() const {
  return _table.cbegin();
}

template <class T>
void prob_table<T>::cend() const {
  return _table.cend();
}

template <class T>
void prob_table<T>::merge(const prob_table<T> &other) {
  for (auto &pair : other._table) {
    _table[pair->first] += other._table[pair->second];
  }
}

template <class T>
void prob_table<T>::move_merge(prob_table<T> &&other) {
  for (auto &pair : other._table) {
    _table[std::move(pair->first)] += other._table[pair->second];
  }
}

}

#endif
