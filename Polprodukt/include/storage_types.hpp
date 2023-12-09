#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include <list>
#include "types.hpp"
#include "package.hpp"

enum class PackageQueueType {
    FIFO,
    LIFO
};


class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual ~IPackageStockpile() = default;

    virtual void push(Package &&package) = 0;

    virtual const_iterator begin() const = 0;

    virtual const_iterator cbegin() const = 0;

    virtual const_iterator end() const = 0;

    virtual const_iterator cend() const = 0;

    virtual size_type size() const = 0;

    virtual bool empty() const = 0;
};


class IPackageQueue : public IPackageStockpile {
public:
    virtual Package pop() = 0;

    virtual PackageQueueType get_queue_type() const = 0;
};

class PackageQueue : public IPackageQueue {


public:
    PackageQueue(PackageQueueType type) :package_list{ std::list<Package>()}, packageQueueType{type} {}

    bool empty() const override { return size() == 0; };

    Package pop() override;

    void push(Package &&) override;

    PackageQueueType get_queue_type() const override;

    const_iterator end() const override { return package_list.end(); };

    const_iterator cend() const override { return package_list.cend(); };

    const_iterator begin() const override{ return package_list.begin(); };

    const_iterator cbegin() const override { return package_list.cbegin(); };

    size_type size() const override;

private:
    std::list<Package> packages;
    PackageQueueType queueType;
};

