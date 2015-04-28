#include <iostream>
#include <random>
#include "markov.hpp"

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<int> seq {0,1,2,0,3,4,5,6,7,8,0};
  markov::markov_chain<int, 1> chain = markov::markov_create<int, 1>();
  markov::read_seq(chain, seq.begin(), seq.end());
  int x = 0;
  for (int i = 0; i < 20; ++i) {
    std::cout << (x = chain.generate(x, gen)) << std::endl;
  }
  return 0;
}
