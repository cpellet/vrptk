#ifndef VRPTK_SOLVERS_VRPTW_HEXALY_H
#define VRPTK_SOLVERS_VRPTW_HEXALY_H

#include "solver.h"
#include "localsolver.h"

using namespace localsolver;

namespace VRPTK {
    class VRPTWHEXALYSolver : public Solver {
    private:
        LocalSolver localsolver;
        int nbCustomers;
        int vehicleCapacity;
        int maxHorizon;
        std::vector<int> demandsData;
        std::vector<int> earliestStartData;
        std::vector<int> latestEndData;
        std::vector<int> serviceTimeData;
        std::vector<std::vector<double>> distanceData;
        std::vector<double> distanceDepotData;
        int nbVehicles;
        std::vector<LSExpression> customersSequences;
        std::vector<LSExpression> vehiclesUsed;
        LSExpression totalLateness;
        LSExpression nbVecUsed;
        LSExpression totalDistance;
    public:
        VRPTWHEXALYSolver(VRPTW* vrptw) : Solver(vrptw) {
            this->nbVehicles = vrptw->getNumVehicles();
            this->nbCustomers = vrptw->getNumRequests();
        }
        Solution solve(int limit) override;
    };

}

#endif // VRPTK_SOLVERS_VRPTW_HEXALY_H