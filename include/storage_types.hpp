// storage_types.hpp
#pragma once

#include <list>
#include "types.hpp"
#include "package.hpp"

class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual ~IPackageStockpile() = default;

    virtual void push(const Package& package) = 0;

    virtual const_iterator begin() const = 0;

    virtual const_iterator end() const = 0;

    virtual size_t size() const = 0;

    virtual bool empty() const = 0;
};

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageQueue : public IPackageStockpile {
public:
    virtual Package pop() = 0;

    virtual PackageQueueType getQueueType() const = 0;
};

class PackageQueue : public IPackageQueue {
private:
    std::list<Package> packages;
    PackageQueueType queueType;

public:
    PackageQueue(PackageQueueType type);

    void push(const Package& package) override;

    const_iterator begin() const override;

    const_iterator end() const override;

    size_t size() const override;

    bool empty() const override;

    PackageQueueType getQueueType() const override;

    Package pop() override;
};
