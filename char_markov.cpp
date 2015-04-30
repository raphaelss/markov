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

#include <iostream>
#include <random>
#include "markov.hpp"

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cout << "usage: char_markov order n\n";
    return 1;
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  auto chain = markov::markov_create<char>(std::stoi(argv[1]));
  std::vector<char> vec(chain.order(), 0);
  char ch;
  while (std::cin.get(ch)) {
    vec.push_back(ch);
    chain.add_from_seq(vec.begin());
    vec.erase(vec.begin());
  }
  for (auto &c : vec) {
    c = 0;
  }
  markov::generate_seq(gen, chain, vec, std::stoi(argv[2]), [](auto &x) {
    std::cout << x;
  });
  std::cout << std::endl;
  return 0;
}
