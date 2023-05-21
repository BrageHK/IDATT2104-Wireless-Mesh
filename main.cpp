#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <random>
#include "node/Node.h"
#include "worker/Workers.h"
#include "topography/Topography.h"

using namespace std;

vector<Node> nodes;
vector<Node*> nodePointers;
bool stop = false;
Topography topography;
vector<vector<int>> heightData;
int fileNumber = 0;

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
    for (auto& node : nodes) {
        if (node.getId() == nodeId) {
            node.setPosition(x, y, z);
        }
    }
}

void getNodeInfo(int nodeId) {
    //print routing table and position
    for (auto& node : nodes) {
        if (node.getId() == nodeId) {
            cout << "----------- Node " << nodeId << " -----------" << endl;
            cout << "Position: (" << node.getX() << ", " << node.getY() << ", " << node.getZ() << ")" << endl;
            node.printRoutingTable();
        }
    }
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

void startCLI() {
    string command;
    cout << "Use the \"help\" command if you are stuck\nEnter a command: " << endl;
    while(!stop) {
        cout << ">> ";
        cin >> command;
        if (command == "quit" || command == "exit" || command == "q" || command == "e" || command == "stop" || command == "s" || command == "end" || command == "x") {
            stop = true;
        } else if (command == "help") {
            cout << "Commands:" << endl;
            cout << "quit: quit the program" << endl;
            cout << "print: print the routing tables of all nodes" << endl;
            cout << "nodeInfo: print information about a node" << endl;
            cout << "change: change the position of a node" << endl;
            cout << "create: create a new node" << endl;
            cout << "send: send a message from a node to another node. Also generates an image that shows the path chosen" << endl;
            cout << "help: print this help message" << endl;
        } else if (command == "print") {
            printRoutingTables();
        } else if (command == "change") {
            int nodeId, x, y, z;
            cout << "Enter the node ID: ";
            cin >> nodeId;
            cout << "Current node position: (" << nodes[nodeId].getX() << ", " << nodes[nodeId].getY() << ", " << nodes[nodeId].getZ() << ")" << endl;
            cout << "Enter the new position (x, y, z): ";
            cin >> x >> y >> z;
            changeNodePosition(nodeId, x, y, z);
            cout << "Node position changed!" << endl;
        } else if (command == "create") {
            int x, y, z, signalStrength;
            cout << "Enter the position for the new node (x, y, z): ";
            cin >> x >> y >> z;
            cout << "Enter the signal strength of the new node: ";
            cin >> signalStrength;
            int nodeId = nodes.size();
            Node node(nodeId, x, y, z, signalStrength, &topography); // Set signal strength to 0 for now
            nodes.push_back(node);
            nodePointers.push_back(&nodes[nodeId]);
            cout << "Node created with ID: " << nodeId << endl;
        } else if(command == "send") {
            vector<std::pair<Node*, Node*>> connectedDrones;
            cout << "There are " << nodes.size() << " nodes in the network." << endl;
            int senderId, receiverId;
            string message;
            cout << "Enter the ID of the sender: \n>>";
            cin >> senderId;
            cout << "Enter the ID of the receiver: \n>>";
            cin >> receiverId;
            cout << "Enter the message: \n>>";
            cin >> ws;
            getline(cin, message);
            sendMessage(senderId, receiverId, message, connectedDrones);
            string filename = "SimulationPictures/"+to_string(fileNumber)+".bmp";
            cout << "Generating image..." << endl;
            topography.writeMapToBMP(nodePointers, connectedDrones, filename);
            cout << "image saved to " << filename << endl;
            fileNumber++;
        } else if(command == "nodeInfo") {
            int nodeId;
            cout << "Enter the ID of the node: ";
            cin >> nodeId;
            getNodeInfo(nodeId);
        } else{
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
    uniform_int_distribution<mt19937::result_type> dist1(0, 499);
    uniform_int_distribution<mt19937::result_type> dist2(0, 20);
    for (int i = 0; i < numberOfNodes; i++) {
        int x = dist1(rng);
        int y = dist1(rng);
        int z = dist2(rng);
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
void startSimulationConsole() {
    cout << "------------- Wireless Mesh simulation -------------" << endl << endl;
    cout << "This program simulates a Wireless Mesh network." << endl;
    cout << "Do you want to run the simulation with predefined values? (y/n)" << endl;
    cout << ">>";
    string answer;
    cin >> answer;
    if (answer == "y") {

        cout << "Choose your simulation: [0-3]" << endl;
        cout << "------------- Simulation options -------------" << endl;
        cout << "\tNodes\tSignal Strength\tPosition" << endl;
        printSimulationPreset(0, 10, 5000, "Random");
        printSimulationPreset(1, 50, 3000, "Random");
        printSimulationPreset(2, 100, 1000, "Random");
        printSimulationPreset(3, 200, 400, "Random");
        cout << "WARNING: Simulation option 4 and 5 may take a long time to complete!" << endl;
        printSimulationPreset(4, 600, 100, "Random");
        printSimulationPreset(5, 750, 150, "Random");

        cout << ">>";
        int simulation;
        cin >> simulation;

        switch (simulation) {
            case 0:
                cout << "Running simulation 0" << endl;
                runWithScatteredNodes(10, 10, 5000);
                return;
            case 1:
                cout << "Running simulation 1" << endl;
                runWithScatteredNodes(50, 25, 3000);
                return;
            case 2:
                cout << "Running simulation 2" << endl;
                runWithScatteredNodes(100, 50, 1000);
                return;
            case 3:
                cout << "Running simulation 3" << endl;
                runWithScatteredNodes(200, 50, 400);
                return;
            case 4:
                cout << "Running simulation 4" << endl;
                runWithScatteredNodes(600, 200, 100);
                return;
            case 5:
                cout << "Running simulation 5" << endl;
                runWithScatteredNodes(750, 100, 150);
                return;
            default:
                cout << "Invalid input" << endl;
                return;
        }
    } else {
        cout << "Running simulation with user input values" << endl;
        cout << "Enter number of nodes: " << endl << ">>";
        int numberOfNodes;
        cin >> numberOfNodes;
        int signalStrength;
        cout << "Enter the signal strength for the nodes: " << endl << ">>";
        cin >> signalStrength;
        cout << "------------- How would you like to position the nodes? -------------" << endl;
        cout << "[0]: Choose the position of each node (not recommended if there are many nodes!)" << endl;
        cout << "[1]: Scatter the nodes randomly" << endl;
        int positionChoice;
        cin >> positionChoice;

        vector<tuple<int, int, int>> nodePositions;
        switch (positionChoice) {
            case 0:
                for(int i=0; i<numberOfNodes; i++) {
                    int x, y, z;
                    cout << "Enter position for Node " << i+1 << "(x, y, z): ";
                    cin >> x >> y >> z;
                    nodePositions.emplace_back(x, y, z);
                }
                cout << "Loading custom simulation with " << numberOfNodes << " nodes and " << signalStrength << " signal strength." << endl;
                simulate(nodePositions, numberOfNodes/4, signalStrength);
                break;
            case 1:
                cout << "Loading custom scattered nodes simulation with " << numberOfNodes << " nodes and " << signalStrength << " signal strength." << endl;
                runWithScatteredNodes(numberOfNodes, numberOfNodes/4, signalStrength);
                break;
            default:
                cout << "Invalid choice. Retry with correct option" << endl;
                break;
        }
    }
}

// Run simulation
int main() {
    //heightData = topography.readElevationData("output.txt");
    heightData = topography.generateCityElevation(500, 500, 20,80,1000,15,100);
    //heightData = topography.generateMountainElevation(500, 500, 0, 60);
    topography.setElevationData(heightData);

    startSimulationConsole();
    cout << "Exiting program." << endl;

    return 0;
}
