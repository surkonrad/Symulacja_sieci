// storage_types.hpp
#pragma once

#include <list>
#include <queue>
#include <map>
#include "package.hpp"
#include "types.hpp"

enum class PackageQueueType { FIFO, LIFO };

class IPackageStockpile {
public:
    virtual ~IPackageStockpile() = default;
    virtual void push(Package&& package) = 0;
    virtual bool empty() const = 0;
    virtual Package pop() = 0;
    virtual const Package& peek() const = 0;
    virtual PackageQueueType get_queue_type() const = 0;
};

class IPackageQueue : public IPackageStockpile {
public:
    virtual ~IPackageQueue() = default;
};

class PackageQueue : public IPackageQueue {
public:
    PackageQueue(PackageQueueType type);
    void push(Package&& package) override;
    bool empty() const override;
    Package pop() override;
    const Package& peek() const override;
    PackageQueueType get_queue_type() const override;

private:
    std::deque<Package> packages_;
    PackageQueueType type_;
};

