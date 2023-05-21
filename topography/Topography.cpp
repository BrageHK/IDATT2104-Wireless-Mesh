#include "../node/Node.h"
#include "Topography.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <random>

double interpolate(double g00, double g01, double g10, double g11, double x, double y) {
    // Use bilinear interpolation
    double a00 = g00;
    double a10 = g10 - g00;
    double a01 = g01 - g00;
    double a11 = g00 - g01 - g10 + g11;

    return a00 + a10 * x + a01 * y + a11 * x * y;
}

double euclidean(double x1, double y1, double x2, double y2, double skew) {
    // Euclidean function with a skew factor
    double dx = x1 - x2;
    double dy = (y1 - y2) * skew;
    return std::sqrt(dx*dx + dy*dy);
}

double gaussian(double x, double mu, double sigma) {
    // Gaussian function
    return std::exp(-(x - mu) * (x - mu) / (2 * sigma * sigma));
}


std::vector<std::vector<int>> Topography::generateMountainElevation(int rows, int cols, int minElevation, int maxElevation) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(minElevation, maxElevation);
    std::uniform_int_distribution<> positionDist(0, rows-1);
    std::uniform_real_distribution<> skewDist(0.7, 1.3);  // Distribution for the skew factor

    // Initialize elevation data to all zeros
    std::vector<std::vector<int>> data(rows, std::vector<int>(cols, 0));

    // Parameters for Perlin noise
    const int numOctaves = 4;  // number of layers of noise to generate
    const double persistence = 0.5;  // how much each layer contributes to the final noise

    for (int octave = 0; octave < numOctaves; ++octave) {
        // Compute frequency and amplitude for this layer
        double frequency = std::pow(2.0, octave);
        double amplitude = std::pow(persistence, octave);

        // Create a grid of random gradients for this layer
        std::vector<std::vector<double>> gradients(rows, std::vector<double>(cols));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                gradients[i][j] = distrib(gen) / static_cast<double>(maxElevation);
            }
        }

        // Compute noise for this layer by interpolating the gradient grid
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                double x = (double)j / cols * frequency;
                double y = (double)i / rows * frequency;

                // Get the integer coordinates of the four corners of the grid cell containing (x, y)
                int x0 = (int)std::floor(x);
                int y0 = (int)std::floor(y);
                int x1 = x0 + 1;
                int y1 = y0 + 1;

                // Get the gradients at the corners of the cell
                double g00 = gradients[y0 % rows][x0 % cols];
                double g01 = gradients[y1 % rows][x0 % cols];
                double g10 = gradients[y0 % rows][x1 % cols];
                double g11 = gradients[y1 % rows][x1 % cols];

                // Interpolate between the gradients to get the noise value at (x, y)
                double noise = interpolate(g00, g01, g10, g11, x - x0, y - y0);

                // Add to the total noise at this point
                data[i][j] += static_cast<int>(noise * amplitude * (maxElevation - minElevation));
            }
        }
    }
    int numPeaks = std::sqrt(rows * cols) / 50;

    struct Peak {
        int x, y;
        double skew;
    };// Change this to control how many peaks to generate

    std::vector<Peak> peaks(numPeaks);
    for (int i = 0; i < numPeaks; ++i) {
        peaks[i] = {positionDist(gen), positionDist(gen), skewDist(gen)};  // Random position and skew
    }


    // Create a "peak map"
    std::vector<std::vector<int>> peakMap(rows, std::vector<int>(cols, 0));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Calculate contribution from all peaks
            for (const auto& peak : peaks) {
                double dist = euclidean(i, j, peak.x, peak.y, peak.skew);
                int peakElevation = maxElevation + 20 * gaussian(dist, 0, rows/10 * peak.skew);  // sigma depends on rows size and skew
                peakMap[i][j] += std::min(peakElevation, maxElevation + 20);
            }
            if (peakMap[i][j] < 0) {
                peakMap[i][j] = 0;
            }
        }
    }

    // Combine the Perlin noise map and the peak map
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] += peakMap[i][j];
        }
    }

    return data;
}


std::vector<std::vector<int>> Topography::getElevationData() {
    return elevationData;
}

void Topography::setElevationData(const std::vector<std::vector<int>> &elevationData) {
    Topography::elevationData = elevationData;
}

