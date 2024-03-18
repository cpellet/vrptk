#ifndef VRPTK_LOADER_H
#define VRPTK_LOADER_H

#include "../dataset.h"
#include <sstream>

namespace VRPTK {
    template <typename T>
    class Loader {
    public:
        Dataset<T> load(const char* filename);
    };
}

#endif // VRPTK_LOADER_H