cmake_minimum_required(VERSION 3.0)
project(markov)

add_definitions(--std=c++14 -Wall -Werror -pedantic -g)

add_executable(markov main.cpp)