std::vector<std::vector<int>> Topography::generateCityElevation(int rows, int cols, int minBuildingHeight, int maxBuildingHeight, int numBuildings, int roadWidth, int roadSpacing) {
    std::vector<std::vector<int>> data(rows, std::vector<int>(cols, 0));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> heightDistrib(minBuildingHeight, maxBuildingHeight);
    std::uniform_int_distribution<> rowDistrib(0, rows - 1);
    std::uniform_int_distribution<> colDistrib(0, cols - 1);

    // Calculate the maximum building size based on the number of buildings
    int maxBuildingSize = std::max(rows, cols) / std::sqrt(numBuildings);

    // Generate roads
    for (int i = 0; i < rows; i += roadSpacing) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < roadWidth && i + k < rows; ++k) {
                data[i + k][j] = 0; // Set the height to 0 for road
            }
        }
    }

    for (int i = 0; i < cols; i += roadSpacing) {
        for (int j = 0; j < rows; ++j) {
            for (int k = 0; k < roadWidth && i + k < cols; ++k) {
                data[j][i + k] = 0; // Set the height to 0 for road
            }
        }
    }

    // Generate buildings
    int attempts = 0; // Keep track of the number of attempts to place buildings
    int buildingsPlaced = 0; // Keep track of the number of buildings placed
    while (buildingsPlaced < numBuildings && attempts < numBuildings * 10) { // limit the attempts to 10 times the number of buildings
        int buildingHeight = heightDistrib(gen);  // Generate a height for the building
        int buildingSize = gen() % maxBuildingSize + 1; // Generate a size for the building
        int startRow = rowDistrib(gen); // Generate a random starting row for the building
        int startCol = colDistrib(gen); // Generate a random starting column for the building

        // Determine the size of the building such that it doesn't exceed the grid
        int endRow = std::min(startRow + buildingSize, rows);
        int endCol = std::min(startCol + buildingSize, cols);

        // Check for overlap with existing buildings and roads
        bool overlap = false;
        for (int row = startRow; row < endRow; ++row) {
            for (int col = startCol; col < endCol; ++col) {
                if (data[row][col] > 0 || row % roadSpacing < roadWidth || col % roadSpacing < roadWidth) {
                    overlap = true;
                    break;
                }
            }
            if (overlap) break;
        }

        // If there's no overlap, place the building on the grid
        if (!overlap) {
            for (int row = startRow; row < endRow; ++row) {
                for (int col = startCol; col < endCol; ++col) {
                    data[row][col] = buildingHeight;
                }
            }
            buildingsPlaced++; // Increment the count of buildings placed
        }
        attempts++; // Increment the count of attempts to place buildings
    }

    return data;
}

    void Topography::writeElevationData(const std::string& filename) {
        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cout << "Unable to open file for writing: " << filename << std::endl;
            return;
        }

        for (const auto& row : elevationData) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i != row.size() - 1) {
                    file << " ";
                }
            }
            file << "\n";
        }
    }

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

    int Topography::getHeight(int x, int y) {
        if (x < 0 || x >=elevationData[0].size() || y < 0 || y >= elevationData.size()) {
            throw std::out_of_range("Coordinates out of range");
        }
        return elevationData[y][x];
    }


    std::tuple<int, int, int> Topography::findObstruction(int startX, int startY, int startZ,
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
            if (getHeight(currentX, currentY) > currentZ) {
                return {currentX, currentY, currentZ};
            }
        }

        // Return special value if no obstruction found
        return {-1, -1, -1};
    }

    bool Topography::isObstructionBetween(int startX, int startY, int startZ,
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
            if (getHeight(currentX, currentY) > currentZ) {
                return true;
            }
        }

        // No obstruction found
        return false;
    }


int getGrayscale(int elevation, int minElevation, int maxElevation) {
    return 255 - ((elevation - minElevation) * 255) / (maxElevation - minElevation);
}

    int getDroneInfluence(int dronePower, int distance) {
        if (distance == 0) {
            return 100;
        }
        // Implement the inverse square law
        double influence = dronePower / (2.0 *M_PI * distance * distance);
        // Scale influence to be between 0 and 100
        influence *= 100;
        if (influence > 100) {
            influence = 100;
        }
        return static_cast<int>(influence);
    }


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

    bool isDronePosition(const std::vector<Node*>& nodes, int x, int y) {
        for (const auto& node : nodes) {
            int distance = std::sqrt(std::pow(x - node->getX(), 2) + std::pow(y - node->getY(), 2));
            if (distance == 0) {
                return true;
            }
        }
        return false;
    }


