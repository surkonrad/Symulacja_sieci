// helpers.cpp
#include "helpers.hpp"
#include <random>

// Domyślny generator liczb losowych
std::default_random_engine random_engine;

// Implementacja generatora prawdopodobieństwa
ProbabilityGenerator probability_generator = []() {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(random_engine);
};
