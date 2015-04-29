#include <iostream>
#include <random>
#include "markov.hpp"

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<int> seq {0,1,2,0,3,4,5,6,7,8,0,1,0,3,4,5,6,7,2,0};
  markov::markov_chain_nth<int> chain = markov::markov_create<int>(2);
  markov::read_seq(chain, seq.begin(), seq.end());
  std::vector <int> vec {0, 3};
  markov::generate_seq(gen, chain, vec, 10, [](int x) {
    std::cout << x << std::endl;
  });
  return 0;
}
