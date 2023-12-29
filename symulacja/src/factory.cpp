
//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git

#include "factory.hpp"
#include "nodes.hpp"
#include <stdexcept>

bool Factory::is_consistent() {
    using colors_map = std::map<const PackageSender*, NodeColor>;
    auto color = colors_map();
    for (auto & node : ramp_collection) {
        color.emplace(&node, NodeColor::UNVISITED);
    }
    for (auto & node : worker_collection) {
        color.emplace(&node, NodeColor::UNVISITED);
    }

    try {
        for (auto & color_ : color) {
            if (!has_reachable_storehouse(color_.first, color))
                throw std::logic_error("Sender has no connection.");
        }
    }
    catch(const std::logic_error& log_error) {
        return false;
    }
    return true;
}

bool
Factory::has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) {
        return true;
    }
    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("No defined receivers.");
    }

    if (std::find_if(sender->receiver_preferences_.begin(), sender->receiver_preferences_.end(),
                     [&sender](auto & elem){ return dynamic_cast<PackageSender*>(elem.first) != sender; }) == sender->receiver_preferences_.end()) {

        for (auto& receiver : sender->receiver_preferences_.get_preferences()) {

            if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                if (std::find_if(sender->receiver_preferences_.begin(), sender->receiver_preferences_.end(),
                                 [&sender](auto& elem) {
                                     return dynamic_cast<PackageSender*>(elem.first) != sender;
                                 }) != sender->receiver_preferences_.end())
                    return true;

            }

            else if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
                IPackageReceiver* receiver_ptr = receiver.first;
                auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
                auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

                if (sendrecv_ptr == sender)
                    continue;
                if (std::find_if(sender->receiver_preferences_.begin(), sender->receiver_preferences_.end(),
                                 [&sender](auto& elem) {
                                     return dynamic_cast<PackageSender*>(elem.first) != sender;
                                 }) != sender->receiver_preferences_.end()) {
                    return true;
                }

                if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED)
                    has_reachable_storehouse(sendrecv_ptr, node_colors);
            }
        }
    }
    node_colors[sender] = NodeColor::VERIFIED;

    if (std::find_if(sender->receiver_preferences_.begin(), sender->receiver_preferences_.end(),
                     [&sender](auto & elem){ return dynamic_cast<PackageSender*>(elem.first) != sender; }) != sender->receiver_preferences_.end())
        return true;
    else
        throw std::logic_error("No defined receivers");
}

void Factory::do_deliveries(Time t) {
    for (auto & ramp : ramp_collection) {
        ramp.deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    for (auto & ramp : ramp_collection) {
        ramp.send_package();
    }
    for (auto & worker : worker_collection) {
        worker.send_package();
    }
}

void Factory::do_work(Time t) {
    for (auto & worker : worker_collection)
        worker.do_work(t);
}

ParsedLineData parse_line(std::string &line) {
    ParsedLineData parsedLineData;
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(line);
    char delimiter = ' ';
    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }
    std::string first_token = *tokens.begin();
    if (first_token == "LOADING_RAMP")
        parsedLineData.element_type = ParsedLineData::ElementType::RAMP;
    else if (first_token == "WORKER")
        parsedLineData.element_type = ParsedLineData::ElementType::WORKER;
    else if (first_token == "STOREHOUSE")
        parsedLineData.element_type = ParsedLineData::ElementType::STOREHOUSE;
    else if (first_token == "LINK")
        parsedLineData.element_type = ParsedLineData::ElementType::LINK;
    else
        throw std::logic_error("Inappropriate line");

    // We get rid of the first element
    tokens.erase(tokens.begin());
    for (auto & elem : tokens) {
        std::size_t find_index = elem.find('=');
        if (find_index != std::string::npos) {
            parsedLineData.parameters.emplace(std::pair<std::string, std::string>(elem.substr(0, find_index), elem.substr(find_index+1, elem.size())));
        }
    }
    return parsedLineData;
}


std::pair<std::string, int> decompose(std::string str){
    std::size_t find_index = str.find('-');
    if (find_index != std::string::npos) {
        return std::pair<std::string, int>(str.substr(0, find_index), std::stoi(str.substr(find_index+1, str.size())));
    }
    return std::pair<std::string, int>();
}


