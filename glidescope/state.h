#ifndef GLIDESCOPE_STATE_H
#define GLIDESCOPE_STATE_H

#include <imgui.h>
#include <map>

#include "vrptw.h"
#include "dataset.h"
#include "solution.h"

using namespace VRPTK;

struct GlobalState {
    Dataset<VRPTW>* dataset;
    int selected_node = -1, selected_solution = -1;
    std::vector<Dataset<Solution>*> solutions;
    void setSelectedNode(int node) { selected_node = node; }
    std::map<int, ImColor> colors;
};

#endif // GLIDESCOPE_STATE_H