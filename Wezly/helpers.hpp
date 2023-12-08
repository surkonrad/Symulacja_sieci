// helpers.hpp
#pragma once

#include <random>
#include <functional>

using ProbabilityGenerator = std::function<double()>;

extern ProbabilityGenerator probability_generator;

