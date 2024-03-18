#ifndef VRPTK_VRPTW_H
#define VRPTK_VRPTW_H

#include <iostream>
#include <map>
#include <vector>
#include <Eigen/Dense>

#include "node.h"
#include "fleet.h"
#include "request.h"
#include "solution.h"

namespace VRPTK {
    class VRPTW {
    protected:
        Eigen::MatrixXd nodes;
        std::vector<VehicleProfile> fleet;
        std::map<int, Request> requests;
    public:
        VRPTW() = default;
        VRPTW(Eigen::MatrixXd  nodes, const std::map<int, Request>& requests)
            : nodes(std::move(nodes)), requests(requests) {
        }

        void setNodes(const std::vector<Node>& nodes);
        Eigen::MatrixXd getNodes() const { return nodes; }
        int getNumNodes() const;

        void setFleet(const std::vector<VehicleProfile>& fleet) {this->fleet = fleet;}
        std::vector<VehicleProfile> getFleet() const { return fleet; }
        int getNumVehicles() const;

        void setRequests(const std::map<int, Request>& requests) { this->requests = requests; }
        std::map<int, Request> getRequests() const { return requests; }
        int getNumRequests() const { return requests.size(); }

        int getTotalDistance(const Solution& solution);
    };
}

#endif // VRPTK_VRPTW_H