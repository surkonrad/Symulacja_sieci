// package.cpp
#include "package.hpp"

std::set<Package::ElementID> Package::assignedIDs;
std::set<Package::ElementID> Package::freedIDs;

Package::Package() {
    if (!freedIDs.empty()) {
        id = *freedIDs.begin();
        freedIDs.erase(freedIDs.begin());
    } else {
        id = *assignedIDs.rbegin() + 1;
    }
    assignedIDs.insert(id);
}

Package::~Package() {
    assignedIDs.erase(id);
    freedIDs.insert(id);
}

Package::ElementID Package::getID() const {
    return id;
}

// Implementacje dodatkowych metod, jeśli są potrzebne
