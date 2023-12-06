// package.hpp
#pragma once

#include <set>
#include <iostream>
#include "types.hpp"

class Package {

public:
    Package();

    explicit Package(ElementID ID) : ID_(ID) { assigned_IDs.insert(ID_); }

    Package(Package &&package)  noexcept : ID_(package.ID_) {}

    Package &operator=(Package &&package) noexcept ;

    ElementID get_id() const { return ID_; }

    ~Package();

private:
    static std::set<ElementID> assignedIDs;
    static std::set<ElementID> freedIDs;

    // Dodatkowe metody i pola wg potrzeb
};

