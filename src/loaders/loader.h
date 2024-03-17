#ifndef VRPTK_LOADER_H
#define VRPTK_LOADER_H

#include "../vrp.h"

namespace VRPTK {
    class Loader {
    public:
        VRP* load(const char* filename);
    };
}

#endif // VRPTK_LOADER_H