#ifndef SIECI_SIMULATION_HPP
#define SIECI_SIMULATION_HPP
#include "factory.hpp"

extern void simulate(Factory & f, TimeOffset d, std::function<void(Factory&, Time)> rf);

#endif //SIECI_SIMULATION_HPP