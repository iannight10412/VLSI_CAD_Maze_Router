#ifndef TYPES_H
#define TYPES_H

enum Direction {
    NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3, UP = 4, DOWN = 5, NONE = -1
};

struct Net {
    int id;
    int layer1, x1, y1;
    int layer2, x2, y2;
};

struct Node {
    int x, y, layer;
    int cost, direction, f_cost;

    Node(int x, int y, int layer, int cost, int direction, int f_cost)
        : x(x), y(y), layer(layer), cost(cost), direction(direction), f_cost(f_cost) {}
};

struct CompareNode_Dijkstra {
    bool operator()(const Node& a, const Node& b) { return a.cost > b.cost; }
};

struct CompareNode_A_star {
    bool operator()(const Node& a, const Node& b) { return a.f_cost > b.f_cost; }
};

#endif