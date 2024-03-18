#ifndef GLIDESCOPE_VIEWS_SCHEDULE_H
#define GLIDESCOPE_VIEWS_SCHEDULE_H

#include <implot.h>

#include "view.h"
#include "utils.h"
#include "efvrptw.h"

class GScheduleView : public GView {
public:
    GScheduleView(std::string name) : GView(name) {}
    ~GScheduleView();
    bool draw(GlobalState* state) override;
    void init(const ImGuiID* dockspace) override {}
};

#endif // GLIDESCOPE_VIEWS_SCHEDULE_H