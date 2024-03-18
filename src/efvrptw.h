#ifndef VRPTK_EFVRPTW_H
#define VRPTK_EFVRPTW_H

#include "fleet.h"
#include "vrptw.h"
#include "request.h"

namespace VRPTK {
    class ChargingStation {
    private:
        int id, node;
        double capacity, power;
    public:
        ChargingStation(int id, int node, double capacity, double power)
            : id(id), node(node), capacity(capacity), power(power) {
        }

        int getId() const { return id; }
        int getNode() const { return node; }
        double getCapacity() const { return capacity; }
        double getPower() const { return power; }
    };

    class PricePoint {
    private:
        double start, end, price;
    public:
        PricePoint(double start, double end, double price)
            : start(start), end(end), price(price) {
        }

        double getStart() const { return start; }
        double getEnd() const { return end; }
        double getPrice() const { return price; }
    };

    class EVehicleProfile : public VehicleProfile {
    private:
        double e_capacity, min_soc, max_soc, current_soc, consumption, fixed_cost, variable_cost;
    public:
        EVehicleProfile(int type, int number, int departure, int arrival, double capacity, double max_time, double e_capacity, double min_soc, double max_soc, double current_soc, double consumption, double fixed_cost, double variable_cost)
            : VehicleProfile(type, number, departure, arrival, capacity, max_time), e_capacity(e_capacity), min_soc(min_soc), max_soc(max_soc), current_soc(current_soc), consumption(consumption), fixed_cost(fixed_cost), variable_cost(variable_cost) {
        }

        double getECapacity() const { return e_capacity; }
        double getMinSOC() const { return min_soc; }
        double getMaxSOC() const { return max_soc; }
        double getCurrent_SOC() const { return current_soc; }
        double getConsumption() const { return consumption; }
        double getFixedCost() const { return fixed_cost; }
        double getVariableCost() const { return variable_cost; }
    };

    class EFVRPTW : public VRPTW {
    private:
        std::vector<EVehicleProfile> fleet;
        std::vector<ChargingStation> stations;
        std::vector<PricePoint> prices;
    public:
        EFVRPTW() = default;
        EFVRPTW(const Eigen::MatrixXd& nodes, const std::vector<EVehicleProfile>& fleet, const std::map<int, Request>& requests, const std::vector<ChargingStation>& stations, const std::vector<PricePoint>& prices)
            : VRPTW(nodes, requests), fleet(fleet), stations(stations), prices(prices) {
        }

        void setFleet(const std::vector<EVehicleProfile>& fleet) { this->fleet = fleet; }

        std::vector<EVehicleProfile> getFleet() const { return fleet; }

        int getNumVehicles() const;
        void setStations(const std::vector<ChargingStation>& stations) { this->stations = stations; }
        std::vector<ChargingStation> getStations() const { return stations; }
        int getNumStations() const { return stations.size(); }
        void setPrices(const std::vector<PricePoint>& prices) { this->prices = prices; }
        std::vector<PricePoint> getPrices() const { return prices; }
        int getNumPrices() const { return prices.size(); }
        int getTotalDistance(const Solution& solution);
    };
}

#endif // VRPTK_EFVRPTW_H