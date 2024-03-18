#include "efvrptw.h"

using namespace VRPTK;

int EFVRPTW::getNumVehicles() const {
    int num_vehicles = 0;
    for (const auto & i : fleet) {
        num_vehicles += i.getNumber();
    }
    return num_vehicles;
}

int EFVRPTW::getTotalDistance(const Solution& solution) {
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