#ifndef ROUTER_H
#define ROUTER_H

#include "Types.h"
#include <vector>
#include <fstream>

int getHeuristic(const Net& net, const int x, const int y, const int layer, const int viaPenalty);
void routeNet_Dijkstra(const Net& net, std::vector<std::vector<std::vector<int>>>& gridMap, int bendPenalty, int viaPenalty, std::ofstream& fout);
void routeNet_A_star(const Net& net, std::vector<std::vector<std::vector<int>>>& gridMap, int bendPenalty, int viaPenalty, std::ofstream& fout);

#endif