int Topography::getTotalDroneInfluence(const std::vector<Node*>& nodes, int x, int y) {
    int totalInfluence = 0;
    for (const auto& node : nodes) {
        int distance = std::sqrt(std::pow(x - node->getX(), 2) + std::pow(y - node->getY(), 2));
        double range = std::sqrt(node->getSignalPower() / (2.0 * M_PI * 0.05));
        if (distance != 0 && distance <= range) {
            if (!isObstructionBetween(node->getX(), node->getY(), node->getZ(), x, y, elevationData[y][x])) {
                // Calculate influence directly here
                double influence = node->getSignalPower() / (2.0 * M_PI * distance * distance);
                influence *= 100;
                if (influence > 100) {
                    influence = 100;
                }
                totalInfluence += static_cast<int>(influence);
            }
        }
    }
    return totalInfluence > 70 ? 70 : totalInfluence;  // Cap total influence at 100
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


    void Topography::writeMapToBMP(const std::vector<Node*>& nodes,
                                   const std::vector<std::pair<Node*, Node*>>& connectedDrones,
                                   const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Unable to open file: " << filename << std::endl;
            return;
        }

        int width = elevationData[0].size();
        int height = elevationData.size();
        writeBMPHeaders(file, width, height);

        int minElevation = elevationData[0][0];
        int maxElevation = elevationData[0][0];
        for (const auto& row : elevationData) {
            for (const auto& elevation : row) {
                if (elevation < minElevation) {
                    minElevation = elevation;
                }
                if (elevation > maxElevation) {
                    maxElevation = elevation;
                }
            }
        }

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
                int grayscale = getGrayscale(elevationData[y][x], minElevation, maxElevation);
                int totalInfluence = getTotalDroneInfluence(nodes, x, y);
                if (isDronePosition(nodes, x, y)) {
                    file.put(0).put(0).put(255);
                } else if (linePoints[y][x]) {
                    file.put(255).put(0).put(0);
                } else if (totalInfluence > 0) {
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

void Topography::printMapToConsole(const std::vector<Node*>& nodes,
                                   const std::vector<std::pair<Node*, Node*>>& connectedDrones) {
    int width = elevationData[0].size();
    int height = elevationData.size();

    std::vector<std::vector<bool>> linePoints(height, std::vector<bool>(width, false));
    for (const auto& connectedDrone : connectedDrones) {
        auto points = drawLine(connectedDrone.first, connectedDrone.second);
        for (const auto& point : points) {
            linePoints[point.second][point.first] = true;
        }
    }

    int minElevation = elevationData[0][0];
    int maxElevation = elevationData[0][0];
    for (const auto& row : elevationData) {
        for (const auto& elevation : row) {
            if (elevation < minElevation) {
                minElevation = elevation;
            }
            if (elevation > maxElevation) {
                maxElevation = elevation;
            }
        }
    }

    // Influence chars
    std::string influenceChars = ".:-=+#%@";
    const int minInfluenceColor = 20;  // Set this to the minimum influence needed to color the character green

    // Write pixel data
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int elevation = elevationData[y][x];
            if (isDronePosition(nodes, x, y)) {
                std::cout << "\033[41mD";  // Drone position (red background)
            } else if (linePoints[y][x]) {
                std::cout << "\033[44mL";  // Connected drone line (blue background)
            } else {
                int totalInfluence = getTotalDroneInfluence(nodes, x, y);
                if (totalInfluence > 0) {
                    char influenceChar = influenceChars[totalInfluence / 10];
                    if (totalInfluence >= minInfluenceColor) {
                        std::cout << "\033[48;2;0;255;0m" << influenceChar;  // Influence character (green background)
                    } else {
                        std::cout << influenceChar;
                    }
                } else {
                    // Normalize elevation to 0-255 for grayscale
                    int normalizedElevation = 255 - static_cast<int>(((elevation - minElevation) / static_cast<double>(maxElevation - minElevation)) * 255);
                    std::cout << "\033[38;2;" << normalizedElevation << ";" << normalizedElevation << ";" << normalizedElevation << "m"  // Set foreground color
                              << "\033[48;2;" << normalizedElevation << ";" << normalizedElevation << ";" << normalizedElevation << "mO";  // Set background color and print character
                }
            }
        }
        std::cout << "\033[0m\n";  // Reset color after each line
    }
}



