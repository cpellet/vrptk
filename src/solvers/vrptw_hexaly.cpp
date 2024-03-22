#include "vrptw_hexaly.h"

using namespace VRPTK;

Solution VRPTWHEXALYSolver::solve(int limit) {
    std::vector<std::vector<int>> routes;

    // Declare optimization model
    LSModel model = localsolver.getModel();

    // Sequence of customers visited by each vehicle
    customersSequences.resize(nbVehicles);
    for (int i = 0; i < nbVehicles; i++) {
        customersSequences[i] = model.listVar(nbCustomers);
    }

    // All customers must be visited by exactly one vehicle
    model.constraint(model.partition(customersSequences.begin(), customersSequences.end()));

    auto demands_array = vrptw->getNodes().col(2);
    LSExpression demands = model.array(demands_array.begin(), demands_array.end());

    std::vector<int> start_times;
    std::vector<int> end_times;
    std::vector<int> service_times;
    auto requests = vrptw->getRequests();
    for (auto& [id, request] : requests) {
        start_times.push_back(request.getStart());
        end_times.push_back(request.getEnd());
        service_times.push_back(request.getServiceTime());
    }
    LSExpression earliest = model.array(start_times.begin(), start_times.end());
    LSExpression latest = model.array(end_times.begin(), end_times.end());
    LSExpression serviceTime = model.array(service_times.begin(), service_times.end());

    LSExpression distMatrix = model.array();
    for (int i = 0; i < vrptw->getNumNodes(); i++) {
        std::vector<int> row;
        for (int j = 0; j < vrptw->getNumNodes(); j++) {
            row.push_back(vrptw->getDistance(i, j));
        }
        distMatrix.addOperand(model.array(row.begin(), row.end()));
    }

    LSExpression distDepot = model.array();
    for (int i = 0; i < vrptw->getNumNodes(); i++) {
        distDepot.addOperand(vrptw->getDistance(0, i));
    }

    vehiclesUsed.resize(nbVehicles);
    std::vector<LSExpression> distRoutes(nbVehicles), endTime(nbVehicles), homeLateness(nbVehicles), lateness(nbVehicles);

    for (int k = 0; k < nbVehicles; k++) {
        LSExpression sequence = customersSequences[k];
        LSExpression c = model.count(sequence);

        vehiclesUsed[k] = c > 0;

        LSExpression demandLambda = model.createLambdaFunction([&](LSExpression j) {return demands[j]; });
        LSExpression routeQuantity = model.sum(sequence, demandLambda);
        model.constraint(routeQuantity <= vehicleCapacity);

        LSExpression distLambda = model.createLambdaFunction([&](LSExpression i) {return model.at(distMatrix, sequence[i - 1], sequence[i]); });
        distRoutes[k] = model.sum(model.range(1, c), distLambda) + model.iif(c > 0, distDepot[sequence[0]] + distDepot[sequence[c - 1]], 0);

        LSExpression endTimeLambda = model.createLambdaFunction([&](LSExpression i, LSExpression prev) {
            return model.max(earliest[sequence[i]],
            model.iif(i == 0, distDepot[sequence[0]],
            prev + model.at(distMatrix, sequence[i - 1], sequence[i]))) +
            serviceTime[sequence[i]];
            });

        endTime[k] = model.array(model.range(0, c), endTimeLambda, 0);

        int maxHorizon = vrptw->getFleet()[k].getMaxTime();

        // Arriving home after max horizon
        homeLateness[k] =
            model.iif(vehiclesUsed[k], model.max(0, endTime[k][c - 1] + distDepot[sequence[c - 1]] - maxHorizon), 0);

        // Completing visit after latest end
        LSExpression lateLambda = model.createLambdaFunction(
            [&](LSExpression i) { return model.max(0, endTime[k][i] - latest[sequence[i]]); });
        lateness[k] = homeLateness[k] + model.sum(model.range(0, c), lateLambda);
    }

    totalLateness = model.sum(lateness.begin(), lateness.end());

    // Total number of trucks used
    nbVecUsed = model.sum(vehiclesUsed.begin(), vehiclesUsed.end());

    // Total distance traveled (convention in Solomon's instances is to round to 2 decimals)
    totalDistance = model.round(100 * model.sum(distRoutes.begin(), distRoutes.end())) / 100;

    // Objective: minimize the lateness, then the number of trucks used, then the distance traveled
    model.minimize(totalLateness);
    model.minimize(nbVecUsed);
    model.minimize(totalDistance);

    model.close();

    // Parametrize the solver
    localsolver.getParam().setTimeLimit(limit);

    localsolver.solve();

    return Solution(routes);
}