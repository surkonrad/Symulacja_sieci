Package PackageQueue::pop() {
    Package package_to_pop;

    switch (packageQueueType) {
        case PackageQueueType::LIFO:
            package_to_pop = std::move(package_list.front());
            package_list.pop_front();
            break;

        case PackageQueueType::FIFO:
            package_to_pop = std::move(package_list.back());
            package_list.pop_back();
            break;
    }
    return package_to_pop;
}

void PackageQueue::push(Package&& package) {
    package_list.emplace_back(std::move(package));
    // Done :)
}

PackageQueueType PackageQueue::get_queue_type() const {
    return packageQueueType;
}

size_type PackageQueue::size() const {
    return package_list.size();
}