#ifndef GLIDESCOPE_VIEWS_INSPECTOR_H
#define GLIDESCOPE_VIEWS_INSPECTOR_H

#include "view.h"
#include "utils.h"
#include "efvrptw.h"

class GInspectorView : public GView {
private:
    void drawRouteInfo(const GlobalState* state);
    void drawEmptyState();
public:
    GInspectorView(std::string name) : GView(name) {}
    ~GInspectorView();
    bool draw(GlobalState* state) override;
    void init(const ImGuiID* dockspace) override {}
};

#endif // GLIDESCOPE_VIEWS_INSPECTOR_H