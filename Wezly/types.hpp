// types.hpp
#pragma once

#include <cstdint>
#include <functional>

using ElementID = uint64_t;
using Time = uint64_t;
using TimeOffset = int64_t;
using ProbabilityGenerator = std::function<double()>;