#ifndef VRPTK_XML_LOADER_H
#define VRPTK_XML_LOADER_H

#include <map>
#include "tinyxml2.h"
#include <iostream>

#include "loader.h"
#include "../vrp.h"

namespace VRPTK {
    class XMLLoader : public Loader {
    public:
        VRP* load(const char* filename);
        std::vector<Node> loadNodes(tinyxml2::XMLDocument* doc);
        std::map<int, Request> loadRequests(tinyxml2::XMLDocument* doc);
    };
}

#endif // VRPTK_XML_LOADER_H