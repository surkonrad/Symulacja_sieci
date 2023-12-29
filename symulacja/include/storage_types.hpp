//
// Created by peter on 19.12.2021.
//

#ifndef SIECI_STORAGE_TYPES_HPP
#define SIECI_STORAGE_TYPES_HPP

#include "package.hpp"
#include "types.hpp"
#include <list>
#include <utility>
#include <stack>

enum class PackageQueueType {
    FIFO, LIFO
};


class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;
    virtual void push(Package && package) = 0;
    virtual bool empty() = 0;
    virtual std::size_t size() = 0;
    virtual ~IPackageStockpile() = default;
    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;
};

class IPackageQueue: public IPackageStockpile {
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;
};

class PackageQueue : public IPackageQueue {
public:
    PackageQueue(PackageQueueType package_queue_type);
    Package pop() override;
    PackageQueueType get_queue_type() override;
    void push(Package && package) override;
    const_iterator begin() const override { return products_.begin(); }
    const_iterator cbegin() const override { return products_.cbegin(); }
    const_iterator end() const override { return products_.end(); }
    const_iterator cend() const override { return products_.cend(); }
    bool empty() override { return size() == 0; }
    std::size_t size() override;
private:
    std::list<Package> products_;
    PackageQueueType packageQueueType;
};

#endif //SIECI_STORAGE_TYPES_HPP
