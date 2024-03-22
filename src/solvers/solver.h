#ifndef VRPTK_SOLVER_H
#define VRPTK_SOLVER_H

#include "solution.h"
#include "vrptw.h"

namespace VRPTK {
    class Solver {
    protected:
        VRPTW* vrptw;
    public:
        Solver(VRPTW* vrptw) : vrptw(vrptw) {}
        virtual Solution solve(int limit) = 0;
    };
}

#endif // VRPTK_SOLVER_H