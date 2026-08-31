#ifndef IO_H
#define IO_H

#include "types.h"
#include <vector>
#include <string>
#include <fstream>
#include <tuple>

void readGridFile(const std::string& filename, int& gridX, int& gridY, int& bendPenalty, int& viaPenalty, std::vector<std::vector<std::vector<int>>>& gridMap);
void readNetsFile(const std::string& filename, std::vector<Net>& nets);
void writeRouteResult(const Net& net, bool success, const std::vector<std::tuple<int, int, int>>& path, std::ofstream& fout);

#endif