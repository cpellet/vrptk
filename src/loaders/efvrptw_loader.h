#ifndef VRPTK_EFVRPTW_LOADER_H
#define VRPTK_EFVRPTW_LOADER_H

#include <map>
#include "tinyxml2.h"

#include "loader.h"
#include "vrptw_loader.h"
#include "../efvrptw.h"

namespace VRPTK {
    class EFVRPTWLoader : public Loader<VRPTW> {
    public:
        static bool isCompatible(const char* filename) {
            tinyxml2::XMLDocument doc;
            doc.LoadFile(filename);
            return doc.FirstChildElement("instance")->FirstChildElement("stations") != nullptr;
        }
        static Dataset<VRPTW>* load(const char* filename) {
            tinyxml2::XMLDocument doc;
            doc.LoadFile(filename);
            auto vrp = new EFVRPTW();
            auto nodes = VRPTWLoader::loadNodes(&doc);
            auto fleet = loadEFleet(&doc);
            auto requests = VRPTWLoader::loadRequests(&doc);
            auto stations = loadStations(&doc);
            auto prices = loadPrices(&doc);
            vrp->setNodes(nodes);
            vrp->setFleet(fleet);
            vrp->setRequests(requests);
            vrp->setStations(stations);
            vrp->setPrices(prices);
            std::string filename_str;
            for (int i = strlen(filename) - 1; i >= 0; i--) {
                if (filename[i] == '/') {
                    filename_str = filename + i + 1;
                    break;
                }
            }
            return new Dataset<VRPTW>("", filename_str, "", filename, vrp, Variant::EFVRPTW, false);
        }
        static std::vector<EVehicleProfile> loadEFleet(tinyxml2::XMLDocument* doc) {
            tinyxml2::XMLNode* fleet_node = doc->FirstChildElement("instance")->FirstChildElement("fleet")->FirstChildElement("vehicle_profile");
            std::vector<EVehicleProfile> fleet;
            while (fleet_node != nullptr) {
                const char* type = fleet_node->ToElement()->Attribute("type");
                const char* number = fleet_node->ToElement()->Attribute("number");
                const char* departure = fleet_node->FirstChildElement("departure_node")->GetText();
                const char* arrival = fleet_node->FirstChildElement("arrival_node")->GetText();
                const char* capacity = fleet_node->FirstChildElement("capacity")->GetText();
                const char* time = fleet_node->FirstChildElement("max_travel_time")->GetText();
                const char* e_capacity = fleet_node->FirstChildElement("energy_capacity")->GetText();
                tinyxml2::XMLNode* soc_node = fleet_node->FirstChildElement("soc");
                const char* min_soc = soc_node->FirstChildElement("min")->GetText();
                const char* max_soc = soc_node->FirstChildElement("max")->GetText();
                const char* current_soc = soc_node->FirstChildElement("initial")->GetText();
                const char* consumption = fleet_node->FirstChildElement("consumption")->GetText();
                const char* fixed_cost = fleet_node->FirstChildElement("fixed_cost")->GetText();
                const char* variable_cost = fleet_node->FirstChildElement("variable_cost")->GetText();
                fleet.emplace_back(atoi(type), atoi(number), atoi(departure), atoi(arrival), atof(capacity), atof(time), atof(e_capacity), atof(min_soc), atof(max_soc), atof(current_soc), atof(consumption), atof(fixed_cost), atof(variable_cost));
                fleet_node = fleet_node->NextSiblingElement("vehicle_profile");
            }
            return fleet;
        }
        static std::vector<ChargingStation> loadStations(tinyxml2::XMLDocument* doc) {
            tinyxml2::XMLNode* stations_node = doc->FirstChildElement("instance")->FirstChildElement("stations")->FirstChildElement("station");
            std::vector<ChargingStation> stations;
            while (stations_node != nullptr) {
                const char* id = stations_node->ToElement()->Attribute("id");
                const char* node = stations_node->ToElement()->Attribute("node");
                const char* capacity = stations_node->FirstChildElement("capacity")->GetText();
                const char* power = stations_node->FirstChildElement("power")->GetText();
                stations.emplace_back(atoi(id), atoi(node), atof(capacity), atof(power));
                stations_node = stations_node->NextSiblingElement("station");
            }
            return stations;
        }
        static std::vector<PricePoint> loadPrices(tinyxml2::XMLDocument* doc) {
            tinyxml2::XMLNode* prices_node = doc->FirstChildElement("instance")->FirstChildElement("prices")->FirstChildElement("point");
            std::vector<PricePoint> prices;
            while (prices_node != nullptr) {
                const char* start = prices_node->ToElement()->Attribute("start");
                const char* end = prices_node->ToElement()->Attribute("end");
                const char* price = prices_node->FirstChildElement("value")->GetText();
                prices.emplace_back(atof(start), atof(end), atof(price));
                prices_node = prices_node->NextSiblingElement("point");
            }
            return prices;
        }
    };
}

#endif // VRPTK_EFVRPTW_LOADER_H