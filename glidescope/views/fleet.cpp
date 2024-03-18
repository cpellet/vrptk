#include "fleet.h"

bool GFleetView::draw(GlobalState* state) {
    if (!GView::draw(state)) return false;
    ImGui::Begin(name, nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    VRPTW* vrp = state->dataset->getData();
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    int num_vehicles = 0;
    ImGui::SeparatorText("Vehicle profiles");
    if (state->dataset->getVariant() == Variant::EFVRPTW) {
        EFVRPTW* efvrptw = (EFVRPTW*)vrp;
        num_vehicles = efvrptw->getNumVehicles();
        std::vector<VRPTK::EVehicleProfile> fleet = efvrptw->getFleet();
        for (int i = 0; i < fleet.size(); i++) {
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            if (ImGui::TreeNode((std::string("#") + std::to_string(fleet[i].getType())).c_str())) {
                ImGui::Text("Trip: %i to %i", fleet[i].getDeparture(), fleet[i].getArrival());
                ImGui::Text("Vehicles: %i", fleet[i].getNumber());
                ImGui::Text("Capacity: %.2f", fleet[i].getCapacity());
                ImGui::Text("Max Time: %.2f", fleet[i].getMaxTime());
                EVehicleProfile* evrp = (EVehicleProfile*)&fleet[i];
                ImGui::Text("Energy: %.2f", evrp[i].getECapacity());
                ImGui::Text("Consumption: %.2f", evrp[i].getConsumption());
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                if (ImGui::TreeNode("SoC")) {
                    ImGui::Text("Initial: %.2f", evrp[i].getCurrent_SOC());
                    ImGui::Text("Min: %.2f", evrp[i].getMinSOC());
                    ImGui::Text("Max: %.2f", evrp[i].getMaxSOC());
                    ImGui::TreePop();
                }
                ImGui::Text("Fixed cost: %.2f", evrp[i].getFixedCost());
                ImGui::Text("Variable cost: %.2f", evrp[i].getVariableCost());
                ImGui::TreePop();
            }
        }
    } else {
        std::vector<VRPTK::VehicleProfile> fleet = vrp->getFleet();
        num_vehicles = vrp->getNumVehicles();
        for (int i = 0; i < fleet.size(); i++) {
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            if (ImGui::TreeNode(std::to_string(fleet[i].getType()).c_str())) {
                ImGui::Text("From %i to %i", fleet[i].getDeparture(), fleet[i].getArrival());
                ImGui::Text("Vehicles: %i", fleet[i].getNumber());
                ImGui::Text("Capacity: %.2f", fleet[i].getCapacity());
                ImGui::Text("Max Time: %.2f", fleet[i].getMaxTime());
                ImGui::TreePop();
            }
        }
    }
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 20);
    ImGui::Text("Total vehicles: %i", num_vehicles);
    ImGui::End();
    return true;
}