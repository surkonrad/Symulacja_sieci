// package.cpp
#include "package.hpp"
#include <iostream>
#include <set>

std::set<ElementID> Package::assigned_IDs {};
std::set<ElementID> Package::freed_IDs {};

Package::Package() {
    if (!freed_IDs.empty()) {
        auto new_id = freed_IDs.begin();
        ID_ = *new_id;
        freed_IDs.erase(new_id);

    }
    else if(!assigned_IDs.empty()){

        ID_ = *assigned_IDs.end() + 1;
        assigned_IDs.insert(ID_);

    }
    else if(freed_IDs.empty() && assigned_IDs.empty()){
        ID_ = 1;
        assigned_IDs.insert(ID_);
    }
}

Package& Package::operator=(Package&& other) noexcept{
if (this == &other)
return *this;
this->ID_ = std::move(other.ID_);
return *this;
}
Package::Package(Package&& package) noexcept : ID_(std::move(package.ID_)){}


Package::~Package() {
    assigned_IDs.erase(ID_);
    freed_IDs.insert(ID_);

}

// Implementacje dodatkowych metod, jeśli są potrzebne
