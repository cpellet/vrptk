#include "vrptw.h"

using namespace VRPTK;

void VRPTW::setNodes(const std::vector<Node>& nodes) {
    this->nodes = Eigen::MatrixXd(nodes.size(), 4);
    for (int i = 0; i < nodes.size(); i++) {
        this->nodes(i, 0) = nodes[i].getId();
        this->nodes(i, 1) = nodes[i].getX();
        this->nodes(i, 2) = nodes[i].getY();
        this->nodes(i, 3) = nodes[i].getType();
    }
}

int VRPTW::getNumNodes() const {
    return nodes.size();
}

int VRPTW::getNumVehicles() const {
    int num_vehicles = 0;
    for (const auto& i : fleet) {
        num_vehicles += i.getNumber();
    }
    return num_vehicles;
}

int VRPTW::getTotalDistance(const Solution& solution) {
    int total_distance = 0;
    for (int i = 0; i < solution.getRoutes().size(); i++) {
        std::vector<int> route = solution.getRoutes()[i];
        for (int j = 0; j < route.size() - 1; j++) {
            int from = route[j];
            int to = route[j + 1];
            total_distance += static_cast<int>(nodes(from, 1) - nodes(to, 1)) * (nodes(from, 1) - nodes(to, 1)) + static_cast<int>(nodes(from, 2) - nodes(to, 2)) * (nodes(from, 2) - nodes(to, 2));
        }
    }
    return total_distance;
}

std::vector<std::string> VRPTW::validateSolution(const Solution& solution) {
    std::vector<std::string> errors;
    std::vector<int> remaining_nodes;
    for (int i = 0; i < nodes.rows(); i++) {
        if (nodes(i, 0) != 0) {
            remaining_nodes.push_back(nodes(i, 0));
        }
    }
    for (int i = 0; i < solution.getRoutes().size(); i++) {
        std::vector<int> route = solution.getRoutes()[i];
        int timestamp = 0;
        for (int j = 0; j < route.size(); j++) {
            int node = route[j];
            if (node < 1 || node > nodes.rows()) {
                errors.push_back("Node " + std::to_string(node) + " does not exist");
            }
            if (std::find(remaining_nodes.begin(), remaining_nodes.end(), node) == remaining_nodes.end()) {
                errors.push_back("Node " + std::to_string(node) + " has already been visited");
            }
            remaining_nodes.erase(std::remove(remaining_nodes.begin(), remaining_nodes.end(), node), remaining_nodes.end());
        }
    }
    if (!remaining_nodes.empty()) {
        errors.push_back("Not all nodes have been visited: remaining nodes are " + std::to_string(remaining_nodes[0]));
    }
    return errors;
}

int VRPTW::getDistance(int i, int j) const {
    return sqrt(pow(nodes(i, 1) - nodes(j, 1), 2) + pow(nodes(i, 2) - nodes(j, 2), 2));
}