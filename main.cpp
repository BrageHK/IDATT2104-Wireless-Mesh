#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <random>
#include "node/Node.h"
#include "worker/Workers.h"

using namespace std;

vector<Node> nodes;
vector<Node*> nodePointers;

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

void printRoutingTables(vector<Node*>& nodes) {
    for (auto& node : nodes) {
        node->printRoutingTable();
    }
}

void changeNodePosition(int nodeId, int x, int y, int z) {
    for (auto& node : nodes) {
        if (node.getId() == nodeId) {
            node.setPosition(x, y, z);
        }
    }
}

void startCLI() {
    string command;
    cout << "Enter a command: " << endl;
    while(true) {
        cin >> command;
        // Run your command
        if (command == "quit") {
            break;
        }
        // Add more commands as per your requirement
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

    startCLI();
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
    cout << "[" << option << "]:\t" << numberOfNodes << "\t\t" << signalStrength << "\t\t" << position << endl;
}

// Opens a console where the user can control the simulation.
void startSimulationConsole() {
    cout << "------------- Wireless Mesh simulation -------------" << endl << endl;
    cout << "This program simulates a Wireless Mesh network." << endl;
    cout << "The program can be exited at any time by typing '69'." << endl << endl;
    cout << "Do you want to run the simulation with predefined values? (y/n)" << endl;
    string answer;
    cin >> answer;
    if (answer == "y") {
        chooseSimulation:
            cout << "Choose your simulation: [0-3]" << endl;
            cout << "------------- Simulation options -------------" << endl;
            cout << "\tNodes\tBroadcasts\tsignal Strength\tPosition" << endl;
            printSimulationPreset(0, 10, 500, "Random");
            printSimulationPreset(1, 50, 300, "Random");
            printSimulationPreset(2, 100, 100, "Random");
            printSimulationPreset(3, 200, 20, "Random");
        
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
                    runWithScatteredNodes(200, 75, 20);
                    return;
                case 69:
                    return;
                default:
                    cout << "Invalid input, try again." << endl;
                    goto chooseSimulation;
            }
    } else {
        cout << "Running simulation with user input values" << endl;
        cout << "Enter number of nodes: " << endl;
        int numberOfNodes;
        cin >> numberOfNodes;
        cout << "Enter number of broadcasts: " << endl;
        int numberOfBroadcasts;
        cin >> numberOfBroadcasts;
        int signalStrength;
        cout << "Enter the signal strength for the nodes: " << endl;
        cin >> signalStrength;
        cout << "------------- How would you like to position the nodes? -------------" << endl;
        cout << "[0]:Choose the position of each node" << endl;
        cout << "[1]:Scatter the nodes randomly" << endl;
        cout << "[2]:Place the nodes in a line" << endl;
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
                simulate(nodePositions, numberOfBroadcasts, signalStrength);
                break;
            case 1:
                runWithScatteredNodes(numberOfNodes, numberOfBroadcasts, signalStrength);
                break;
            case 2:
                runWithLineNodes(numberOfNodes, numberOfBroadcasts, signalStrength);
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
    Workers worker_threads(4);
    worker_threads.start(); // Create 4 internal threads

    worker_threads.post([] {
        // wait 3 seconds
        this_thread::sleep_for(chrono::seconds(3));
        cout << "last " << this_thread::get_id() << endl;
    });
    worker_threads.post([] {
        cout << "eHello  " << this_thread::get_id() << endl;
    });
    worker_threads.post([] {
        cout << "Hello  " << this_thread::get_id() << endl;
    });
    worker_threads.post([] {
        cout << "Hello  " << this_thread::get_id() << endl;
    });
    worker_threads.post([] {
        cout << "Hello fr" << this_thread::get_id() << endl;
    });
    worker_threads.post([] {
        cout << "first? " << this_thread::get_id() << endl;
    });

    worker_threads.stop();
    startSimulationConsole();
    cout << "Exiting program." << endl;

    /*int numberOfNodes = 10;
    int numberOfBroadcasts = 10;

    // Create and init 10 nodes

    for(int i = 0; i < numberOfNodes; i++) {
        nodes.emplace_back(i, i, i, i, 20);
    }

    for(int i = numberOfNodes; i < numberOfNodes*2; i++) {
        nodes.emplace_back(i, i, i, i, 0);
    }

    // Update all nodes with the full list of nodes
    // As the is just a simulation, the easiest way for nodes to know they are in range is to have every
    // node position in every node.
    // Each node has access to every other node, but this is just used for getting the position of the other nodes.

    for(Node& node : nodes) {
        nodePointers.push_back(&node);
    }
    updateNodePointers(nodePointers);

    // Exchange routing tables between all pairs of nodes
    for(Node& node : nodes) {
        node.updateAllNodes(nodePointers);
    }

    broadcastNodes(nodePointers, numberOfBroadcasts);

    // Print out routing tables for each node
    for (const auto& node : nodes) {
        node.printRoutingTable();
    }*/

    return 0;
}
