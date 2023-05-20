#include "../node/Node.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>

 class Topography {

 public:
     void writeMapToBMP(const std::vector<std::vector<int>>& heightData,
                        const std::vector<Node*>& nodes,
                        const std::vector<std::pair<Node*, Node*>>& connectedDrones,
                        const std::string& filename);

     std::vector<std::vector<int>> readElevationData(const std::string& filename);

     std::tuple<int, int, int> findObstruction(const std::vector<std::vector<int>>& heightData,
                                                        int startX, int startY, int startZ,
                                                        int endX, int endY, int endZ);

     bool isObstructionBetween(const std::vector<std::vector<int>>& heightData,
                                        int startX, int startY, int startZ,
                                        int endX, int endY, int endZ);

};