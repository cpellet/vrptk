#ifndef VRPTK_VRP_H
#define VRPTK_VRP_H

#include <map>
#include <vector>
#include <Eigen/Dense>

#include "node.h"
#include "fleet.h"
#include "request.h"
#include "solution.h"

namespace VRPTK {

class VRP {
private:
    Eigen::MatrixXd nodes;
    std::vector<VehicleProfile> fleet;
    std::map<int, Request> requests;
public:
    VRP() {}
    VRP(const Eigen::MatrixXd& nodes, const std::map<int, Request>& requests)
        : nodes(nodes), requests(requests) {}
    
    void setNodes(const std::vector<Node>& nodes);
    Eigen::MatrixXd getNodes() const { return nodes; }
    int getNumNodes();

    void setFleet(const std::vector<VehicleProfile>& fleet) { this->fleet = fleet; }
    std::vector<VehicleProfile> getFleet() const { return fleet; }
    int getNumVehicles();

    void setRequests(const std::map<int, Request>& requests) { this->requests = requests; }
    std::map<int, Request> getRequests() const { return requests; }
    int getNumRequests() { return requests.size(); }

    int getTotalDistance(const Solution& solution);
};

}

#endif // VRPTK_VRP_H