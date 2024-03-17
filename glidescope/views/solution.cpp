#include "solution.h"

bool GSolutionView::draw(GlobalState* state) {
    if (!GView::draw(state)) return false;
    ImGui::Begin(name, nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.15f, 0), false, window_flags);
    auto solutions = state->solutions;
    if (!solutions.empty()) {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Solutions")) {
            for (int i = 0; i < state->solutions.size(); i++) {
                if (ImGui::Selectable(solutions[i]->getFilename().c_str(), i == state->selected_solution)) {
                    state->selected_solution = i;
                }
            }
        } else {
            state->selected_solution = -1;
        }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("ChildR", ImVec2(0, 0), false, window_flags);
    if (state->selected_solution == -1) {
        drawEmptyState();
    } else {
        std::vector<std::vector<int> > routes = solutions[state->selected_solution]->getRoutes();
        if (ImGui::BeginTabBar("SolutionTabBar", ImGuiTabBarFlags_None)) {
            if (ImGui::BeginTabItem("Vehicles")) {
                drawSolutionPaths(state);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Metrics")) {
                drawSolutionMetrics(state);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::EndChild();
    ImGui::End();
    return true;
}

void GSolutionView::drawEmptyState() {
    std::string text = "No active solution";
    auto windowWidth = ImGui::GetWindowSize().x;
    auto windowHeight = ImGui::GetWindowSize().y;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::SetCursorPosY((windowHeight) * 0.5f);
    ImGui::Text("%s", text.c_str());
}

void GSolutionView::drawSolutionPaths(const GlobalState* state) {
    std::vector<std::vector<int> > routes = state->solutions[state->selected_solution]->getRoutes();
    ImGui::BeginTable("Solution", 2, ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Id");
    ImGui::TableSetupColumn("Itinerary");
    ImGui::TableHeadersRow();
    for (int i = 0; i < routes.size(); i++) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
        ImGui::Text("%i", i);
        ImGui::PopStyleColor();
        ImGui::TableNextColumn();
        std::string route_str = "";
        for (int j = 0; j < routes[i].size(); j++) {
            route_str += std::to_string(routes[i][j]);
            if (j < routes[i].size() - 1) {
                route_str += " -> ";
            }
        }
        ImGui::Text("%s", route_str.c_str());
    }
    ImGui::EndTable();
}

void GSolutionView::drawSolutionMetrics(const GlobalState* state) {
    std::vector<std::vector<int> > routes = state->solutions[state->selected_solution]->getRoutes();
    auto solutions = state->solutions;
    ImGui::Text("Total vehicles:");
    ImGui::SameLine(200); ImGui::Text("%zu", routes.size());
    ImGui::Text("Total distance:");
    ImGui::SameLine(200); ImGui::Text("%i", state->vrp->getTotalDistance(*solutions[state->selected_solution]));
}