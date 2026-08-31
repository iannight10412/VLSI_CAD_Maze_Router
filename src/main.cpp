#include "../include/IO.h"
#include "../include/Router.h"
#include "Types.h"
#include "IO.h"
#include "Router.h"
#include <iostream>
#include <chrono>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <grid_file> <nl_file> <output_prefix>\n";
        return 1;
    }

    std::string gridFile = argv[1];
    std::string netsFile = argv[2];
    std::string outPrefix = argv[3];

    int gridX, gridY, bendPenalty, viaPenalty;
    std::vector<std::vector<std::vector<int>>> gridMap;
    std::vector<Net> nets;

    readGridFile(gridFile, gridX, gridY, bendPenalty, viaPenalty, gridMap);
    readNetsFile(netsFile, nets);

    auto gridMap_Dijkstra = gridMap;
    auto gridMap_A_star = gridMap;

    std::ofstream fout_Dijkstra(outPrefix + "_Dijkstra.rout");
    std::ofstream fout_A_star(outPrefix + "_A_star.rout");

    fout_Dijkstra << nets.size() << "\n";
    fout_A_star << nets.size() << "\n";

    std::cout << "--- Running Dijkstra ---\n";
    auto start_Dijkstra = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < nets.size(); ++i) {
        routeNet_Dijkstra(nets[i], gridMap_Dijkstra, bendPenalty, viaPenalty, fout_Dijkstra);
    }
    auto duration_Dijkstra = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start_Dijkstra);

    std::cout << "--- Running A* ---\n";
    auto start_A_star = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < nets.size(); ++i) {
        routeNet_A_star(nets[i], gridMap_A_star, bendPenalty, viaPenalty, fout_A_star);
    }
    auto duration_A_star = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start_A_star);

    std::cout << "\n========================================\n";
    std::cout << "          Performance Report            \n";
    std::cout << "========================================\n";
    std::cout << "Dijkstra Time : " << duration_Dijkstra.count() << " ms\n";
    std::cout << "A* Time       : " << duration_A_star.count() << " ms\n";
    if (duration_A_star.count() < duration_Dijkstra.count()) {
        std::cout << "Result        : A* is " << duration_Dijkstra.count() / duration_A_star.count() << "x faster!\n";
    }
    std::cout << "========================================\n";

    fout_Dijkstra.close();
    fout_A_star.close();
    return 0;
}