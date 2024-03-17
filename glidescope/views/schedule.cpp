#include "schedule.h"

bool GScheduleView::draw(GlobalState* state) {
    if (!GView::draw(state)) return false;
    ImGui::Begin(name, nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    const std::map<int, VRPTK::Request> requests = state->vrp->getRequests();
    int selected_node = state->selected_node;
    if (requests.size() > 0) {
        if (ImGui::BeginTable("Schedule", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY)) {
            ImGui::TableSetupColumn("Id");
            ImGui::TableSetupColumn("Timeline");
            ImGui::TableHeadersRow();
            auto posYStart = ImGui::GetCursorPosY();
            for (auto request : requests) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(request.first / 7.0f, 0.6f, 0.6f));
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
                table_draw_list->AddRectFilled(ImVec2(ImGui::GetCursorScreenPos().x + (start * win_width / max_val), ImGui::GetCursorScreenPos().y + (selected_node == request.first ? 0 : 5)), ImVec2(ImGui::GetCursorScreenPos().x + (end * win_width / max_val), ImGui::GetCursorScreenPos().y + (selected_node == request.first ? 17 : 10)), (ImU32)ImColor::HSV(request.first / 7.0f, 0.6f, 0.6f));
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
    }
    ImGui::End();
    return true;
}