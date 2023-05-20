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

// This method makes sure that every node has a pointer to every other node. This is used for keeping track of the
// position of the nodes in the network. This way, the program can simulate the position of nodes. As every node
// knows the position of every other node, it is possible to calculate what nodes are reachable. In the real word,
// this would be done by using actual antennas and measuring the signal strength.
void updateNodePointers(vector<Node*>& nodes) {
    for (auto& node : nodes) {
        node->updateAllNodes(nodes);
    }
}

// This method broadcasts the routing table to all nodes in range. Nodes in range will update their routing table
// based on the information they receive.
void broadcastNodes(vector<Node*>& nodes, int numberOfBroadcasts) {
    for (int i = 0; i < numberOfBroadcasts; i++) {
        for (auto& node : nodes) {
            node->broadcast();
        }
    }
}

void printRoutingTables() {
    for (auto& node : nodes) {
        node.printRoutingTable();
    }
}

void generateImage() {
    Topography topography;//TODO: fix this
    //topography.generateImage(nodes);
}

void changeNodePosition(int nodeId, int x, int y, int z) {
    for (auto& node : nodes) {
        if (node.getId() == nodeId) {
            node.setPosition(x, y, z);
        }
    }
}

// Every node broadcasts its routing table every 15 seconds.
void regularBroadcasting() {
    while(!stop) {
        broadcastNodes(nodePointers, 1);
        // Sleep for 15 seconds
        this_thread::sleep_for(chrono::seconds(15));
    }
}

void startCLI() {
    string command;
    cout << "Use the \"help\" command if you are stuck\nEnter a command: " << endl;
    while(!stop) {
        cout << ">>";
        cin >> command;
        // Run your command
        if (command == "quit") {
            stop = true;
            break;
        }
        // Add more commands as per your requirement
        if(command == "help") {
            cout << "Commands: " << endl;
            cout << "quit: quit the program" << endl;
            cout << "print: print the routing tables of all nodes" << endl;
            cout << "generate: generate an image of the wireless mesh network with buildings" << endl;
            cout << "change: change the position of a node" << endl;
            cout << "create: create a new node" << endl;
            cout << "delete: delete a node" << endl;
            cout << "help: print this help message" << endl;
        } else if(command == "print") {
            printRoutingTables();
        } else if(command == "change") {
            int nodeId, x, y, z;
            cout << "Number of nodes in network is: " << nodes.size() << "\nEnter node id: \n>>";
            cin >> nodeId;
            cout << "Current node position: (" << nodes[nodeId].getX() << ", " << nodes[nodeId].getY() << ", " << nodes[nodeId].getZ() << ")" << endl;
            cout << "Enter x, y, z position: \n>>";
            cin >> x >> y >> z;
            changeNodePosition(nodeId, x, y, z);
            cout << "Node position changed!" << endl;
        }
    }
}

// Start the Wireless Mesh simulation with the given parameters.
void simulate(vector<tuple<int, int, int>> nodePositions, int numberOfBroadcasts, int signalStrength) {
    for(int i = 0; i < nodePositions.size(); i++) {
        int x = get<0>(nodePositions[i]);
        int y = get<1>(nodePositions[i]);
        int z = get<2>(nodePositions[i]);
        Node node(i, x, y, z, signalStrength);
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

void runWithLineNodes(int numberOfNodes, int numberOfBroadcasts, int signalStrength) {
    //TODO: make sure that nodes are not placed in buildings
    vector<tuple<int, int, int>> nodePositions;
    for(int i = 0; i < numberOfNodes; i++) {
        nodePositions.emplace_back(i*10, 0, 0); // Placing nodes in line (x-axis)
    }
    simulate(nodePositions, numberOfBroadcasts, signalStrength);
}

void runWithScatteredNodes(int numberOfNodes, int numberOfBroadcasts, int signalStrength) {
    //TODO: make sure that nodes are not placed on top of each other and not in buildings
    random_device dev;
    mt19937 rng(dev());
    vector<tuple<int, int, int>> nodePositions;
    uniform_int_distribution<mt19937::result_type> dist1(0, 500);
    uniform_int_distribution<mt19937::result_type> dist2(0, 20);
    for (int i = 0; i < numberOfNodes; i++) {
        int x = dist1(rng);
        int y = dist1(rng);
        int z = dist2(rng);
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
        printSimulationPreset(0, 10, 500, "Random");
        printSimulationPreset(1, 50, 300, "Random");
        printSimulationPreset(2, 100, 100, "Random");
        printSimulationPreset(3, 200, 100, "Random");

        cout << ">>";
        int simulation;
        cin >> simulation;

        switch (simulation) {
            case 0:
                cout << "Running simulation 0" << endl;
                runWithScatteredNodes(10, 10, 500);
                return;
            case 1:
                cout << "Running simulation 1" << endl;
                runWithScatteredNodes(50, 25, 300);
                return;
            case 2:
                cout << "Running simulation 2" << endl;
                runWithScatteredNodes(100, 50, 100);
                return;
            case 3:
                cout << "Running simulation 3" << endl;
                runWithScatteredNodes(200, 100, 400);
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
        cout << "[2]: Place the nodes in a line" << endl << ">>";
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
                simulate(nodePositions, numberOfNodes/2, signalStrength);
                break;
            case 1:
                cout << "Loading custom scattered nodes simulation with " << numberOfNodes << " nodes and " << signalStrength << " signal strength." << endl;
                runWithScatteredNodes(numberOfNodes, numberOfNodes/2, signalStrength);
                break;
            case 2:
                cout << "Loading custom line nodes simulation with " << numberOfNodes << " nodes and " << signalStrength << " signal strength." << endl;
                runWithLineNodes(numberOfNodes, numberOfNodes/2, signalStrength);
                break;
            default:
                cout << "Invalid choice. Retry with correct option" << endl;
                break;
        }

        for (const auto& node : nodes) {
            node.printRoutingTable();
        }
    }
}

// Run simulation
int main() {
    startSimulationConsole();
    cout << "Exiting program." << endl;

    Topography geoTest;


    std::vector<std::vector<int>> heightData = geoTest.readElevationData("output.txt");

    cout << "Hello World!" << endl;


    Node node1(1, 50, 50, 5, 10.0);
    Node node2(2, 150, 150, 10, 20.0);
    Node node3(3, 321, 250, 15, 100.0);
    Node node4(4, 213, 350, 20, 40.0);
    Node node5(5, 213, 450, 25, 100.0);

    // Add pointer of  them to a vector
    std::vector<Node*> nodes = {&node1, &node2, &node3, &node4, &node5};

    cout << "Hello World!" << endl;

    // Connect some of them together
    std::vector<std::pair<Node*, Node*>> connectedDrones = {
            {&node1, &node2},
            {&node2, &node5},
            {&node3, &node4},
            {&node4, &node1},
            {&node5, &node1},
    };

    cout << "Hello World!" << endl;

    std::string filename = "output.bmp";

    // Call the writeMapToBMP function with these test data
    geoTest.writeMapToBMP(heightData, nodes, connectedDrones, filename);

    return 0;
}
