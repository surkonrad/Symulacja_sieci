// package.hpp
#pragma once

#include <set>
#include <iostream>

class Package {
public:
    using ElementID = int; // Przykładowy typ, dostosuj go do swoich potrzeb

private:
    static std::set<ElementID> assignedIDs;
    static std::set<ElementID> freedIDs;

    ElementID id;

public:
    Package();

    ~Package();

    ElementID getID() const;

    // Dodatkowe metody i pola wg potrzeb
};