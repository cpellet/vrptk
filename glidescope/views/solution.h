#ifndef GLIDESCOPE_VIEWS_SOLUTION_H
#define GLIDESCOPE_VIEWS_SOLUTION_H

#include "view.h"
#include "utils.h"

class GSolutionView : public GView {
private:
    void drawEmptyState();
    void drawSolutionPaths(const GlobalState* state);
    void drawSolutionMetrics(const GlobalState* state);
    void drawSolutionValidation(const GlobalState* state);
public:
    GSolutionView(std::string name) : GView(name) {}
    ~GSolutionView();
    bool draw(GlobalState* state) override;
    void init(const ImGuiID* dockspace) override {}
};

#endif // GLIDESCOPE_VIEWS_SOLUTION_H