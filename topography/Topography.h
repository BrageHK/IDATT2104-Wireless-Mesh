#include "../node/Node.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>

 class Topography {

 private:
     std::vector<std::vector<int>> elevationData;

     int getTotalDroneInfluence(const std::vector<Node *> &nodes, int x, int y);

 public:

     Topography() : elevationData(500, std::vector<int>(500, 0)) {}

     void setElevationData(const std::vector<std::vector<int>> &elevationData);

     std::vector<std::vector<int>> getElevationData();

     std::vector<std::vector<int>> readElevationData(const std::string& filename);

     void writeMapToBMP(const std::vector<Node*>& nodes,
                        const std::vector<std::pair<Node*, Node*>>& connectedDrones,
                        const std::string& filename);

     std::tuple<int, int, int> findObstruction(int startX, int startY, int startZ, int endX, int endY, int endZ);

     bool isObstructionBetween(int startX, int startY, int startZ, int endX, int endY, int endZ);

     std::vector<std::vector<int>> generateMountainElevation(int rows, int cols, int minElevation, int maxElevation);

     void writeElevationData(const std::string &filename);

     std::vector<std::vector<int>>
     generateCityElevation(int rows, int cols, int minBuildingHeight, int maxBuildingHeight, int numBuildings, int roadWidth, int roadSpacing);

     int getHeight(int x, int y);


     void
     printMapToConsole(const std::vector<Node*> &nodes, const std::vector<std::pair<Node *, Node *>> &connectedDrones);
 };