#ifndef VRPTK_SOLN_LOADER_H
#define VRPTK_SOLN_LOADER_H

#include "solution.h"
#include "loader.h"

#include <fstream>

namespace VRPTK {
    class SolutionLoader : public Loader<Solution> {
    public:
        Dataset<Solution>* load(const char* filename);
    };

}

#endif // VRPTK_SOLN_LOADER_H