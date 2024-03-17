#include "nodes.h"

bool GNodesView::draw(GlobalState* state) {
    if (!GView::draw(state)) return false;
    ImGui::Begin(name, nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    auto nodes = state->vrp->getNodes();
    if (ImGui::BeginTable("bodies_list", 4, ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Node");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Coords");
        ImGui::TableHeadersRow();
        for (int i = 0; i < nodes.rows(); i++) {
            char label[32];
            snprintf(label, 32, "#%i", i);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            bool selected = i == state->selected_node;
            if (ImGui::Selectable(label, selected, ImGuiSelectableFlags_SpanAllColumns)) {
                state->selected_node = i;
            }
            ImGui::TableNextColumn();
            ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV((int)nodes(i, 3) / 7.0f, 0.6f, 0.6f));
            ImGui::Text("%s", nodeTypeToAbbrev(nodes(i, 3)).c_str());
            ImGui::PopStyleColor();
            ImGui::TableNextColumn();
            ImGui::Text("(%i, %i)", (int)nodes(i, 1), (int)nodes(i, 2));
        }
        ImGui::EndTable();
    }
    ImGui::End();
    return true;
}