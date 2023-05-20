#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include "node/Node.h"
#include "worker/Workers.h"
#include "topography/Topography.h"

using namespace std;

void update(std::vector<Node*>& nodes) {
    for (auto& node : nodes) {
        node->updateAllNodes(nodes);
    }
}

int main() {

    /*
    std::vector<Node> nodes;
    for(int i = 0; i < 10; i++) {
        nodes.emplace_back(i, i, i, i, 20);
    }

    // Update all nodes with the full list of nodes
    // As the is just a simulation, the easiest way for nodes to know they are in range is to have every
    // node position in every node


    std::vector<Node*> nodePointers;
    for(Node& node : nodes) {
        nodePointers.push_back(&node);
    }
    update(nodePointers);

    // Exchange routing tables between all pairs of nodes
    for(Node& node : nodes) {
        node.updateAllNodes(nodePointers);
    }

    for(Node& node : nodes) {
        node.broadcast();
    }
    for(Node& node : nodes) {
        node.broadcast();
    }
    // Print out routing tables for each node
    for (const auto& node : nodes) {
        node.printRoutingTable();
    }*/

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




    /*
    Workers worker_threads(4);
    worker_threads.start(); // Create 4 internal threads

    worker_threads.start();
    worker_threads.post([&node1] {

        cout << "Hello from thread " << this_thread::get_id() << endl;
    });

    worker_threads.stop();*/

    return 0;
}
