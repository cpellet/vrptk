#ifndef VRPTK_SOLUTION_H
#define VRPTK_SOLUTION_H

#include <vector>
#include <string>

namespace VRPTK {

class Solution {
private:
    std::string filename;
    std::vector<std::vector<int> > routes;
public:
    Solution(std::vector<std::vector<int> > routes, std::string filename) : routes(routes), filename(filename) {}

    std::vector<std::vector<int> > getRoutes() const {
        return routes;
    }

    std::string getFilename() const {
        return filename;
    }
};

}

#endif // VRPTK_SOLUTION_H