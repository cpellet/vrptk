#include "inspector.h"

bool GInspectorView::draw(GlobalState* state) {
    if (!GView::draw(state)) return false;
    int selected_node = state->selected_node;
    ImGui::Begin(name, &enabled, ImGuiWindowFlags_AlwaysAutoResize);
    if (selected_node == -1) {
        drawEmptyState();
    } else {
        VRPTW* vrp = state->dataset->getData();
        Eigen::MatrixXd nodes = vrp->getNodes();
        ImGui::Text("Node %i", selected_node);
        ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV((int)nodes(selected_node, 3) / 7.0f, 0.6f, 0.6f));
        ImGui::Text("%s", nodeTypeToString(nodes(selected_node, 3)).c_str());
        ImGui::PopStyleColor();
        ImGui::Text("X: %i, Y: %i", (int)nodes(selected_node, 1), (int)nodes(selected_node, 2));
        std::map<int, VRPTK::Request> requests = vrp->getRequests();
        bool should_show = false;
        for (auto it = requests.begin(); it != requests.end(); it++) {
            if (selected_node == it->second.getNode()) {
                if (!should_show) {
                    ImGui::SeparatorText("Requests");
                    should_show = true;
                }
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                if (ImGui::TreeNode(("#" + std::to_string(it->first)).c_str())) {
                    ImGui::Text("TW: %i to %i", it->second.getStart(), it->second.getEnd());
                    ImGui::Text("Quantity: %.2f", it->second.getQuantity());
                    ImGui::Text("Service Time: %.2f", it->second.getServiceTime());
                    ImGui::TreePop();
                }
            }
        }
        if (nodes(selected_node, 3) == 2 && state->dataset->getVariant() == Variant::EFVRPTW) {
            ImGui::SeparatorText("Station info");
            EFVRPTW* efvrptw = (EFVRPTW*)state->dataset->getData();
            std::vector<ChargingStation> stations = efvrptw->getStations();
            for (int i = 0; i < stations.size(); i++) {
                if (stations[i].getNode() == selected_node) {
                    ImGui::Text("Capacity: %.2f", stations[i].getCapacity());
                    ImGui::Text("Power: %.2f", stations[i].getPower());
                }
            }
        }
        drawRouteInfo(state);
    }
    ImGui::End();
    return true;
}

void GInspectorView::drawEmptyState() {
    std::string text = "No target";
    auto windowWidth = ImGui::GetWindowSize().x;
    auto windowHeight = ImGui::GetWindowSize().y;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::SetCursorPosY((windowHeight) * 0.5f);
    ImGui::Text("%s", text.c_str());
}

void GInspectorView::drawRouteInfo(const GlobalState* state) {
    if (state->selected_solution != -1) {
        ImGui::SeparatorText("Route");
        std::vector<std::vector<int> > routes = state->solutions[state->selected_solution]->getData()->getRoutes();
        int route_id = -1;
        for (int i = 0; i < routes.size(); i++) {
            if (std::find(routes[i].begin(), routes[i].end(), state->selected_node) != routes[i].end()) {
                route_id = i;
                break;
            }
        }
        if (route_id != -1) {
            ImGui::Text("Route %i", route_id);
        } else {
            ImGui::Text("Not in any route");
        }
    }
}