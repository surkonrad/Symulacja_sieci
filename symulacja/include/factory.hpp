//
// Created by peter on 19.12.2021.
//

//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
#ifndef SIECI_FACTORY_HPP
#define SIECI_FACTORY_HPP

#include <vector>
#include <string>
#include <sstream>
#include "types.hpp"
#include "nodes.hpp"
#include "config.hpp"

template <class Node>
class NodeCollection {
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;
    void add(Node && node);
    void remove_by_id(ElementID id);
    iterator find_by_id(ElementID id) {
        return std::find_if(collection_.begin(), collection_.end(),
                            [&id](const auto & element){ return id == element.get_id();});
    }
    [[nodiscard]] const_iterator find_by_id(ElementID id) const {
        return std::find_if(collection_.cbegin(), collection_.cend(),
                            [&id](const auto & element){ return id == element.get_id();});
    }
    [[nodiscard]] const_iterator begin() const { return collection_.begin(); }
    [[nodiscard]] const_iterator cbegin() const { return collection_.cbegin(); }
    [[nodiscard]] const_iterator end() const { return collection_.end(); }
    [[nodiscard]] const_iterator cend() const { return collection_.cend(); }
    iterator begin() { return collection_.begin(); }
    iterator end() { return collection_.end(); }
private:
    container_t collection_;
};


class Factory {
public:
    enum class NodeColor { UNVISITED, VISITED, VERIFIED };
    // Dla obiektów klasy Ramp
    void add_ramp(Ramp && ramp) { ramp_collection.add(std::move(ramp)); }
    void remove_ramp(ElementID id) { ramp_collection.remove_by_id(id); }
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramp_collection.find_by_id(id); }
    [[nodiscard]] NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return ramp_collection.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramp_collection.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramp_collection.cend(); }
    // Dla obiektów klasy Worker
    void add_worker(Worker && worker) { worker_collection.add(std::move(worker)); }
    void remove_worker(ElementID id) { remove_receiver(worker_collection, id); }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return worker_collection.find_by_id(id); }
    [[nodiscard]] NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return worker_collection.find_by_id(id); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const { return worker_collection.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend() const { return worker_collection.cend(); }
    // Dla obiektów klasy Storehouse
    void add_storehouse(Storehouse && storehouse) { storehouse_collection.add(std::move(storehouse)); }
    void remove_storehouse(ElementID id) { remove_receiver(storehouse_collection, id); }
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouse_collection.find_by_id(id); }
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouse_collection.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouse_collection.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouse_collection.cend(); }
    // Metody realizujące "logikę biznesową"
    bool is_consistent();
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);
    bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);

private:
    template<class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
    NodeCollection<Ramp> ramp_collection;
    NodeCollection<Worker> worker_collection;
    NodeCollection<Storehouse> storehouse_collection;
};


struct ParsedLineData {
    enum class ElementType { RAMP, WORKER, STOREHOUSE, LINK };
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

ParsedLineData parse_line(std::string &line);
Factory load_factory_structure(std::istream & is);
std::pair<std::string, int> decompose(std::string str);
void save_factory_structure(Factory & factory, std::ostream & os);

template<class Node>
void NodeCollection<Node>::add(Node&& node) {
    if (find_by_id(node.get_id()) == collection_.end()) {
        collection_.push_back(std::move(node));
    }
}

template<class Node>
void NodeCollection<Node>::remove_by_id(ElementID id) {
    auto iter = find_by_id(id);
    if (iter != collection_.end()) {
        collection_.erase(iter);
    }
}


template<class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
        auto iter = collection.find_by_id(id);
    auto receiver_ptr = dynamic_cast<IPackageReceiver*>(&(*iter));

    for(auto ramp = ramp_collection.begin(); ramp != ramp_collection.end(); ++ramp){
        auto & _preferences = (*ramp).receiver_preferences_.get_preferences();
        for(auto receiver = _preferences.begin(); receiver != _preferences.end(); ++receiver){
            if((*receiver).first == receiver_ptr){
                (*ramp).receiver_preferences_.remove_receiver(receiver_ptr);
                break;
            }
        }
    }
    for(auto worker = worker_collection.begin(); worker != worker_collection.end(); ++worker){
        auto & _preferences = (*worker).receiver_preferences_.get_preferences();
        for(auto receiver = _preferences.begin(); receiver != _preferences.end(); ++receiver){
            if((*receiver).first == receiver_ptr){
                (*worker).receiver_preferences_.remove_receiver(receiver_ptr);
                break;
            }
        }
    }
    collection.remove_by_id(id);
}

#endif //SIECI_FACTORY_HPP

//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
