#include "../node/Node.h"
#include "Topography.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>

    std::vector<std::vector<int>> Topography::readElevationData(const std::string& filename) {
        std::vector<std::vector<int>> data;
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cout << "Unable to open file: " << filename << std::endl;
            return data;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::vector<int> row;
            std::istringstream iss(line);
            int value;

            while (iss >> value) {
                row.push_back(value);
            }

            data.push_back(row);
        }

        return data;
    }

    int getHeight(const std::vector<std::vector<int>>& heightData, int x, int y) {
        if (x < 0 || x >= heightData[0].size() || y < 0 || y >= heightData.size()) {
            throw std::out_of_range("Coordinates out of range");
        }
        return heightData[y][x];
    }


    std::tuple<int, int, int> Topography::findObstruction(const std::vector<std::vector<int>>& heightData,
                                                          int startX, int startY, int startZ,
                                                          int endX, int endY, int endZ) {
        // Calculate differences in x, y, and z dimensions
        int diffX = endX - startX;
        int diffY = endY - startY;
        int diffZ = endZ - startZ;

        // Calculate the number of steps
        int steps = std::max({abs(diffX), abs(diffY), abs(diffZ)});

        // Calculate the step size for each dimension
        double stepX = static_cast<double>(diffX) / steps;
        double stepY = static_cast<double>(diffY) / steps;
        double stepZ = static_cast<double>(diffZ) / steps;

        // Iterate over each step
        for (int i = 0; i <= steps; i++) {
            // Calculate the current position
            int currentX = static_cast<int>(startX + i * stepX);
            int currentY = static_cast<int>(startY + i * stepY);
            int currentZ = static_cast<int>(startZ + i * stepZ);

            // Check if the actual height at this point is greater than the interpolated height
            if (getHeight(heightData, currentX, currentY) > currentZ) {
                return {currentX, currentY, currentZ};
            }
        }

        // Return special value if no obstruction found
        return {-1, -1, -1};
    }

    bool Topography::isObstructionBetween(const std::vector<std::vector<int>>& heightData,
                                          int startX, int startY, int startZ,
                                          int endX, int endY, int endZ) {
        // Calculate differences in x, y, and z dimensions
        int diffX = endX - startX;
        int diffY = endY - startY;
        int diffZ = endZ - startZ;

        // Calculate the number of steps
        int steps = std::max({abs(diffX), abs(diffY), abs(diffZ)});

        // Calculate the step size for each dimension
        double stepX = static_cast<double>(diffX) / steps;
        double stepY = static_cast<double>(diffY) / steps;
        double stepZ = static_cast<double>(diffZ) / steps;

        // Iterate over each step
        for (int i = 0; i <= steps; i++) {
            // Calculate the current position
            int currentX = static_cast<int>(startX + i * stepX);
            int currentY = static_cast<int>(startY + i * stepY);
            int currentZ = static_cast<int>(startZ + i * stepZ);

            // Check if the actual height at this point is greater than the interpolated height
            if (getHeight(heightData, currentX, currentY) > currentZ) {
                return true;
            }
        }

        // No obstruction found
        return false;
    }

    /*

    double calculateCoverageArea() {
        std::vector<std::vector<std::vector<bool>>> grid(GRID_WIDTH, std::vector<std::vector<bool>>(GRID_HEIGHT, std::vector<bool>(GRID_DEPTH, false)));
        int coveredGrids = 0;
        int totalGrids = GRID_WIDTH * GRID_HEIGHT * GRID_DEPTH;

        for (Node& node : nodes) {
            int startX = std::max(0, node.x - node.getRange());
            int endX = std::min(GRID_WIDTH - 1, node.x + node.getRange());
            int startY = std::max(0, node.y - node.getRange());
            int endY = std::min(GRID_HEIGHT - 1, node.y + node.getRange());
            int startZ = std::max(0, node.z - node.getRange());
            int endZ = std::min(GRID_DEPTH - 1, node.z + node.getRange());

            for (int x = startX; x <= endX; x++) {
                for (int y = startY; y <= endY; y++) {
                    for (int z = startZ; z <= endZ; z++) {
                        if (!grid[x][y][z] && node.calculateDistanceTo(x, y, z) <= node.getRange()) {
                            grid[x][y][z] = true;
                            coveredGrids++;
                        }
                    }
                }
            }
        }

        return (double)coveredGrids / totalGrids;
    }

    double calculateSignalStrength() {
        double signalStrengthScore = 0.0;
        int totalNodes = nodes.size();
        int totalPossiblePairs = 0;

        for (Node& node : nodes) {
            for (Node& otherNode : nodes) {
                if (&node != &otherNode) {
                    double strength = node.calculateSignalStrength(otherNode);
                    if (strength >= MIN_SIGNAL_STRENGTH) {
                        signalStrengthScore += strength;
                        totalPossiblePairs++;
                    }
                }
            }
        }
        return signalStrengthScore /= totalPossiblePairs;
        }

    double calculateConnectivity() {
        int totalNodes = nodes.size();
        int totalPossiblePairs = totalNodes * (totalNodes - 1) / 2;
        int connectedPairs = 0;

        for (Node& node : nodes) {
            for (Node& otherNode : nodes) {
                if (&node != &otherNode) {
                    if (node.calculateDistanceTo(otherNode) <= MAX_CONNECTION_DISTANCE) {
                        connectedPairs++;
                    }
                }
            }
        }

        return (double)connectedPairs / totalPossiblePairs;
}





    double calculateFitness() {
        double coverageScore = calculateCoverageArea();
        double connectivityScore = calculateConnectivity();
        double signalStrengthScore = calculateSignalStrength();

        return (coverageScore + connectivityScore + signalStrengthScore) / 3.0;
    }

    void printMapToFile(const std::vector<std::vector<int>>& heightData,
                        const std::vector<std::tuple<int, int>>& dronePositions,
                        const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Unable to open file: " << filename << std::endl;
            return;
        }

        for (int y = 0; y < heightData.size(); y++) {
            for (int x = 0; x < heightData[0].size(); x++) {
                // Check if a drone is at this position
                auto it = std::find(dronePositions.begin(), dronePositions.end(), std::make_tuple(x, y));
                if (it != dronePositions.end()) {
                    file << 'D';  // Print 'D' for the drone's position
                } else {
                    // Print a character representing the height at this point
                    char heightChar = '0' + std::min(heightData[y][x], 9);
                    file << heightChar;
                }
            }
            file << '\n';
        }
        file.close();
    }

    // Function to write the map data to a PPM image file
    void writeMapToPPM(const std::vector<std::vector<int>>& heightData,
                       const std::vector<std::tuple<int, int>>& dronePositions,
                       const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Unable to open file: " << filename << std::endl;
            return;
        }

        int width = heightData[0].size();
        int height = heightData.size();

        // PPM header
        file << "P3\n";
        file << width << ' ' << height << '\n';
        file << "255\n";

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Check if a drone is at this position
                auto it = std::find(dronePositions.begin(), dronePositions.end(), std::make_tuple(x, y));
                if (it != dronePositions.end()) {
                    // Print red color for the drone's position
                    file << "255 0 0 ";
                } else {
                    // Print grayscale color representing the height at this point
                    int value = 255 - (heightData[y][x] * 255) / 20;  // 20 is the max elevation
                    file << value << ' ' << value << ' ' << value << ' ';
                }
            }
            file << '\n';
        }

        file.close();
    }

    bool isNearDrone(int x, int y, const std::vector<std::tuple<int, int>>& dronePositions) {
        for (const auto& drone : dronePositions) {
            int dx = x - std::get<0>(drone);
            int dy = y - std::get<1>(drone);
            if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1) {
                return true;
            }
        }
        return false;
    }

    bool isInDroneRange(int x, int y, const std::vector<std::tuple<int, int, int>>& dronePositions) {
        for (const auto& drone : dronePositions) {
            int dx = x - std::get<0>(drone);
            int dy = y - std::get<1>(drone);
            int range = std::get<2>(drone);  // Assume the range is the third element in the tuple
            if (dx * dx + dy * dy <= range * range) {
                return true;
            }
        }
        return false;
    }

    int getDroneRange(int x, int y, const std::vector<std::tuple<int, int, int>>& dronePositions) {
        for (const auto& drone : dronePositions) {
            int dx = x - std::get<0>(drone);
            int dy = y - std::get<1>(drone);
            int range = std::get<2>(drone);  // Assume the range is the third element in the tuple
            int distanceSquared = dx * dx + dy * dy;
            if (distanceSquared <= range * range) {
                // Return the remaining range as a percentage
                return ((range * range - distanceSquared) * 100) / (range * range);
            }
        }
        return 0;
    }

    std::tuple<int, int, int> getColor(int elevation) {
        // Values are arbitrary and can be adjusted to fit your needs
        if (elevation < 5)
            return {102, 255, 102}; // Light Green
        else if (elevation < 10)
            return {0, 153, 0}; // Dark Green
        else if (elevation < 15)
            return {153, 76, 0}; // Brown
        else
            return {102, 51, 0}; // Dark Brown
    }








    void writeMapToBMP(const std::vector<std::vector<int>>& heightData,
                       const std::vector<std::tuple<int, int, int>>& dronePositions,
                       const std::vector<std::pair<int, int>>& connectedDrones,
                       const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Unable to open file: " << filename << std::endl;
            return;
        }

        int width = heightData[0].size();
        int height = heightData.size();

        // Bitmap file header (14 bytes)
        file.put('B').put('M');  // Magic number
        file.put(0).put(0).put(0).put(0);  // File size in bytes (will fill later)
        file.put(0).put(0);  // Reserved field
        file.put(0).put(0);  // Reserved field
        file.put(54).put(0).put(0).put(0);  // Offset of pixel data inside the image

        // Bitmap info header (40 bytes)
        file.put(40).put(0).put(0).put(0);  // Info header size
        file.write(reinterpret_cast<const char*>(&width), 4);  // Image width
        file.write(reinterpret_cast<const char*>(&height), 4);  // Image height
        file.put(1).put(0);  // Number of color planes
        file.put(24).put(0);  // Bits per pixel
        file.put(0).put(0).put(0).put(0);  // Compression
        file.put(0).put(0).put(0).put(0);  // Image size
        file.put(0).put(0).put(0).put(0);  // Horizontal resolution
        file.put(0).put(0).put(0).put(0);  // Vertical resolution
        file.put(0).put(0).put(0).put(0);  // Number of colors
        file.put(0).put(0).put(0).put(0);  // Important colors

        // Write pixel data
        auto it = dronePositions.begin();


        std::vector<std::vector<bool>> linePoints(height, std::vector<bool>(width, false));


        for (const auto& connectedDrone : connectedDrones) {

            int indexA = connectedDrone.first;
            int indexB = connectedDrone.second;

            std::pair<int, int> pointA = std::make_pair(std::get<0>(dronePositions[indexA]), std::get<1>(dronePositions[indexA]));
            std::pair<int, int> pointB = std::make_pair(std::get<0>(dronePositions[indexB]), std::get<1>(dronePositions[indexB]));

            auto points = drawLine(pointA, pointB);
            for (const auto& point : points) {
                int x = point.first;
                int y = point.second;
                linePoints[y][x] = true;
            }
        }


        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int grayscale = getGrayscale(heightData[y][x]);
                int totalInfluence = 0;
                bool isDrone = false;
                for (const auto& drone : dronePositions) {
                    int distance = std::sqrt(std::pow(x - std::get<0>(drone), 2) + std::pow(y - std::get<1>(drone), 2));
                    if (distance == 0) {
                        isDrone = true;
                        break;
                    } else {
                        totalInfluence += getDroneInfluence(std::get<2>(drone), distance);
                    }
                }
                totalInfluence = totalInfluence > 100 ? 100 : totalInfluence;  // Cap total influence at 100
                if (isDrone) {
                    file.put(0).put(0).put(255);
                } else if (linePoints[y][x]) {
                    file.put(255).put(0).put(0);
                } else if (totalInfluence > 0 && heightData[y][x] <= 0) {
                    file.put(static_cast<int>((grayscale * (100.0 - totalInfluence)) / 100.0));
                    file.put(static_cast<int>((grayscale * (100.0 - totalInfluence) + totalInfluence * 255.0) / 100.0));
                    file.put(static_cast<int>((grayscale * (100.0 - totalInfluence)) / 100.0));
                } else {
                    file.put(grayscale);
                    file.put(grayscale);
                    file.put(grayscale);
                }
            }
        }
        auto fileSize = file.tellp();
        file.seekp(2);
        file.write(reinterpret_cast<const char*>(&fileSize), 4);
        file.close();
    }*/

    int getGrayscale(int elevation) {
        return 255 - (elevation * 255) / 20;  // 20 is the max elevation
    }

    int getDroneInfluence(int dronePower, int distance) {
        if (distance == 0) {
            return 100;
        }
        // Implement the inverse square law
        double influence = dronePower / (2.0 * M_PI * distance * distance);
        // Scale influence to be between 0 and 100
        influence *= 100;
        if (influence > 100) {
            influence = 100;
        }
        return static_cast<int>(influence);
    }

    /*
    std::vector<std::pair<int, int>> drawLine(std::pair<int, int> pointA, std::pair<int, int> pointB) {
        int x0 = pointA.first, y0 = pointA.second;
        int x1 = pointB.first, y1 = pointB.second;

        int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;

        std::vector<std::pair<int, int>> linePoints;

        while (true) {
            linePoints.push_back(std::make_pair(x0, y0));
            if (x0 == x1 && y0 == y1)
                break;
            e2 = 2 * err;
            if (e2 >= dy) {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx) {
                err += dx;
                y0 += sy;
            }
        }
        return linePoints;
    }*/

    std::vector<std::pair<int, int>> drawLine(Node* nodeA, Node* nodeB) {
        int x0 = nodeA->getX(), y0 = nodeA->getY();
        int x1 = nodeB->getX(), y1 = nodeB->getY();

        int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;

        std::vector<std::pair<int, int>> linePoints;

        while (true) {
            linePoints.push_back(std::make_pair(x0, y0));
            if (x0 == x1 && y0 == y1)
                break;
            e2 = 2 * err;
            if (e2 >= dy) {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx) {
                err += dx;
                y0 += sy;
            }
        }
        return linePoints;
    }

    /*
    std::vector<std::pair<int, int>> getLinePoints(const std::tuple<int, int, int>& droneA, const std::tuple<int, int, int>& droneB) {
        std::pair<int, int> pointA = std::make_pair(std::get<0>(droneA), std::get<1>(droneA));
        std::pair<int, int> pointB = std::make_pair(std::get<0>(droneB), std::get<1>(droneB));

        return drawLine(pointA, pointB);  // drawLine() should be defined somewhere in your code
    }*/

    /*
    std::vector<std::pair<int, int>> getLinePoints(Node* nodeA, Node* nodeB) {
        std::pair<int, int> pointA = std::make_pair(nodeA->x, nodeA->y);
        std::pair<int, int> pointB = std::make_pair(nodeB->x, nodeB->y);

        return drawLine(pointA, pointB);  // drawLine() should be defined somewhere in your code
    }*/

    /*
    bool isDronePosition(const std::vector<std::tuple<int, int, int>>& dronePositions, int x, int y) {
        for (const auto& drone : dronePositions) {
            int distance = std::sqrt(std::pow(x - std::get<0>(drone), 2) + std::pow(y - std::get<1>(drone), 2));
            if (distance == 0) {
                return true;
            }
        }
        return false;
    }*/

    bool isDronePosition(const std::vector<Node*>& nodes, int x, int y) {
        for (const auto& node : nodes) {
            int distance = std::sqrt(std::pow(x - node->getX(), 2) + std::pow(y - node->getY(), 2));
            if (distance == 0) {
                return true;
            }
        }
        return false;
    }

    /*
    int getTotalDroneInfluence(const std::vector<std::tuple<int, int, int>>& dronePositions, int x, int y) {
        int totalInfluence = 0;
        for (const auto& drone : dronePositions) {
            int distance = std::sqrt(std::pow(x - std::get<0>(drone), 2) + std::pow(y - std::get<1>(drone), 2));
            if (distance != 0) {
                totalInfluence += getDroneInfluence(std::get<2>(drone), distance);
            }
        }
        return totalInfluence > 100 ? 100 : totalInfluence;  // Cap total influence at 100
    }*/

    int getTotalDroneInfluence(const std::vector<Node*>& nodes, int x, int y) {
        int totalInfluence = 0;
        for (const auto& node : nodes) {
            int distance = std::sqrt(std::pow(x - node->getX(), 2) + std::pow(y - node->getY(), 2));
            if (distance != 0) {
                totalInfluence += getDroneInfluence(node->getSignalPower(), distance);
            }
        }
        return totalInfluence > 100 ? 100 : totalInfluence;  // Cap total influence at 100
    }

    void writeInfluencedPixel(std::ofstream& file, int grayscale, int totalInfluence) {
        file.put(static_cast<int>((grayscale * (100.0 - totalInfluence)) / 100.0));
        file.put(static_cast<int>((grayscale * (100.0 - totalInfluence) + totalInfluence * 255.0) / 100.0));
        file.put(static_cast<int>((grayscale * (100.0 - totalInfluence)) / 100.0));
    }

    void writeGrayscalePixel(std::ofstream& file, int grayscale) {
        file.put(grayscale);
        file.put(grayscale);
        file.put(grayscale);
    }

    void writeBMPHeaders(std::ofstream& file, int width, int height) {
        // Bitmap file header (14 bytes)
        file.put('B').put('M');  // Magic number
        file.put(0).put(0).put(0).put(0);  // File size in bytes (will fill later)
        file.put(0).put(0);  // Reserved field
        file.put(0).put(0);  // Reserved field
        file.put(54).put(0).put(0).put(0);  // Offset of pixel data inside the image

        // Bitmap info header (40 bytes)
        file.put(40).put(0).put(0).put(0);  // Info header size
        file.write(reinterpret_cast<const char*>(&width), 4);  // Image width
        file.write(reinterpret_cast<const char*>(&height), 4);  // Image height
        file.put(1).put(0);  // Number of color planes
        file.put(24).put(0);  // Bits per pixel
        file.put(0).put(0).put(0).put(0);  // Compression
        file.put(0).put(0).put(0).put(0);  // Image size
        file.put(0).put(0).put(0).put(0);  // Horizontal resolution
        file.put(0).put(0).put(0).put(0);  // Vertical resolution
        file.put(0).put(0).put(0).put(0);  // Number of colors
        file.put(0).put(0).put(0).put(0);  // Important colors
    }

    /*
    void writeMapToBMP(const std::vector<std::vector<int>>& heightData,
                       const std::vector<std::tuple<int, int, int>>& dronePositions,
                       const std::vector<std::pair<int, int>>& connectedDrones,
                       const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Unable to open file: " << filename << std::endl;
            return;
        }

        int width = heightData[0].size();
        int height = heightData.size();
        writeBMPHeaders(file, width, height);

        std::vector<std::vector<bool>> linePoints(height, std::vector<bool>(width, false));
        for (const auto& connectedDrone : connectedDrones) {
            auto points = getLinePoints(dronePositions[connectedDrone.first], dronePositions[connectedDrone.second]);
            for (const auto& point : points) {
                linePoints[point.second][point.first] = true;
            }
        }

        // Write pixel data
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int grayscale = getGrayscale(heightData[y][x]);
                int totalInfluence = getTotalDroneInfluence(dronePositions, x, y);
                if (isDronePosition(dronePositions, x, y)) {
                    file.put(0).put(0).put(255);
                } else if (linePoints[y][x]) {
                    file.put(255).put(0).put(0);
                } else if (totalInfluence > 0 && heightData[y][x] <= 0) {
                    writeInfluencedPixel(file, grayscale, totalInfluence);
                } else {
                    writeGrayscalePixel(file, grayscale);
                }
            }
        }

        auto fileSize = file.tellp();
        file.seekp(2);
        file.write(reinterpret_cast<const char*>(&fileSize), 4);
        file.close();
    }*/

    void Topography::writeMapToBMP(const std::vector<std::vector<int>>& heightData,
                                   const std::vector<Node*>& nodes,
                                   const std::vector<std::pair<Node*, Node*>>& connectedDrones,
                                   const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Unable to open file: " << filename << std::endl;
            return;
        }

        int width = heightData[0].size();
        int height = heightData.size();
        writeBMPHeaders(file, width, height);

        std::vector<std::vector<bool>> linePoints(height, std::vector<bool>(width, false));
        for (const auto& connectedDrone : connectedDrones) {
            auto points = drawLine(connectedDrone.first, connectedDrone.second);
            for (const auto& point : points) {
                linePoints[point.second][point.first] = true;
            }
        }

        // Write pixel data
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int grayscale = getGrayscale(heightData[y][x]);
                int totalInfluence = getTotalDroneInfluence(nodes, x, y);
                if (isDronePosition(nodes, x, y)) {
                    file.put(0).put(0).put(255);
                } else if (linePoints[y][x]) {
                    file.put(255).put(0).put(0);
                } else if (totalInfluence > 0 && heightData[y][x] <= 0) {
                    writeInfluencedPixel(file, grayscale, totalInfluence);
                } else {
                    writeGrayscalePixel(file, grayscale);
                }
            }
        }

        auto fileSize = file.tellp();
        file.seekp(2);
        file.write(reinterpret_cast<const char*>(&fileSize), 4);
        file.close();
    }

