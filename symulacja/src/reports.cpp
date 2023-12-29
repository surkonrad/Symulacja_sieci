//
// Created by peter on 19.12.2021.

//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
#include <reports.hpp>

void generate_structure_report(const Factory& f, std::ostream& os) {
    // RAMPS
    os << "\n== LOADING RAMPS ==\n\n";
    for (auto iter = f.ramp_cbegin(); iter != f.ramp_cend(); ++iter) {
        os << "LOADING RAMP #" << iter->get_id() << '\n' << "  Delivery interval: " << iter->get_delivery_interval() << "\n" << "  Receivers:\n";
        for (auto & receiver : iter->receiver_preferences_.get_preferences()) {
            os << "    worker #" << receiver.first->get_id() << '\n';
        }
        os << "\n";
    }

    os << "\n== WORKERS ==\n\n";
    for (auto iter = f.worker_cbegin(); iter != f.worker_cend(); ++iter) {
        os << "WORKER #" << iter->get_id() << "\n  Processing time: " << iter->get_processing_duration()
        << "\n  Queue type: " << ((iter->get_queue()->get_queue_type() == PackageQueueType::FIFO) ? "FIFO" : "LIFO") << '\n';
        os << "  Receivers:\n";
        for (auto & receiver : iter->receiver_preferences_.get_preferences()) {
            if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
                os << "    worker #" << receiver.first->get_id() << '\n';
            }
            else {
                os << "    storehouse #" << receiver.first->get_id() << '\n';
            }
        }
        os << '\n';
    }

    os << "\n== STOREHOUSES ==\n\n";
    for (auto iter = f.storehouse_cbegin(); iter != f.storehouse_cend(); ++iter) {
        os << "STOREHOUSE #" << iter->get_id() << "\n\n";
    }

    os.flush();
}

void generate_simulation_turn_report(const Factory & f, std::ostream & os, Time t) {
    os << "=== [ Turn: " << t << " ] ===\n\n";
    if (f.worker_cbegin() == f.worker_cend())
        os << "== WORKERS ==\n";
    else {
        os << "== WORKERS ==\n\n";
        for (auto iter = f.worker_cbegin(); iter != f.worker_cend(); ++iter) {
            os << "WORKER #" << iter->get_id();
            if (iter->get_processing_buffer().has_value() > 0)
                os << "\n  PBuffer: #" <<
                   iter->get_processing_buffer().value().get_id() << " (pt = " << iter->get_processing_duration()-iter->get_package_processing_start_time() << ")\n";
            else {
                os << "\n  PBuffer: (empty)\n";
            }
            os << "  Queue: ";
            if (iter->get_queue()->empty())
                os << "(empty)\n";
            else {
                for (auto element = dynamic_cast<PackageQueue*>(iter->get_queue())->cbegin(); element != dynamic_cast<PackageQueue*>(iter->get_queue())->cend(); ++element) {
                    if (element == std::next(dynamic_cast<PackageQueue*>(iter->get_queue())->cend(),-1))
                        os << "#" << element->get_id() << '\n';
                    else {
                        os << "#" << element->get_id() << ", ";
                    }
                }
            }
            os << "  SBuffer: ";
            if (iter->get_sending_buffer().has_value()) {
                os << "#" << iter->get_sending_buffer().value().get_id() << "\n";
            }
            else {
                os << "(empty)\n";
            }
        }
    }

    os << "\n\n== STOREHOUSES ==\n\n";
    for (auto iter = f.storehouse_cbegin(); iter != f.storehouse_cend(); ++iter) {
        os << "STOREHOUSE #" << iter->get_id();
        if (iter->get_queue()->empty()) {
            os << "\n  Stock: (empty)\n";
        }
        else {
            os << "\n  Stock: ";
            for (auto stock_element = dynamic_cast<PackageQueue*>(iter->get_queue())->begin(); stock_element != dynamic_cast<PackageQueue*>(iter->get_queue())->end(); ++stock_element) {
                if (stock_element == std::next(dynamic_cast<PackageQueue*>(iter->get_queue())->end(), -1))
                    os << "#" << stock_element->get_id() << '\n';
                else {
                    os << "# " << stock_element->get_id() << ", ";
                }
            }
        }
        os << '\n';
    }
    os.flush();
}

bool IntervalReportNotifier::should_generate_report(Time t) {
    if (delta_t_ == 1) {
        if (t % delta_t_ == 0)
            return true;
    }
    else if (t % delta_t_ == 1) {
        return true;
    }
    return false;
}

bool SpecificTurnsReportNotifier::should_generate_report(Time t) {
    if (turns_.find(t) != turns_.end()) {
        return true;
    }
    return false;
}

//URL repozytorium: https://github.com/PiotrWolnik/NetSim.git
