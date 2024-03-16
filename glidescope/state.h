#ifndef GLIDESCOPE_STATE_H
#define GLIDESCOPE_STATE_H

#include "vrp.h"

struct GlobalState {
    VRPTK::VRP* vrp;
    int selected_node = -1, selected_solution = -1;
    std::vector<VRPTK::Solution*> solutions;
    void setSelectedNode(int node) { selected_node = node; }
};

#endif // GLIDESCOPE_STATE_H