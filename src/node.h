#ifndef VRPTK_NODE_H
#define VRPTK_NODE_H

namespace VRPTK {
    class Node {
    private:
        int x, y, id, type;
    public:
        Node(int x, int y, int id, int type) : x(x), y(y), id(id), type(type) {}

        int getX() const { return x; }
        int getY() const { return y; }
        int getId() const { return id; }
        int getType() const { return type; }
    };
}

#endif // VRPTK_NODE_H