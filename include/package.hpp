#pragma once

#include <set>
#include "types.hpp"
#include <iostream>

class Package {
public:
    Package();

    explicit Package(ElementID ID);

    Package(Package &&package) noexcept;

    Package &operator=(Package &&package) noexcept;

    ElementID get_id() const;

    ~Package();

private:
    static std::set<ElementID> assignedIDs;
    static std::set<ElementID> freedIDs;

    ElementID ID_; // Poprawa: dodanie pola ID_
};

