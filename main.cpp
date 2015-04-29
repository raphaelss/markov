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
#include <fstream>
#include <random>
#include <string>
#include "markov.hpp"

void read_stream(std::istream &in,
    markov::markov_chain_nth<std::string> &chain);

int main(int argc, char **argv) {
  if (argc < 4) {
    std::cerr << "usage: markov order n file1 [file2 file3 ...fileN]\n";
    return 1;
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  auto chain = markov::markov_create<std::string>(std::stoi(argv[1]));
  for (int i = 3; i < argc; ++i) {
    std::ifstream fin(argv[i]);
    read_stream(fin, chain);
  }
  std::vector<std::string> vec(chain.order(), "");
  markov::generate_seq(gen, chain, vec, std::stoi(argv[2]), [](auto &x) {
    std::cout << x << " ";
  });
  std::cout << std::endl;
  return 0;
}

void read_stream(std::istream &in,
    markov::markov_chain_nth<std::string> &chain) {
  std::vector<std::string> seq(chain.order(), "");
  std::string tmp;
  while (in >> tmp) {
    seq.push_back(tmp);
    chain.add_from_seq(seq.begin());
    seq.erase(seq.begin());
  }
}
