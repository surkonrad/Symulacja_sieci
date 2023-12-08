// nodes.cpp
#include "nodes.hpp"

Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> stockpile)
        : id_(id), stockpile_(std::move(stockpile)) {}

void Storehouse::receive_package(Package&& package) {
    stockpile_->push(std::move(package));
}

const Package& Storehouse::view_package() const {
    return stockpile_->peek();
}

bool Storehouse::has_package() const {
    return !stockpile_->empty();
}

ElementID Storehouse::get_id() const {
    return id_;
}

ReceiverPreferences::ReceiverPreferences(ProbabilityGenerator probability_generator)
        : probability_generator(probability_generator) {}

void ReceiverPreferences::add_receiver(IPackageReceiver* receiver, double probability) {
    preferences[receiver] = probability;
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* receiver) {
    preferences.erase(receiver);
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double random_value = probability_generator();
    double cumulative_probability = 0.0;

    for (const auto& preference : preferences) {
        cumulative_probability += preference.second;
        if (random_value <= cumulative_probability) {
            return preference.first;
        }
    }

    return nullptr;
}

void PackageSender::send_package(ReceiverPreferences& preferences, Package&& package) {
    IPackageReceiver* receiver = preferences.choose_receiver();
    if (receiver != nullptr) {
        receiver->receive_package(std::move(package));
    }
}

Ramp::Ramp(ElementID id, TimeOffset delivery_interval, ProbabilityGenerator probability_generator)
        : id_(id), delivery_interval_(delivery_interval), last_delivery_time_(0), probability_generator(probability_generator) {}

void Ramp::deliver_goods(Time t) {
    if (t - last_delivery_time_ >= delivery_interval_) {
        Package package(id_);
        send_package(*receiver_preferences, std::move(package));
        last_delivery_time_ = t;
    }
}

Worker::Worker(ElementID id, std::unique_ptr<IPackageQueue> queue, TimeOffset processing_time)
        : id_(id), queue_(std::move(queue)), processing_time_(processing_time), start_processing_time_(0) {}

void Worker::do_work(Time t) {
    if (!queue_->empty()) {
        if (start_processing_time_ == 0) {
            start_processing_time_ = t;
        }
        if (t - start_processing_time_ >= processing_time_) {
            Package package = queue_->pop();
            send_package(*receiver_preferences, std::move(package));
            start_processing_time_ = 0;
        }
    }
}
