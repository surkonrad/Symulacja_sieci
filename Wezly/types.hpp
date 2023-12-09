// types.hpp

#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <list>
#include "package.hpp"

using size_type=std::size_t;
using ElementID = unsigned int;
using Time = int;
using TimeOffset = int;
using ProbabilityGenerator = std::function<double()>;

#endif

// Tutaj możesz dodawać inne definicje, aliasy itp., jeśli są potrzebne