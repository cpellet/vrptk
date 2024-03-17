#include "fleet.h"

bool GFleetView::draw(GlobalState* state) {
    if (!GView::draw(state)) return false;
    ImGui::Begin(name, nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    std::vector<VRPTK::VehicleProfile> fleet = state->vrp->getFleet();
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Profiles")) {
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
        ImGui::TreePop();
    }
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 20);
    ImGui::Text("Total vehicles: %i", state->vrp->getNumVehicles());
    ImGui::End();
    return true;
}