#include "schedule.h"

bool GScheduleView::draw(GlobalState* state) {
    if (!GView::draw(state)) return false;
    ImGui::Begin(name, nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    const std::map<int, VRPTK::Request> requests = state->dataset->getData()->getRequests();
    int selected_node = state->selected_node;
    if (requests.size() > 0) {
        if (ImGui::BeginTable("Schedule", 2, ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Id");
            ImGui::TableSetupColumn("Timeline");
            ImGui::TableHeadersRow();
            auto posYStart = ImGui::GetCursorPosY();
            for (auto request : requests) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)state->colors[request.first]);
                if (ImGui::Selectable(std::to_string(request.first).c_str(), selected_node == request.first, ImGuiSelectableFlags_SpanAllColumns)) {
                    state->selected_node = request.first;
                }
                ImGui::PopStyleColor();
                ImGui::TableNextColumn();
                auto table_draw_list = ImGui::GetWindowDrawList();
                auto start = request.second.getStart();
                auto end = request.second.getEnd();
                auto win_width = ImGui::GetWindowWidth() - 20;
                auto max_val = 1000;
                table_draw_list->AddRectFilled(ImVec2(ImGui::GetCursorScreenPos().x + (start * win_width / max_val), ImGui::GetCursorScreenPos().y + (selected_node == request.first ? 0 : 4)), ImVec2(ImGui::GetCursorScreenPos().x + (end * win_width / max_val), ImGui::GetCursorScreenPos().y + (selected_node == request.first ? 12 : 8)), (ImU32)state->colors[request.first]);
                if (selected_node == request.first) {
                    float local_y = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (start * win_width / max_val) - ImGui::CalcTextSize(std::to_string(start).c_str()).x - 5);
                    ImGui::Text("%i", start);
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (end * win_width / max_val) + 5);
                    ImGui::SetCursorPosY(local_y);
                    ImGui::Text("%i", end);
                }
            }
            ImGui::EndTable();
        }
        if (state->dataset->getVariant() == Variant::EFVRPTW) {
            EFVRPTW* efvrptw = (EFVRPTW*)state->dataset->getData();
            std::vector<PricePoint> prices = efvrptw->getPrices();
            std::vector<double> x;
            std::vector<double> y;
            double max_val = 0;
            for (auto price : prices) {
                x.push_back(price.getStart());
                y.push_back(price.getPrice());
                max_val = std::max(max_val, price.getStart());
            }
            if (ImPlot::BeginPlot("Energy Prices", "Time", "Price", ImVec2(-1, 200), ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_LockMin, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_LockMin)) {
                ImPlot::PlotLine("Global", x.data(), y.data(), x.size());
                ImPlot::EndPlot();
            }
        }
    }
    ImGui::End();
    return true;
}