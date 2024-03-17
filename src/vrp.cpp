#include "vrp.h"

using namespace VRPTK;

void VRP::setNodes(const std::vector<Node>& nodes) {
    this->nodes = Eigen::MatrixXd(nodes.size(), 4);
    for (int i = 0; i < nodes.size(); i++) {
        this->nodes(i, 0) = nodes[i].getId();
        this->nodes(i, 1) = nodes[i].getX();
        this->nodes(i, 2) = nodes[i].getY();
        this->nodes(i, 3) = nodes[i].getType();
    }
}

int VRP::getNumNodes() {
    return nodes.size();
}

int VRP::getNumVehicles() {
    int num_vehicles = 0;
    for (int i = 0; i < fleet.size(); i++) {
        num_vehicles += fleet[i].getNumber();
    }
    return num_vehicles;
}

int VRP::getTotalDistance(const Solution& solution) {
    int total_distance = 0;
    for (int i = 0; i < solution.getRoutes().size(); i++) {
        std::vector<int> route = solution.getRoutes()[i];
        for (int j = 0; j < route.size() - 1; j++) {
            int from = route[j];
            int to = route[j + 1];
            total_distance += (int)(nodes(from, 1) - nodes(to, 1)) * (nodes(from, 1) - nodes(to, 1)) + (int)(nodes(from, 2) - nodes(to, 2)) * (nodes(from, 2) - nodes(to, 2));
        }
    }
    return total_distance;
}