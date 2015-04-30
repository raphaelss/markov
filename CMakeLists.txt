cmake_minimum_required(VERSION 3.0)
project(markov)

add_definitions(--std=c++14 -Wall -Werror -pedantic -g)

add_executable(words_markov words_markov.cpp)
add_executable(char_markov char_markov.cpp)
