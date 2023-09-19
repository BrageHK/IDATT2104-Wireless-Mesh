#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <random>
#include "node/Node.h"
#include "worker/Workers.h"
#include "topography/Topography.h"
#include <map>
#include <functional>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

using namespace std;

vector<Node> nodes;
vector<Node*> nodePointers;
bool stop = false;
Topography topography;
vector<vector<int>> heightData;
int fileNumber = 0;
int width;
int height;

// This method makes sure that every node has a pointer to every other node. This is used for keeping track of the
// position of the inputNodes in the network. This way, the program can simulate the position of inputNodes. As every node
// knows the position of every other node, it is possible to calculate what inputNodes are reachable. In the real word,
// this would be done by using actual antennas and measuring the signal strength.
void updateNodePointers(vector<Node*>& inputNodes) {
    for (auto& node : inputNodes) {
        node->updateAllNodes(inputNodes);
    }
}

// This method broadcasts the routing table to all inputNodes in range. Nodes in range will update their routing table
// based on the information they receive.
void broadcastNodes(vector<Node*>& inputNodes, int numberOfBroadcasts) {
    for (int i = 0; i < numberOfBroadcasts; i++) {
        for (auto& node : inputNodes) {
            node->broadcast();
        }
    }
}

void printRoutingTables() {
    for (auto& node : nodes) {
        node.printRoutingTable();
    }
}

void changeNodePosition(int nodeId, int x, int y, int z) {
    Node& node = nodes[nodeId];
    node.setPosition(x, y, z);
}

void getNodeInfo(int nodeId) {
    // Print routing table and position
    const Node& node = nodes[nodeId];
    cout << "----------- Node " << nodeId << " -----------" << endl;
    cout << "Position: (" << node.getX() << ", " << node.getY() << ", " << node.getZ() << ")" << endl;
    node.printRoutingTable();
}

void sendMessage(int senderId, int receiverId, const string& message, vector<pair<Node*, Node*>>& connectedDrones) {
    for (auto& node : nodes) {
        if (node.getId() == senderId) {
            node.sendMessage(receiverId, message, connectedDrones);
        }
    }
}

// Every node broadcasts its routing table every 15 seconds.
void regularBroadcasting() {
    while(!stop) {
        broadcastNodes(nodePointers, 1);
        // Sleep for 15 seconds
        this_thread::sleep_for(chrono::seconds(5));
    }
}

pair<int, int> getTerminalSize() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int columns = w.ws_col;
    int rows = w.ws_row;
#endif

    return {columns, rows};
}

void printHelp() {
    cout << "Commands:" << endl;
    cout << "quit: quit the program" << endl;
    cout << "print: print the routing tables of all nodes" << endl;
    cout << "nodeInfo: print information about a node" << endl;
    cout << "change: change the position of a node" << endl;
    cout << "create: create a new node" << endl;
    cout << "send: send a message from a node to another node. Also generates an image that shows the path chosen" << endl;
    cout << "help: print this help message" << endl;
    cout << "save: save the topography to a file" << endl;
}


