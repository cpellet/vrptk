#ifndef VRPTK_REQUEST_H
#define VRPTK_REQUEST_H

namespace VRPTK {

class Request {
private:
    int id, node, start, end;
    double quantity, service_time;
public:
    Request(int id, int node, int start, int end, double quantity, double service_time)
        : id(id), node(node), start(start), end(end), quantity(quantity), service_time(service_time) {}

    int getId() const { return id; }
    int getNode() const { return node; }
    int getStart() const { return start; }
    int getEnd() const { return end; }
    double getQuantity() const { return quantity; }
    double getServiceTime() const { return service_time; }
};

}

#endif // VRPTK_REQUEST_H