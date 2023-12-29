//
// Created by peter on 19.12.2021.

//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git

#include "nodes.hpp"

#include <utility>

void Storehouse::receive_package(Package&& p) {
    storehouse_->push(std::move(p));
}

ReceiverPreferences::const_iterator ReceiverPreferences::begin() const {
    return preferences.begin();
}

ReceiverPreferences::const_iterator ReceiverPreferences::cbegin() const {
    return preferences.cbegin();
}

ReceiverPreferences::const_iterator ReceiverPreferences::end() const {
    return preferences.end();
}

ReceiverPreferences::const_iterator ReceiverPreferences::cend() const {
    return preferences.cend();
}

void Worker::receive_package(Package&& p) {
    worker_ptr->push(std::move(p));
}


ReceiverPreferences::ReceiverPreferences(ProbabilityGenerator pg) : pg_(pg) {
    preferences = preferences_t();
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* ptr) {
    auto it = std::find_if(preferences.begin(), preferences.end(),
                           [&ptr](auto & elem){ return ptr == elem.first; });
    if (it != preferences.end()) {
        preferences.extract(it->first);
        for (auto & elem : preferences) {
            elem.second = double(1)/double(preferences.size());
        }
    }
}

void ReceiverPreferences::add_receiver(IPackageReceiver* ptr) {
    auto it = std::find_if(preferences.begin(), preferences.end(),
                           [&ptr](auto &elem) { return elem.first == ptr; });
    if (it == preferences.end()) {
        if (preferences.size() > 0) {
            preferences.emplace(ptr, 1/preferences.size());
            for (auto & elem : preferences) {
                elem.second = double(1)/double(preferences.size());
            }
        }
        else {
            preferences.emplace(ptr, 1);
        }

    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    std::vector<double> probability_vals;
    for (auto & elem : preferences) {
        if (probability_vals.size() > 0)
            probability_vals.push_back(elem.second + probability_vals[probability_vals.size()-1]);
        probability_vals.push_back(0.0);
    }
    double probability = pg_();

    std::size_t i = 0;
    IPackageReceiver* ptr = preferences.begin()->first;
    for (auto iter = preferences.begin(); iter != preferences.end(); ++iter, ++i) {
        if (probability < probability_vals[i])
            continue;
        else {
            ptr = iter->first;
            break;
        }
    }
    remove_receiver(ptr);
    return ptr;
}

void PackageSender::push_package(Package&& package) {
    if (buffer_.has_value() == 0)
        buffer_.emplace(std::move(package));
}

void PackageSender::send_package() {
    if (!receiver_preferences_.get_preferences().empty()) {
        if (buffer_) {
            auto receiver = receiver_preferences_.choose_receiver();
            receiver->receive_package(std::move(buffer_.value()));
            buffer_.reset();
        }
    }
}

void Ramp::deliver_goods(Time t) {
    t_ = t;
    if(t % di_ == 1){
        push_package(Package());
    }
}

void Worker::do_work(Time t) {
    if (!worker_buffer_.has_value()) {
        if (!worker_ptr->empty()){
            worker_buffer_.emplace(worker_ptr->pop());
            t_ = t;
        }
    }
    if (worker_buffer_.has_value()){
        if (t_ + pd_ -1 == t){
            push_package( std::move(worker_buffer_.value()));
            worker_buffer_.reset();
            if(!worker_ptr->empty()){
                worker_buffer_.emplace(worker_ptr->pop());
                t_ = t;
            }
        }
    }
}


//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