void changeNodePositionCLI() {
    int nodeId, x, y, z;

    cout << "Enter the node ID: ";
    while (!(cin >> nodeId) || nodeId < 0 || nodeId >= nodes.size()) {
        cout << "Invalid node ID. Please enter a valid node ID: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Current node position: (" << nodes[nodeId].getX() << ", " << nodes[nodeId].getY() << ", " << nodes[nodeId].getZ() << ")" << endl;

    cout << "Enter the new position (x, y, z): ";
    while (!(cin >> x >> y >> z) || x < 0 || y < 0 || z < 0 || x > width-1 || y > height-1) {
        cout << "Invalid position. Please enter valid x, y, and z coordinates: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    changeNodePosition(nodeId, x, y, z);

    cout << "Node position changed!" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover newline character

}

void createNodeCLI() {
    double signalStrength;
    int x, y, z;

    cout << "Enter the position for the new node (x, y, z): ";
    while (!(cin >> x >> y >> z) || x < 0 || y < 0 || z < 0 || x > width-1 || y > height-1) {
        cout << "Invalid position. Please enter valid x, y, and z coordinates: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter the signal strength of the new node: ";
    while (!(cin >> signalStrength)) {
        cout << "Invalid signal strength. Please enter a valid numeric value: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int nodeId = nodes.size();
    Node node(nodeId, x, y, z, signalStrength, &topography); // Set signal strength to 0 for now
    nodes.push_back(node);
    nodePointers.push_back(&nodes[nodeId]);

    cout << "Node created with ID: " << nodeId << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover newline character

}

void sendMessageCLI() {
    vector<pair<Node*, Node*>> connectedDrones;
    cout << "\nThere are " << nodes.size() << " nodes in the network." << endl;
    int senderId, receiverId;
    string message;
    do {
        cout << "Enter the ID of the sender: \n>>";
        cin >> senderId;
    } while(senderId < 0 || senderId >= nodes.size());
    do {
        cout << "Enter the ID of the receiver: \n>>";
        cin >> receiverId;
    } while(receiverId < 0 || receiverId >= nodes.size());
    cout << "Enter the message: \n>>";
    cin >> ws;
    getline(cin, message);


    int choice;
    cout << "[1]: Only send message" << endl;
    cout << "[2]: Send message and generate image to file" << endl;
    cout << "[3]: Send message and print image to console (not recommended if width is wider than the console)" << endl;
    cout << ">> ";
    cin >> choice;

    sendMessage(senderId, receiverId, message, connectedDrones);

    if(choice == 2) {
        std::string directory = "SimulationPictures";
        if (!std::filesystem::exists(directory)) {
            std::filesystem::create_directory(directory);
        }
        std::string filename = directory + "/" + std::to_string(fileNumber) + ".bmp";
        std::cout << "Generating image file. Please wait..." << std::endl;
        topography.writeMapToBMP(nodePointers, connectedDrones, filename);
        std::cout << "image saved to " << filename << std::endl;
        fileNumber++;
    }
    if(choice == 3) {
        topography.printMapToConsole(nodePointers, connectedDrones);
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover newline character

}

void getNodeInfoCLI() {
    int nodeId;

    cout << "Enter the ID of the node: ";
    while (!(cin >> nodeId) || nodeId < 0 || nodeId >= nodes.size()-1) {
        cout << "Invalid node ID. Please enter a valid node ID: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    getNodeInfo(nodeId);
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover newline character

}

void saveElevations(){
    string filename;
    cout << "Enter a filename: ";
    cin >> filename;
    topography.writeElevationData(filename);
    cout << "Elevation data saved to " << filename << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover newline character

}

//todo sjekk om lese og skrive til fil funker
void startCLI() {

    string command;
    map<string, function<void()>> commandHandlers;

    commandHandlers["quit"] = commandHandlers["exit"] = commandHandlers["q"] =
    commandHandlers["e"] = commandHandlers["stop"] = commandHandlers["s"] =
    commandHandlers["end"] = commandHandlers["x"] = [&]() { stop = true; };

    commandHandlers["help"] = printHelp;
    commandHandlers["print"] = printRoutingTables;
    commandHandlers["change"] = changeNodePositionCLI;
    commandHandlers["create"] = createNodeCLI;
    commandHandlers["send"] = sendMessageCLI;
    commandHandlers["nodeInfo"] = getNodeInfoCLI;
    commandHandlers["save"] = saveElevations;

    cout << "Use the \"help\" command if you are stuck" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover newline character

    while(!stop) {
        cout << "Enter a command:" << endl;
        cout << ">> ";
        getline(cin, command);

        auto commandHandler = commandHandlers.find(command);
        if (commandHandler != commandHandlers.end()) {
            commandHandler->second();
        } else {
            cout << "Invalid command! Use the \"help\" command to see available commands." << endl;
            cin.clear();
        }
    }
}

// Start the Wireless Mesh simulation with the given parameters.
void simulate(vector<tuple<int, int, int>> nodePositions, int numberOfBroadcasts, int signalStrength) {
    for(int i = 0; i < nodePositions.size(); i++) {
        int x = get<0>(nodePositions[i]);
        int y = get<1>(nodePositions[i]);
        int z = get<2>(nodePositions[i]);
        Node node(i, x, y, z, signalStrength, &topography);

        nodes.push_back(node);
    }

    for(auto& node : nodes) {
        nodePointers.push_back(&node);
    }
    updateNodePointers(nodePointers);

    broadcastNodes(nodePointers, numberOfBroadcasts);

    Workers worker_threads(2);
    worker_threads.start(); // Create 4 internal threads

    worker_threads.post([] {
        regularBroadcasting();
    });

    worker_threads.post([] {
        startCLI();
    });

    worker_threads.stop();

}

// Nodes may share the same position, but they can't be placed inside a building.
void runWithScatteredNodes(int numberOfNodes, int numberOfBroadcasts, int signalStrength) {
    random_device dev;
    mt19937 rng(dev());
    vector<tuple<int, int, int>> nodePositions;
    uniform_int_distribution<mt19937::result_type> distX(1, height-1);
    uniform_int_distribution<mt19937::result_type> distY(1, width-1);
    uniform_int_distribution<mt19937::result_type> distZ(1, 20);
    for (int i = 0; i < numberOfNodes; i++) {
        int x = distX(rng);
        int y = distY(rng);
        int z = distZ(rng);
        if(topography.getHeight(x, y) > z) {
            z += topography.getHeight(x, y) + 1;
        }
        nodePositions.emplace_back(x, y, z);
    }
    simulate(nodePositions, numberOfBroadcasts, signalStrength);
}

void printSimulationPreset(int option, int numberOfNodes, int signalStrength, string position) {
    cout << "[" << option << "]:\t" << numberOfNodes << "\t" << signalStrength << "\t\t" << position << endl;
}

// Opens a console where the user can control the simulation.

struct Simulation {
    int id;
    int nodes;
    int signalStrength;
    std::string description;
};

std::vector<Simulation> simulations = {
        {0, 10, 5000, "Nodes may not connected"},
        {1, 30, 7000, "Fast simulation"},
        {2, 50, 6000, "Fast simulation"},
        {3, 100, 2000, "Medium simulation"},
        {4, 200, 400, "Expect to wait some seconds"},
        {5, 600, 70, "May take very long to simulate"},
        {6, 750, 70, "May take very long to simulate"},
        {7, 30, 50, "Good for visualizing in the terminal"},
        {8, 100, 20, "Good for visualizing in the terminal"}
};

void printSimulationOptions() {
    cout << "------------- Simulation options -------------" << endl;
    cout << "\tNodes\tSignal Strength\tInfo" << endl;
    for (auto &simulation : simulations) {
        printSimulationPreset(simulation.id, simulation.nodes, simulation.signalStrength, simulation.description);
    }
}

void runSimulation(int simulationId) {
    const auto& sim = simulations[simulationId];
    std::cout << "Running simulation " << sim.id << std::endl;
    runWithScatteredNodes(sim.nodes, sim.nodes / 4, sim.signalStrength);
}

void startSimulationConsole() {
    std::cout << "------------- Wireless Mesh simulation -------------" << std::endl << std::endl;
    std::cout << "This program simulates a Wireless Mesh network." << std::endl;
    std::string answer;
    do {
        std::cout << "Do you want to run the simulation with predefined values? (y/n)" << std::endl;
        std::cout << ">>";
        std::cin >> answer;
    } while (answer != "y" && answer != "n");

    if (answer == "y") {
        printSimulationOptions();
        int simulation;
        do {
            std::cout << ">>";
            std::cin >> simulation;
            if (simulation < 0 || simulation >= simulations.size()) {
                std::cout << "Invalid input. Please try again." << std::endl;
            }
        } while (simulation < 0 || simulation >= simulations.size());
        runSimulation(simulation);
    } else {
        cout << "Running simulation with user input values" << endl;
        int numberOfNodes;
        do {
            cout << "Enter number of nodes: " << endl << ">>";
            cin >> numberOfNodes;
            if (numberOfNodes <= 0) {
                cout << "Number of nodes must be greater than 0. Please try again." << endl;
            }
        } while (numberOfNodes <= 0);

        double signalStrength;
        do {
            cout << "Enter the signal strength for the nodes: " << endl << ">>";
            cin >> signalStrength;
            if (signalStrength < 0) {
                cout << "Signal strength cannot be negative. Please try again." << endl;
            }
        } while (signalStrength < 0);

        int positionChoice;
        do {
            cout << "------------- How would you like to position the nodes? -------------" << endl;
            cout << "[0]: Choose the position of each node (not recommended if there are many nodes!)" << endl;
            cout << "[1]: Scatter the nodes randomly" << endl << ">>";
            cin >> positionChoice;
            if (positionChoice < 0 || positionChoice > 1) {
                cout << "Invalid choice. Please select 0 or 1." << endl;
            }
        } while (positionChoice < 0 || positionChoice > 1);

        vector<tuple<int, int, int>> nodePositions;
        switch (positionChoice) {
            case 0:
                for(int i=0; i<numberOfNodes; i++) {
                    int x, y, z;
                    cout << "Enter position for Node " << i+1 << "(x, y, z): ";
                    while (!(cin >> x >> y >> z) || x < 0 || y < 0 || z < 0 || x > width-1 || y > height-1) {
                        cout << "Invalid position. Please enter valid x, y, and z coordinates: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    nodePositions.emplace_back(x, y, z);
                }
                cout << "Loading custom simulation with " << numberOfNodes << " nodes and " << signalStrength << " signal strength." << endl;
                simulate(nodePositions, 70, signalStrength);
                break;
            case 1:
                cout << "Loading custom scattered nodes simulation with " << numberOfNodes << " nodes and " << signalStrength << " signal strength." << endl;
                runWithScatteredNodes(numberOfNodes, 70, signalStrength);
                break;
        }
    }
}



// Run simulation
int main() {
    pair<int, int> terminalSize = getTerminalSize();
    width = terminalSize.first;
    height = terminalSize.second;

    int topographyChoice;
    do {
        cout << "Choose the type of topography: \n[0]: Default City \n[1]: Default Mountain \n[2]: Custom City \n[3]: Custom Mountain \n[4]: Import from file \n[5]: Console-sized City \n[6]: Console-sized Mountain" << endl << ">>";
        cin >> topographyChoice;
        if (topographyChoice < 0 || topographyChoice > 6) {
            cout << "Invalid choice. Please enter a number between 0 and 6." << endl;
        }
    } while (topographyChoice < 0 || topographyChoice > 6);

    string filename;
    int customWidth = -1, customHeight = -1;

    switch(topographyChoice) {
        case 0:
            heightData = topography.generateCityElevation(500, 500, 20, 80, 1000, 15, 100);
            cout << "You chose default City topography." << endl;
            break;
        case 1:
            heightData = topography.generateMountainElevation(500, 500, 0, 60);
            cout << "You chose default Mountain topography." << endl;
            break;
        case 2:
            while (customWidth <= 0 || customHeight <= 0) {
                cout << "Enter the dimensions for the custom topography (positive width and height): " << endl << ">>";
                cin >> customWidth >> customHeight;
                if (customWidth <= 0 || customHeight <= 0) {
                    cout << "Dimensions should be positive. Please enter again." << endl;
                }
            }
            heightData = topography.generateCityElevation(customHeight, customWidth, 20, 80, 1000, 15, 100);
            cout << "You chose custom City topography." << endl;
            break;
        case 3:
            while (customWidth <= 0 || customHeight <= 0) {
                cout << "Enter the dimensions for the custom topography (positive width and height): " << endl << ">>";
                cin >> customWidth >> customHeight;
                if (customWidth <= 0 || customHeight <= 0) {
                    cout << "Dimensions should be positive. Please enter again." << endl;
                }
            };
            heightData = topography.generateMountainElevation(customHeight, customWidth, 0, 60);
            cout << "You chose custom Mountain topography." << endl;
            break;
        case 4:
            cout << "You chose to load topography from file." << endl;
            cout << "Enter filename: " << endl << ">>";
            cin >> filename;
            {
                ifstream ifile(filename);
                if (ifile) { // Check if file exists
                    heightData = topography.readElevationData(filename);
                } else {
                    cout << "File " << filename << " does not exist. Defaulting to City topography." << endl;
                    heightData = topography.generateCityElevation(500, 500, 20, 80, 1000, 15, 100);
                }
            }
            break;
        case 5:
            heightData = topography.generateCityElevation(width, width, 20, 80, 1000, 5, 25);
            cout << "You chose console-sized City topography." << endl;
            break;
        case 6:
            heightData = topography.generateMountainElevation(width, width, 0, 60);
            cout << "You chose console-sized Mountain topography." << endl;
            break;
        default:
            cout << "Invalid choice, defaulting to City topography." << endl;
            heightData = topography.generateCityElevation(500, 500, 20, 80, 1000, 5, 100);
            break;
    }
    width = heightData[0].size();
    height = heightData.size();
    topography.setElevationData(heightData);
    startSimulationConsole();

    cout << "Exiting program." << endl;
    return 0;
}
