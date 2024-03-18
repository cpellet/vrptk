#ifndef VRPTK_VRPTW_LOADER_H
#define VRPTK_VRPTW_LOADER_H

#include <map>
#include "tinyxml2.h"

#include "loader.h"
#include "../vrptw.h"

namespace VRPTK {
    class VRPTWLoader : public Loader<VRPTW> {
    public:
        Dataset<VRPTW>* load(const char* filename) {
            tinyxml2::XMLDocument doc;
            doc.LoadFile(filename);
            auto vrp = new VRPTW();
            auto nodes = loadNodes(&doc);
            auto fleet = loadFleet(&doc);
            auto requests = loadRequests(&doc);
            vrp->setNodes(nodes);
            vrp->setFleet(fleet);
            vrp->setRequests(requests);
            return new Dataset<VRPTW>("", filename, "", filename, vrp, Variant::VRPTW, false);
        }
        static std::vector<Node> loadNodes(tinyxml2::XMLDocument* doc) {
            tinyxml2::XMLNode* net_node = doc->FirstChildElement("instance")->FirstChildElement("network")->FirstChildElement("nodes")->FirstChildElement("node");
            std::vector<Node> nodes;
            while (net_node != nullptr) {
                const char* id = net_node->ToElement()->Attribute("id");
                const char* type = net_node->ToElement()->Attribute("type");
                const char* x = net_node->FirstChildElement("cx")->GetText();
                const char* y = net_node->FirstChildElement("cy")->GetText();
                nodes.emplace_back(atof(x), atof(y), atof(id), atof(type));
                net_node = net_node->NextSiblingElement("node");
            }
            return nodes;
        }
        static std::map<int, Request> loadRequests(tinyxml2::XMLDocument* doc) {
            tinyxml2::XMLNode* requests_node = doc->FirstChildElement("instance")->FirstChildElement("requests")->FirstChildElement("request");
            std::map<int, Request> requests;
            while (requests_node != nullptr) {
                const char* id = requests_node->ToElement()->Attribute("id");
                const char* node = requests_node->ToElement()->Attribute("node");
                const char* start = requests_node->FirstChildElement("tw")->FirstChildElement("start")->GetText();
                const char* end = requests_node->FirstChildElement("tw")->FirstChildElement("end")->GetText();
                const char* quantity = requests_node->FirstChildElement("quantity")->GetText();
                const char* service_time = requests_node->FirstChildElement("service_time")->GetText();
                requests.insert(std::pair<int, Request>(atoi(id), Request(atoi(id), atoi(node), atoi(start), atoi(end), atof(quantity), atof(service_time))));
                requests_node = requests_node->NextSiblingElement("request");
            }
            return requests;
        }
        static std::vector<VehicleProfile> loadFleet(tinyxml2::XMLDocument* doc) {
            tinyxml2::XMLNode* fleet_node = doc->FirstChildElement("instance")->FirstChildElement("fleet")->FirstChildElement("vehicle_profile");
            std::vector<VehicleProfile> fleet;
            while (fleet_node != nullptr) {
                const char* type = fleet_node->ToElement()->Attribute("type");
                const char* number = fleet_node->ToElement()->Attribute("number");
                const char* departure = fleet_node->FirstChildElement("departure_node")->GetText();
                const char* arrival = fleet_node->FirstChildElement("arrival_node")->GetText();
                const char* capacity = fleet_node->FirstChildElement("capacity")->GetText();
                const char* time = fleet_node->FirstChildElement("max_travel_time")->GetText();
                fleet.emplace_back(atoi(type), atoi(number), atoi(departure), atoi(arrival), atof(capacity), atof(time));
                fleet_node = fleet_node->NextSiblingElement("vehicle_profile");
            }
            return fleet;
        }
    };
}

#endif // VRPTK_VRPTW_LOADER_H