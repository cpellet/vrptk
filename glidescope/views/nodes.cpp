#include "nodes.h"

bool GNodesView::draw(GlobalState* state) {
    if (!GView::draw(state)) return false;
    ImGui::Begin(name, nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    auto nodes = state->dataset->getData()->getNodes();
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
            ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)state->colors[i]);
            if (ImGui::Selectable(label, selected, ImGuiSelectableFlags_SpanAllColumns)) {
                state->selected_node = i;
            }
            ImGui::PopStyleColor();
            ImGui::TableNextColumn();
            ImGui::Text("%s", nodeTypeToAbbrev(nodes(i, 3)).c_str());
            ImGui::TableNextColumn();
            ImGui::Text("(%i, %i)", (int)nodes(i, 1), (int)nodes(i, 2));
        }
        ImGui::EndTable();
    }
    ImGui::End();
    return true;
}