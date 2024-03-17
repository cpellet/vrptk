#ifndef VRPTK_DATASET_H
#define VRPTK_DATASET_H

#include <string>
#include "vrp.h"

namespace VRPTK {
    enum class Variant {
        VRPTW,
        EFVRPTW
    };
    class Dataset {
    private:
        std::string collection;
        std::string name;
        std::string patch;
        std::string filepath;
        VRP* vrp;
        Variant variant;

    public:
        Dataset(std::string collection, std::string name, std::string patch, std::string filepath, VRP* vrp, Variant variant)
            : collection(collection), name(name), patch(patch), filepath(filepath), vrp(vrp), variant(variant) {
        }

        std::string getCollection() const { return collection; }
        std::string getName() const { return name; }
        std::string getPatch() const { return patch; }
        std::string getFilepath() const { return filepath; }
        VRP* getVRP() const { return vrp; }
        Variant getVariant() const { return variant; }
    };
}

#endif // VRPTK_DATASET_H