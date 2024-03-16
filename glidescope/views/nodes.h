#ifndef GLIDESCOPE_VIEWS_NODES_H
#define GLIDESCOPE_VIEWS_NODES_H

#include "view.h"
#include "utils.h"

class GNodesView : public GView {
public:
    GNodesView(std::string name) : GView(name) {}
    ~GNodesView();
    bool draw(GlobalState* state) override;
    void init(const ImGuiID* dockspace) override {}
};

#endif // GLIDESCOPE_VIEWS_NODES_H