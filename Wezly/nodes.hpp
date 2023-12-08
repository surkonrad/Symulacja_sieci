// nodes.hpp
#pragma once

#include "package.hpp"
#include "storage_types.hpp"

class IPackageReceiver {
public:
    virtual ~IPackageReceiver() = default;
    virtual void receive_package(Package&& package) = 0;
    virtual const Package& view_package() const = 0;
    virtual bool has_package() const = 0;
    virtual ElementID get_id() const = 0;
};

class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> stockpile);
    void receive_package(Package&& package) override;
    const Package& view_package() const override;
    bool has_package() const override;
    ElementID get_id() const override;

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> stockpile_;
};

class ReceiverPreferences {
public:
    ReceiverPreferences(ProbabilityGenerator probability_generator = probability_generator_default);
    void add_receiver(IPackageReceiver* receiver, double probability);
    void remove_receiver(IPackageReceiver* receiver);
    IPackageReceiver* choose_receiver();

private:
    std::map<IPackageReceiver*, double> preferences;
    ProbabilityGenerator probability_generator_default = []() { return 0.0; };
    ProbabilityGenerator probability_generator;
};

class PackageSender {
public:
    void send_package(ReceiverPreferences& preferences, Package&& package);

protected:
    ReceiverPreferences* receiver_preferences;
};

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset delivery_interval, ProbabilityGenerator probability_generator = probability_generator_default);
    void deliver_goods(Time t);

private:
    ElementID id_;
    TimeOffset delivery_interval_;
    Time last_delivery_time_;
    ProbabilityGenerator probability_generator_default = []() { return 0.0; };
    ProbabilityGenerator probability_generator;
};

class Worker : public PackageSender {
public:
    Worker(ElementID id, std::unique_ptr<IPackageQueue> queue, TimeOffset processing_time);
    void do_work(Time t);

private:
    ElementID id_;
    std::unique_ptr<IPackageQueue> queue_;
    TimeOffset processing_time_;
    Time start_processing_time_;
};

