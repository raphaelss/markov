/*
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
 */

#ifndef PROB_TABLE_HPP_MARKOVMODEL_INCLUDED
#define PROB_TABLE_HPP_MARKOVMODEL_INCLUDED
#include <unordered_map>
#include <random>

namespace markov {

template <class T>
class prob_table {
public:
  prob_table(): _table(), _sum(0) {}

  double probability(const T &x) const {
    try {
      return _table.at(x);
    } catch (std::out_of_range &e) {
      return 0;
    }
  }

  double get(const T &x) const {
    return _table.at(x);
  }

  double prob_sum() const {
    return _sum;
  }

  template <class Tt>
  void (Tt &&x, double prob) {
    _table[x] += prob;
    _sum += prob;
  }

  template <class Tt>
  void set(Tt &&x, double prob) {
    _sum = _sum - probability(x) + prob;
    _table[x] = prob;
  }

  bool has(const T &x) const {
    return _table.find(x) != _table.end();
  }

  bool empty() const {
    return _table.empty();
  }

  void clear() {
    _table.clear();
    _sum = 0;
  }

  template <class RndGen>
  T choose(RndGen &gen) const {
    std::uniform_real_distribution<double> dist(0, _sum);
    auto it = _table.begin();
    for (double r = dist(gen) - it->second; r > 0; ++it, r -= it->second);
    return it->first;
  }

  void normalize(double total=1) {
    for (auto &pair : _table) {
      pair.second /= _sum;
    }
    _sum = 1;
  }

  void cbegin() const {
    return _table.cbegin();
  }

  void cend() const {
    return _table.cend();
  }

  void merge(const prob_table<T> &other) {
    for (const auto &pair : other._table) {
      _table[pair->first] += other._table[pair->second];
    }
  }

  void move_merge(prob_table<T> &&other) {
    for (auto &pair : other._table) {
      _table[std::move(pair->first)] += other._table[pair->second];
    }
  }

private:
  std::unordered_map<T, double> _table;
  double _sum;
};

#endif
