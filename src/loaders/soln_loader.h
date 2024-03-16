#ifndef VRPTK_SOLN_LOADER_H
#define VRPTK_SOLN_LOADER_H

#include "solution.h"
#include "loader.h"

#include <iostream>
#include <fstream>
#include <string>

namespace VRPTK {

class SolutionLoader: public Loader {
public:
    Solution* load(const char* filename);
};

}

#endif // VRPTK_SOLN_LOADER_H