Factory load_factory_structure(std::istream & is) {
    Factory factory = Factory();
    std::string line;
    while (std::getline(is, line)) {
        if (line.empty() || *line.begin() == ';')
            continue;
        ParsedLineData parsedLineData = parse_line(line);
        switch (parsedLineData.element_type) {
            case ParsedLineData::ElementType::RAMP :
                factory.add_ramp(Ramp(std::stoi(parsedLineData.parameters["id"]), std::stoi(parsedLineData.parameters["delivery-interval"])));
                break;

            case ParsedLineData::ElementType::WORKER :
                factory.add_worker(Worker(std::stoi(parsedLineData.parameters["id"]),
                                          std::stoi(parsedLineData.parameters["processing-time"]),
                                          std::make_unique<PackageQueue>(parsedLineData.parameters["queue-type"] == "FIFO" ? PackageQueueType::FIFO : PackageQueueType::LIFO)));
                break;

            case ParsedLineData::ElementType::STOREHOUSE :
                factory.add_storehouse(Storehouse(std::stoi(parsedLineData.parameters["id"])));
                break;

            case ParsedLineData::ElementType::LINK :
                std::pair<std::string, int> src = decompose(parsedLineData.parameters["src"]);
                std::pair<std::string, int> dest = decompose(parsedLineData.parameters["dest"]);

                // Sender
                PackageSender * sender;
                if (src.first == "ramp") {
                    sender = &(*factory.find_ramp_by_id(src.second));
                }
                else if (src.first == "worker") {
                    sender = &(*factory.find_worker_by_id(src.second));
                }
                else {
                    throw std::logic_error("Invalid source");
                }

                // Receiver
                IPackageReceiver * receiver;
                if (dest.first == "worker") {
                    receiver = &(*factory.find_worker_by_id(dest.second));
                }
                else if (dest.first == "store") {
                    receiver = &(*factory.find_storehouse_by_id(dest.second));
                }
                else {
                    throw std::logic_error("Invalid destination");
                }

                sender->receiver_preferences_.add_receiver(receiver);

                break;
        }
    }
    return factory;
}

void save_factory_structure(Factory & factory, std::ostream & os) {
    // Ramps
    os << "; == LOADING RAMPS ==\n\n";
    for (auto iter = factory.ramp_cbegin(); iter != factory.ramp_cend(); ++iter) {
        os << "LOADING_RAMP id=" << iter->get_id() << " delivery-interval=" << iter->get_delivery_interval() << "\n";
    }

    // Workers
    os << "\n; == WORKERS ==\n\n";
    for (auto iter = factory.worker_cbegin(); iter != factory.worker_cend(); ++iter) {
        os << "WORKER id=" << iter->get_id() << " processing-time=" << iter->get_processing_duration() << " queue-type=" << (iter->get_queue()->get_queue_type() == PackageQueueType::FIFO ? "FIFO" : "LIFO") << "\n";
    }

    // Storehouses
    os << "\n; == STOREHOUSES ==\n\n";
    for (auto iter = factory.storehouse_cbegin(); iter != factory.storehouse_cend(); ++iter) {
        os << "STOREHOUSE id=" << iter->get_id() << '\n';
    }

    // LINKS
    os << "\n; == LINKS ==\n\n";
    // links for ramps
    for (auto iter = factory.ramp_cbegin(); iter != factory.ramp_cend(); ++iter) {
        for (auto & worker : iter->receiver_preferences_.get_preferences()) {
            os << "LINK src=ramp-" << iter->get_id() << " dest=worker-" << worker.first->get_id() << "\n";
        }
        os << '\n';
    }

    // links for workers
    for (auto iter = factory.worker_cbegin(); iter != factory.worker_cend(); ++iter) {
        for (auto & receiver : iter->receiver_preferences_.get_preferences()) {
            if (receiver.first->get_receiver_type() == ReceiverType::WORKER)
                os << "LINK src=worker-" << iter->get_id() << " dest=worker-" << receiver.first->get_id() << '\n';
            else
                os << "LINK src=worker-" << iter->get_id() << " dest=store-" << receiver.first->get_id() << '\n';
        }
        os << '\n';
    }
    os.flush();
}


//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
