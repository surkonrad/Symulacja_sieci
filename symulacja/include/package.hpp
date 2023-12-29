//
// Created by peter on 19.12.2021.
//
//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
#ifndef SIECI_PACKAGE_HPP
#define SIECI_PACKAGE_HPP

#include "types.hpp"
#include <utility>


class Package {
public:
    explicit Package() noexcept;
    Package(ElementID ID);
    Package(Package && package) noexcept;
    Package & operator=(Package && package) noexcept;
    ElementID get_id() const { return ID_; }
    ~Package();
private:
    ElementID ID_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif //SIECI_PACKAGE_HPP


//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
