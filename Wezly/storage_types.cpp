
#include "storage_types.hpp"
#include "types.hpp"



Package PackageQueue::pop() {
    Package package_to_pop;

    switch(packageQueueType){
        case PackageQueueType::LIFO:
            package_to_pop = std::move(*package_list.begin());
            package_list.pop_front();
            break;

        case PackageQueueType::FIFO:
            package_to_pop =  std::move(*package_list.end());
            package_list.pop_back();
            break;
    }
    return package_to_pop;
}


void PackageQueue::push(Package&& package) {

    package_list.push_back(std::move(package));

}
PackageQueueType PackageQueue::get_queue_type() const {
    return packageQueueType;
}


size_type PackageQueue::size() const {

    std::size_t size_of_container = 0;

    for (const_iterator iter = package_list.begin(); iter != package_list.end(); iter++) {
        size_of_container++;
    }
    return size_of_container;
}
