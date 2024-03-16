#include "utils.h"

std::string nodeTypeToString(int type) {
    switch (type) {
        case 0:
            return "Depot";
        case 1:
            return "Delivery";
        case 2:
            return "Charging station";
        default:
            return "Unknown";
    }
}

std::string nodeTypeToAbbrev(int type) {
    switch (type) {
        case 0:
            return "DEP";
        case 1:
            return "DEL";
        case 2:
            return "CS";
        default:
            return "U";
    }
}