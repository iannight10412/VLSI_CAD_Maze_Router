#include "IO.h"
#include <iostream>
#include "Types.h"
#include <vector>
#include <string>
#include <fstream>
#include <tuple>

void readGridFile(const std::string& filename, int& gridX, int& gridY, int& bendPenalty, int& viaPenalty, std::vector<std::vector<std::vector<int>>>& gridMap) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    fin >> gridX >> gridY >> bendPenalty >> viaPenalty;
    gridMap.resize(2, std::vector<std::vector<int>>(gridY, std::vector<int>(gridX)));
    for (int layer = 0; layer < 2; ++layer)
        for (int y = 0; y < gridY; ++y)
            for (int x = 0; x < gridX; ++x)
                fin >> gridMap[layer][y][x];
    fin.close();
}

void readNetsFile(const std::string& filename, std::vector<Net>& nets) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    int netCount;
    fin >> netCount;
    nets.resize(netCount);
    for (int i = 0; i < netCount; ++i) {
        fin >> nets[i].id >> nets[i].layer1 >> nets[i].x1 >> nets[i].y1 >> nets[i].layer2 >> nets[i].x2 >> nets[i].y2;
        nets[i].layer1--; 
        nets[i].layer2--; 
    }
    fin.close();
}

void writeRouteResult(const Net& net, bool success, const std::vector<std::tuple<int, int, int>>& path, std::ofstream& fout) {
    fout << net.id << "\n";
    if (!success) {
        fout << "0\n"; 
        return;
    }
    for (size_t i = 0; i < path.size(); ++i) {
        int layer = std::get<0>(path[i]);
        int y = std::get<1>(path[i]);
        int x = std::get<2>(path[i]);
        fout << layer + 1 << " " << x << " " << y << "\n"; 
        if (i < path.size() - 1 && layer != std::get<0>(path[i + 1])) {
            fout << "3" << " " << x << " " << y << "\n"; 
        }
    }
    fout << "0\n"; 
}