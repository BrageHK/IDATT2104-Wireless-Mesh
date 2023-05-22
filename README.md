# IDATT2104-Wireless-Mesh

## Installations Instructions
this project contains software for a wireless mesh network.
There are two executable files provided for your convenience:
one for Windows users and another for MacOS and Linux users.

### Software Requirements:


- GCC Compiler (version 7.0 or above)
- Make
- A C++17 compatible system

### Steps to Compile and Run the Software:

**If you want to use the provided executable files, skip to step 4.**

1. #### Download or clone the project repository to your local machine.
2. #### Navigate to the project directory.

    For MacOS/Linux users:
    
        cd path_to_your_directory/Wireless-Mesh/
    
    For Windows users:
    
        cd path_to_your_directory\Wireless-Mesh\

3. #### Use the provided makefile to compile the project.

    For MacOS/Linux users:
    
        make
    
    For Windows users:
    First, make sure you have installed MinGW or any software that provides a Make environment for Windows.
    
        mingw32-make
    
    If the make command does not work, you can compile the project manually using the following command:
    
        g++ -std=c++17 -pthread -o Mesh main.cpp node/Node.cpp worker/Workers.cpp topography/Topography.cpp
    

4. #### Run the executable file.

    For MacOS/Linux users:
    
        ./mesh
    
    For Windows users:
    
        mesh.exe


## Elevation Generation

The `Topography` class offers two methods to generate elevation maps, generateMountainElevation and
generateCityElevation. Both methods produce a 2D matrix representing an area's height in each grid cell.

### Function Details

-`interpolate`: It performs bilinear interpolation based on the four corners of a square grid cell. 
It's used to interpolate elevation at a given point based on nearby grid cells.

-`euclidean`: This function calculates the Euclidean distance between two points,
with a skew factor for more diversity in elevation and terrain generation.

-`gaussian`: It's used to create a Gaussian distribution,
which helps shape the peaks of mountains and hills in the elevation map.

### Mountain Elevation Generation

`generateMountainElevation` method generates an elevation map designed to mimic the natural terrain found
in mountainous regions. It makes use of Perlin noise, a type of gradient noise often used in procedural
texture and terrain generation.

The function operates on the following principles:

- First, it creates an empty elevation data matrix filled with zeros.

- Next, it generates a series of Perlin noise maps. Each layer is generated with a different frequency and amplitude,
which then contribute to the final noise.
    
- The method uses a random skew factor to adjust the influence of each noise layer.
    
- Once all the noise layers have been generated, they are summed to create the final elevation map.
    
- Peaks are generated at random positions with a Gaussian function affecting their shape.

### City Elevation Generation

`generateCityElevation` method generates an elevation map representing a cityscape.
The map includes roads and buildings, each with varying heights.

The method operates on these key steps:

- It first creates an empty elevation data matrix filled with zeros.
- Then it generates a grid of roads across the city. The road width and spacing between roads are adjustable parameters.
- Buildings are placed on the grid randomly. Their heights and sizes are also randomly generated,
and the method ensures that buildings do not overlap with one another or with the roads.
- The function has a limit to the number of attempts to place a building to prevent an infinite
loop if the parameters do not allow for the requested number of buildings.

Please note that both functions rely on the user providing a reasonable range of elevations and other parameters.
For realistic results, consider the scale and nature of the terrain or city you're trying to model.




## Visualization

This project provides both console and bitmap image visualization for the drone network. Each drone node,
connection line between nodes, and the influence of drone signals are rendered with distinct colors.
How it works

### Grayscale Mapping

The `getGrayscale(int elevation, int minElevation, int maxElevation)` function is used to generate a grayscale
value for a given elevation. This allows us to visualize elevation data on the map in shades of gray. The
function scales the elevation between the minimum and maximum elevations to a grayscale value between 0 and 255.

### Drawing Connection Lines

The `drawLine(Node* nodeA, Node* nodeB)` function uses a basic line-drawing algorithm (Bresenham's line algorithm)
to determine the set of points that represent a straight line between two nodes (drones). This is used to visualize
the connections between drones.

### Drone Position

The `isDronePosition(const std::vector<Node*>& nodes, int x, int y)` function checks if a given point (x, y)
corresponds to the position of any drone in the network.

### Drone Signal Influence

The `getTotalDroneInfluence(const std::vector<Node*>& nodes, int x, int y)` function calculates the total
influence of all drones at a particular point (x, y). The influence is calculated based on the distance
between the point and each drone, and the signal power of each drone.

### Writing Bitmap Images

Bitmap images are created using the `writeMapToBMP(...)` function. This function writes BMP headers,
then goes through each point in the topography, checking if the point corresponds to a drone position,
a line between drones, or an area influenced by a drone's signal. It writes the corresponding pixel data for each case.

### Console Rendering

The `printMapToConsole(...)` function is used to render the map to the console.
It uses ASCII characters to represent the drones, lines between drones, and the influence of drone signals.

In both types of visualization, drones are represented with blue color,
lines between drones are represented with red color,
and the areas influenced by drone signals are represented in a gradient color where darker
color indicates a stronger signal. The elevation is represented with grayscale colors where
darker color indicates a higher elevation.
   



  
