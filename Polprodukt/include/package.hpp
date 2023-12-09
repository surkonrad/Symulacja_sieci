#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP


#include <set>
#include "types.hpp"
#include <iostream>

class Package {
public:
    Package();


    Package(ElementID ID) : ID_(ID){}
    Package(Package&& other) noexcept;
    Package& operator=(Package&& other) noexcept;

    ~Package();

    ElementID get_id() const {return ID_;}

private:
    ElementID ID_;
    static std::set<ElementID> assignedIDs;
    static std::set<ElementID> freedIDs;


};
#endif
