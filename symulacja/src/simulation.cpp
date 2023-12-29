//
// Created by peter on 19.12.2021.

//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
#include "simulation.hpp"

void simulate(Factory & f, TimeOffset d, std::function<void(Factory&, Time)> rf) {
    if(f.is_consistent()) {
        for (Time t = 1; t < d; ++t) {
            f.do_deliveries(t);
            f.do_package_passing();
            f.do_work(t);
            rf(f, t);
        }
    }
    else
        throw std::logic_error("Factory structure inconsistent");
}

//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
