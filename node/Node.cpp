#include "Node.h"

Node::Node(int nodeId, int xPos, int yPos, int zPos, double power) : id(nodeId), x(xPos), y(yPos), z(zPos), signalPower(power) {

    routingTable[id] = {id, id, 0.0, 0};
}

void Node::receiveRoutingTable(const std::unordered_map<int, RoutingTableEntry>& receivedTable, int neighborId) {
    neighborTables[neighborId] = receivedTable;
    updateRoutingTable();
}

// This method sends routing table information to other nodes in range to update the other nodes.
void Node::broadcast() {
    for(Node* node : allNodes) {
        if(node->id != this->id) {  // Do not send to self
            double signalStrength = calculateSignalStrength(node->x, node->y, node->z);
            if(signalStrength >= 0.1) {  // Only send to nodes within range
                sendRoutingTable(*node);
            }
        }
    }
}

std::vector<Node*> Node::getNodesInRadius() {
    std::vector<Node*> nodesInRadius;
    for (Node* otherNode : allNodes) {
        if (this->id != otherNode->id) { // Skip the node itself
            double signalStrength = calculateSignalStrength(otherNode->x, otherNode->y, otherNode->z);
            if (signalStrength >= 0.2f) {
                nodesInRadius.push_back(otherNode);
            }
        }
    }
    return nodesInRadius;
}

void Node::updateRoutingTable() {
    std::unordered_map<int, RoutingTableEntry> newRoutingTable;
    for (const auto& neighbor : neighborTables) {
        for (const auto& entry : neighbor.second) {
            int destination = entry.first;
            const RoutingTableEntry& neighborEntry = entry.second;

            double signalStrength = calculateSignalStrength(allNodes[neighbor.first]->x, allNodes[neighbor.first]->y, allNodes[neighbor.first]->z);
            if (signalStrength > 0 && (newRoutingTable.count(destination) == 0 ||
                                       signalStrength > newRoutingTable[destination].signalStrength ||
                                       (signalStrength == newRoutingTable[destination].signalStrength &&
                                        neighborEntry.sequenceNumber > newRoutingTable[destination].sequenceNumber))) {
                newRoutingTable[destination] = neighborEntry;
                newRoutingTable[destination].nextHop = neighbor.first;
                newRoutingTable[destination].signalStrength = signalStrength;
            }
        }
    }

    routingTable = newRoutingTable;
    printRoutingTable();
}




void Node::sendRoutingTable(Node& neighbor) {
    neighbor.receiveRoutingTable(routingTable, id);
}

void Node::exchangeRoutingTableWith(Node& other) {
    sendRoutingTable(other);
    other.sendRoutingTable(*this);
}

void Node::AddRoute(RoutingTableEntry& entry, int id) {
    routingTable[id] = entry;
}

double Node::calculateSignalStrength(int destX, int destY, int destZ) {
    double distance = std::sqrt(std::pow(x - destX, 2) + std::pow(y - destY, 2) + std::pow(z - destZ, 2));
    double signalStrength = signalPower / (4.0 * M_PI * distance * distance);

    std::cout << "Signal strength from Node " << id << " to Node " << destX << " is " << signalStrength << std::endl;

    // Check if the signal strength is too low
    if (signalStrength < 0.1) {
        return -1.0; // Return -1 indicating a weak signal
    }
    if(signalStrength > signalPower) {
        return signalPower;
    }

    return signalStrength;
}

void Node::printRoutingTable() const {
    std::cout << "Routing Table for Node " << id << ":" << std::endl;
    for (const auto& entry : routingTable) {
        std::cout << "id: " << entry.first;
        std::cout << ", Destination: " << entry.second.destination;
        std::cout << ", Next Hop: " << entry.second.nextHop;
        std::cout << ", Signal Strength: " << entry.second.signalStrength;
        std::cout << ", Sequence Number: " << entry.second.sequenceNumber << std::endl;
    }
    std::cout << std::endl;
}

void Node::printNeighborTables() const {
    std::cout << "Neighbor tables for node: " << id << ":" << std::endl;
    for (const auto& table : neighborTables) {
        std::cout << table.first << ", size: " << table.second.size() << ". ";
    }
}

void Node::updateAllNodes(std::vector<Node*> &allNodes) {
    this->allNodes = allNodes;
}
