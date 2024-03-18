#ifndef VRPTK_SOLUTION_H
#define VRPTK_SOLUTION_H

#include <vector>

namespace VRPTK {
    class Solution {
    private:
        std::vector<std::vector<int> > routes;
    public:
        explicit Solution(const std::vector<std::vector<int> >& routes) : routes(routes) {}

        std::vector<std::vector<int> > getRoutes() const {
            return routes;
        }
    };
}

#endif // VRPTK_SOLUTION_H