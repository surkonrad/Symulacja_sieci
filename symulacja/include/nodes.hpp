#ifndef SIECI_NODES_HPP
#define SIECI_NODES_HPP

#include <map>
#include <ostream>
#include <optional>
#include <memory>
#include "types.hpp"
#include "storage_types.hpp"
#include "package.hpp"
#include "config.hpp"
#include "helpers.hpp"


enum class ReceiverType {
    WORKER, STOREHOUSE
};


class IPackageReceiver {
public:
    virtual void receive_package(Package && p) = 0;
    [[nodiscard]] virtual ElementID get_id() const = 0;
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
        [[nodiscard]] virtual ReceiverType get_receiver_type() const = 0;
    #endif
    virtual ~IPackageReceiver() = default;
    [[nodiscard]] virtual IPackageStockpile::const_iterator cbegin() const = 0;
    [[nodiscard]] virtual IPackageStockpile::const_iterator cend() const = 0;
    [[nodiscard]] virtual IPackageStockpile::const_iterator begin() const = 0;
    [[nodiscard]] virtual IPackageStockpile::const_iterator end() const = 0;
};


class Storehouse : public IPackageReceiver {
public:
    explicit Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d =
            std::unique_ptr<IPackageStockpile>(new PackageQueue(PackageQueueType::FIFO)))
            : storehouse_(std::move(d)), ID_(id) {}
    // Klasa Worker realizuje interfejs IPackageReceiver dlatego należy przeciążyć metody tej klasy
    void receive_package(Package && p) override;
    [[nodiscard]] ElementID get_id() const override { return ID_; }
    // Mozliwe do usuniecia
    [[nodiscard]] IPackageQueue * get_queue() const { return dynamic_cast<IPackageQueue*>(storehouse_.get()); }
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
        [[nodiscard]] ReceiverType get_receiver_type() const override { return ReceiverType::STOREHOUSE; }
    #endif
    [[nodiscard]] IPackageStockpile::const_iterator cbegin() const override { return storehouse_->cbegin(); }
    [[nodiscard]] IPackageStockpile::const_iterator cend() const override { return storehouse_->cend(); }
    [[nodiscard]] IPackageStockpile::const_iterator begin() const override { return storehouse_->begin(); }
    [[nodiscard]] IPackageStockpile::const_iterator end() const override { return storehouse_->end(); }
private:
    std::unique_ptr<IPackageStockpile> storehouse_;
    ElementID ID_;
};


class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator);
    void add_receiver(IPackageReceiver* ptr);
    void remove_receiver(IPackageReceiver * ptr);
    IPackageReceiver * choose_receiver();
    [[nodiscard]] const preferences_t & get_preferences() const { return preferences; }
    [[nodiscard]] const_iterator begin() const;
    [[nodiscard]] const_iterator cbegin() const;
    [[nodiscard]] const_iterator end() const;
    [[nodiscard]] const_iterator cend() const;
    preferences_t preferences;
private:
    ProbabilityGenerator pg_;
};


class PackageSender {
public:
    PackageSender() = default;
    PackageSender(PackageSender && package_sender) = default;
    PackageSender& operator=(PackageSender && packageSender) = default;
    virtual ~PackageSender() = default;
    void send_package();
    [[nodiscard]] std::optional<Package> & get_sending_buffer() const { return (std::optional<Package> &) std::move(buffer_); }
    ReceiverPreferences receiver_preferences_;
protected:
    virtual void push_package(Package&& package);
private:
    std::optional<Package> buffer_;
};


class Ramp : public PackageSender {
public:
    explicit Ramp(ElementID id, TimeOffset di) : ID_(id), di_(di) {}
    Ramp & operator=(Ramp && ramp) = default;
    Ramp(Ramp && ramp) = default;
    void deliver_goods(Time t);
    [[nodiscard]] TimeOffset get_delivery_interval() const { return di_; }
    [[nodiscard]] ElementID get_id() const { return ID_; }
private:
    ElementID ID_;
    TimeOffset di_;
    Time t_;
};


class Worker : public PackageSender, public IPackageReceiver {
public:
    explicit Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> ptr) : ID_(id), pd_(pd), t_(0), worker_ptr(std::move(ptr)) {}
    void do_work(Time t);
    [[nodiscard]] TimeOffset get_processing_duration() const { return pd_; }
    Time get_package_processing_start_time() const { return t_; }
    [[nodiscard]] IPackageQueue * get_queue() const { return worker_ptr.get(); }
    // Klasa Worker realizuje interfejs IPackageReceiver dlatego należy przeciążyć metody tej klasy
    void receive_package(Package && p) override;
    [[nodiscard]] ElementID get_id() const override { return ID_; }
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
        [[nodiscard]] ReceiverType get_receiver_type() const override { return ReceiverType ::WORKER; }
    #endif
    [[nodiscard]] std::optional<Package> & get_processing_buffer() const { return (std::optional<Package> &) std::move(worker_buffer_); }
    [[nodiscard]] IPackageStockpile::const_iterator cbegin() const override { return worker_ptr->cbegin(); }
    [[nodiscard]] IPackageStockpile::const_iterator cend() const override { return worker_ptr->cend(); }
    [[nodiscard]] IPackageStockpile::const_iterator begin() const override { return worker_ptr->begin(); }
    [[nodiscard]] IPackageStockpile::const_iterator end() const override { return worker_ptr->end(); }
private:
    ElementID ID_;
    TimeOffset pd_;
    Time t_;
    std::optional<Package> worker_buffer_;
    std::unique_ptr<IPackageQueue> worker_ptr;
};

#endif //SIECI_NODES_HPP