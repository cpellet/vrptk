#ifndef VRPTK_DATASET_H
#define VRPTK_DATASET_H

#include <string>

namespace VRPTK {
    enum class Variant {
        VRPTW,
        EFVRPTW
    };
    template <typename T>
    class Dataset {
    private:
        std::string collection;
        std::string name;
        std::string patch;
        std::string filepath;
        T* vrp;
        Variant variant;
        bool isSolution;

    public:
        Dataset(std::string collection, std::string name, std::string patch, std::string filepath, T* vrp, Variant variant, bool isSolution)
            : collection(std::move(collection)), name(std::move(name)), patch(std::move(patch)), filepath(std::move(filepath)), vrp(vrp), variant(variant), isSolution(isSolution) {
        }

        std::string getCollection() const { return collection; }
        std::string getName() const { return name; }
        std::string getPatch() const { return patch; }
        std::string getFilepath() const { return filepath; }
        T* getData() const { return vrp; }
        Variant getVariant() const { return variant; }
        bool getIsSolution() const { return isSolution; }
    };
}

#endif // VRPTK_DATASET_H