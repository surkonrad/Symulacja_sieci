// storage_types.cpp
#include "storage_types.hpp"

PackageQueue::PackageQueue(PackageQueueType type) : queueType(type) {}

void PackageQueue::push(const Package& package) {
    packages.push_back(package);
}

IPackageStockpile::const_iterator PackageQueue::begin() const {
    return packages.begin();
}

IPackageStockpile::const_iterator PackageQueue::end() const {
    return packages.end();
}

size_t PackageQueue::size() const {
    return packages.size();
}

bool PackageQueue::empty() const {
    return packages.empty();
}

PackageQueueType PackageQueue::getQueueType() const {
    return queueType;
}

Package PackageQueue::pop() {
    if (queueType == PackageQueueType::FIFO) {
        auto frontPackage = packages.front();
        packages.pop_front();
        return frontPackage;
    } else if (queueType == PackageQueueType::LIFO) {
        auto backPackage = packages.back();
        packages.pop_back();
        return backPackage;
    } else {
        // Handle unsupported queue type
        std::cerr << "Unsupported queue type!" << std::endl;
        // Zwracamy pustą paczkę - tu możesz dostosować do swoich potrzeb
        return Package();
    }
}
