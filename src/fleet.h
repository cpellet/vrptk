#ifndef VRPTK_FLEET_H
#define VRPTK_FLEET_H

namespace VRPTK {

class VehicleProfile {
private:
    int type, number, departure, arrival;
    double capacity, max_time;
public:
    VehicleProfile(int type, int number, int departure, int arrival, double capacity, double max_time)
        : type(type), number(number), departure(departure), arrival(arrival), capacity(capacity), max_time(max_time) {}

    int getType() const { return type; }
    int getNumber() const { return number; }
    int getDeparture() const { return departure; }
    int getArrival() const { return arrival; }
    double getCapacity() const { return capacity; }
    double getMaxTime() const { return max_time; }
};

}

#endif // VRPTK_FLEET_H