// storage_types.cpp
#include "storage_types.hpp"

PackageQueue::PackageQueue(PackageQueueType type) : type_(type) {}

void PackageQueue::push(Package&& package) {
    packages_.emplace_back(std::move(package));
}

bool PackageQueue::empty() const {
    return packages_.empty();
}

Package PackageQueue::pop() {
    if (type_ == PackageQueueType::FIFO) {
        Package front = std::move(packages_.front());
        packages_.pop_front();
        return front;
    } else {
        Package back = std::move(packages_.back());
        packages_.pop_back();
        return back;
    }
}

const Package& PackageQueue::peek() const {
    if (type_ == PackageQueueType::FIFO) {
        return packages_.front();
    } else {
        return packages_.back();
    }
}

PackageQueueType PackageQueue::get_queue_type() const {
    return type_;
}
