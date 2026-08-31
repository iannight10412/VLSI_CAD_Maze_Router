#include "../include/IO.h"
#include "../include/Router.h"
#include <iostream>
#include <queue>
#include <climits>
#include <cmath>
#include <algorithm>
#include <tuple>
#include <fstream>

int getHeuristic(const Net& net, const int x, const int y, const int layer, const int viaPenalty) {
    return std::abs(x - net.x2) + std::abs(y - net.y2) + (layer - net.layer2) * viaPenalty; 
}

void routeNet_Dijkstra(const Net& net, std::vector<std::vector<std::vector<int>>>& gridMap, int bendPenalty, int viaPenalty, std::ofstream& fout) {
    std::vector<std::vector<std::vector<int>>> costMap(2, std::vector<std::vector<int>>(gridMap[0].size(), std::vector<int>(gridMap[0][0].size(), INT_MAX)));
    std::vector<std::vector<std::vector<int>>> parentsMap(2, std::vector<std::vector<int>>(gridMap[0].size(), std::vector<int>(gridMap[0][0].size(), -1)));
    gridMap[net.layer1][net.y1][net.x1] = 1;
    gridMap[net.layer2][net.y2][net.x2] = 1;
    
    const int dx[6] = {0, 1, 0, -1, 0, 0}; 
    const int dy[6] = {-1, 0, 1, 0, 0, 0};
    const int dz[6] = {0, 0, 0, 0, 1, -1};
    
    std::priority_queue<Node, std::vector<Node>, CompareNode_Dijkstra> pq;
    costMap[net.layer1][net.y1][net.x1] = 1;
    pq.push(Node(net.x1, net.y1, net.layer1, 1, -1, getHeuristic(net, net.x1, net.y1, net.layer1, viaPenalty))); 

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        if (current.cost > costMap[current.layer][current.y][current.x]) continue; 
        
        if (current.x == net.x2 && current.y == net.y2 && current.layer == net.layer2) {
            std::cout << "Net " << net.id << " routed with cost: " << current.cost << std::endl;
            std::vector<std::tuple<int,int,int>> path;
            int x = current.x, y = current.y, layer = current.layer;
            while (x != net.x1 || y != net.y1 || layer != net.layer1) {
                path.push_back(std::make_tuple(layer, y, x));
                int dir = parentsMap[layer][y][x];
                if (dir == -1) break; 
                x -= dx[dir]; y -= dy[dir]; layer -= dz[dir];
            }
            path.push_back(std::make_tuple(net.layer1, net.y1, net.x1)); 
            std::reverse(path.begin(), path.end());
            writeRouteResult(net, true, path, fout);
            for (size_t i = 0; i < path.size(); ++i) {
                gridMap[std::get<0>(path[i])][std::get<1>(path[i])][std::get<2>(path[i])] = -1; 
            }
            return;
        }

        for (int dir = 0; dir < 6; ++dir) {
            int newX = current.x + dx[dir], newY = current.y + dy[dir], newLayer = current.layer + dz[dir];
            if (newX < 0 || newX >= gridMap[0][0].size() || newY < 0 || newY >= gridMap[0].size() || newLayer < 0 || newLayer >= 2) continue;
            if (gridMap[newLayer][newY][newX] == -1) continue;

            int newCost = current.cost + gridMap[newLayer][newY][newX]; 
            if (current.direction != -1 && current.direction < 4 && dir < 4 && current.direction != dir) newCost += bendPenalty; 
            if (current.layer != newLayer) newCost += viaPenalty; 

            if (newCost < costMap[newLayer][newY][newX]) {
                costMap[newLayer][newY][newX] = newCost;
                parentsMap[newLayer][newY][newX] = dir;
                pq.push(Node(newX, newY, newLayer, newCost, dir, newCost + getHeuristic(net, newX, newY, newLayer, viaPenalty))); 
            }
        }
    }
    std::vector<std::tuple<int, int, int>> emptyPath;
    writeRouteResult(net, false, emptyPath, fout);
}

void routeNet_A_star(const Net& net, std::vector<std::vector<std::vector<int>>>& gridMap, int bendPenalty, int viaPenalty, std::ofstream& fout) {
    std::vector<std::vector<std::vector<int>>> costMap(2, std::vector<std::vector<int>>(gridMap[0].size(), std::vector<int>(gridMap[0][0].size(), INT_MAX)));
    std::vector<std::vector<std::vector<int>>> parentsMap(2, std::vector<std::vector<int>>(gridMap[0].size(), std::vector<int>(gridMap[0][0].size(), -1)));
    gridMap[net.layer1][net.y1][net.x1] = 1;
    gridMap[net.layer2][net.y2][net.x2] = 1;
    
    const int dx[6] = {0, 1, 0, -1, 0, 0}; 
    const int dy[6] = {-1, 0, 1, 0, 0, 0};
    const int dz[6] = {0, 0, 0, 0, 1, -1};
    
    std::priority_queue<Node, std::vector<Node>, CompareNode_A_star> pq;
    costMap[net.layer1][net.y1][net.x1] = 1;
    pq.push(Node(net.x1, net.y1, net.layer1, 1, -1, 1 + getHeuristic(net, net.x1, net.y1, net.layer1, viaPenalty))); 

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        if (current.cost > costMap[current.layer][current.y][current.x]) continue; 
        
        if (current.x == net.x2 && current.y == net.y2 && current.layer == net.layer2) {
            std::cout << "Net " << net.id << " routed with cost: " << current.cost << std::endl;
            std::vector<std::tuple<int,int,int>> path;
            int x = current.x, y = current.y, layer = current.layer;
            while (x != net.x1 || y != net.y1 || layer != net.layer1) {
                path.push_back(std::make_tuple(layer, y, x));
                int dir = parentsMap[layer][y][x];
                if (dir == -1) break; 
                x -= dx[dir]; y -= dy[dir]; layer -= dz[dir];
            }
            path.push_back(std::make_tuple(net.layer1, net.y1, net.x1)); 
            std::reverse(path.begin(), path.end());
            writeRouteResult(net, true, path, fout);
            for (size_t i = 0; i < path.size(); ++i) {
                gridMap[std::get<0>(path[i])][std::get<1>(path[i])][std::get<2>(path[i])] = -1; 
            }
            return;
        }

        for (int dir = 0; dir < 6; ++dir) {
            int newX = current.x + dx[dir], newY = current.y + dy[dir], newLayer = current.layer + dz[dir];
            if (newX < 0 || newX >= gridMap[0][0].size() || newY < 0 || newY >= gridMap[0].size() || newLayer < 0 || newLayer >= 2) continue;
            if (gridMap[newLayer][newY][newX] == -1) continue;

            int newCost = current.cost + gridMap[newLayer][newY][newX]; 
            if (current.direction != -1 && current.direction < 4 && dir < 4 && current.direction != dir) newCost += bendPenalty; 
            if (current.layer != newLayer) newCost += viaPenalty; 

            if (newCost < costMap[newLayer][newY][newX]) {
                costMap[newLayer][newY][newX] = newCost;
                parentsMap[newLayer][newY][newX] = dir;
                pq.push(Node(newX, newY, newLayer, newCost, dir, newCost + getHeuristic(net, newX, newY, newLayer, viaPenalty)));
            }
        }
    }
    std::vector<std::tuple<int, int, int>> emptyPath;
    writeRouteResult(net, false, emptyPath, fout);
}