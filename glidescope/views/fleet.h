#ifndef GLIDESCOPE_VIEWS_FLEET_H
#define GLIDESCOPE_VIEWS_FLEET_H

#include "view.h"
#include "utils.h"

class GFleetView : public GView {
public:
    GFleetView(std::string name) : GView(name) {}
    ~GFleetView();
    bool draw(GlobalState* state) override;
    void init(const ImGuiID* dockspace) override {}
};

#endif // GLIDESCOPE_VIEWS_FLEET_H