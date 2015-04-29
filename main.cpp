#include <iostream>
#include <random>
#include "markov.hpp"

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<int> seq {0,1,2,0,3,4,5,6,7,8,0,1,0,3,4,5,6,7,2,0};
  markov::markov_chain_nth<int> chain = markov::markov_create<int>(2);
  markov::read_seq(chain, seq.begin(), seq.end());
  std::vector <int> vec {0, 1};
  int tmp;
  for (int i = 0; i < 20; ++i) {
    tmp = chain.next(vec, gen);
    vec[0] = vec[1];
    vec[1] = tmp;
    std::cout << tmp << std::endl;
  }
  return 0;
}
