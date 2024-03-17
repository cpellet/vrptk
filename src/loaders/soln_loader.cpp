#include "soln_loader.h"

using namespace VRPTK;

Solution* SolutionLoader::load(const char* filename) {
    std::string line;
    std::ifstream file(filename);
    std::vector<std::vector<int>> routes;
    while (std::getline(file, line)) {
        if (line.find("Route") != std::string::npos) {
            std::vector<int> route;
            std::string route_str = line.substr(line.find(":") + 1);
            std::stringstream ss(route_str);
            int node;
            while (ss >> node) {
                route.push_back(node);
            }
            routes.push_back(route);
        }
    }
    std::string filename_str;
    for (int i = strlen(filename) - 1; i >= 0; i--) {
        if (filename[i] == '/') {
            filename_str = filename + i + 1;
            break;
        }
    }
    return new Solution(routes, filename_str);
}