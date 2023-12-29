//
// Created by peter on 19.12.2021.
//

//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
#ifndef SIECI_SIMULATION_HPP
#define SIECI_SIMULATION_HPP
#include "factory.hpp"

extern void simulate(Factory & f, TimeOffset d, std::function<void(Factory&, Time)> rf);

#endif //SIECI_SIMULATION_HPP

//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
