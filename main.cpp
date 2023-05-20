#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include "node/Node.h"
#include "worker/Workers.h"

using namespace std;

void update(std::vector<Node*>& nodes) {
    for (auto& node : nodes) {
        node->updateAllNodes(nodes);
    }
}

int main() {
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
    }


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
