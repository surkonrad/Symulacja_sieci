//
// Created by peter on 19.12.2021.
//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
#include "package.hpp"

std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

Package::Package() noexcept {
    if (assigned_IDs.empty() && freed_IDs.empty()) {
        ID_ = 1;
        assigned_IDs.insert(ID_);
    }
    else if (!freed_IDs.empty()) {
        ID_ = freed_IDs.erase(*freed_IDs.begin());
    }
    else if (!assigned_IDs.empty()) {
        ID_ = *assigned_IDs.end() + 1;
        assigned_IDs.insert(ID_);
    }
}

Package::Package(ElementID ID) : ID_(ID) {}

Package::Package(Package&& package) noexcept : ID_(std::move(package.ID_)){}

Package & Package::operator=(Package&& package) noexcept {
    if (this == &package)
        return *this;
    this->ID_ = std::move(package.ID_);
    return *this;
}

Package::~Package() {
    freed_IDs.insert(ID_);
    assigned_IDs.erase(ID_);
}


//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
